/* G.O.L.F.
// Copyright (C) 2015 LucKey Productions (luckeyproductions.nl)
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "mastercontrol.h"
#include "golfcam.h"
#include "inputmaster.h"
#include "cellmaster.h"

//namespace Urho3D {
//template <> unsigned MakeHash(const golf::PlayerIndex& index)
//{
//    return golf::PlayerIndexToHash(index);
//}
//}

URHO3D_DEFINE_APPLICATION_MAIN(MasterControl);

MasterControl::MasterControl(Context *context):
    Application(context),
    cache_{GetSubsystem<ResourceCache>()},
    paused_{false},
    stepInterval_{0.23f},
    sinceStep_{stepInterval_}
{
    assert(cache_);

    human_[static_cast<int>(golf::PlayerIndex::player1)] = true;
    human_[static_cast<int>(golf::PlayerIndex::player2)] = true;
}


void MasterControl::Setup()
{
    // Modify engine startup parameters.
    // Set custom window title.
    engineParameters_["WindowTitle"] = "G.O.L.F.";
    engineParameters_["LogName"] = GetSubsystem<FileSystem>()->GetAppPreferencesDir("urho3d", "logs")+"golf.log";
//    engineParameters_["FullScreen"] = false;
//    engineParameters_["Headless"] = false;
//    engineParameters_["WindowWidth"] = 960;
//    engineParameters_["WindowHeight"] = 600;
}
void MasterControl::Start()
{
    new InputMaster(context_, this);
    graphics_ = GetSubsystem<Graphics>();
    game_ = new golf::Game();

    // Get default style
    defaultStyle_ = cache_->GetResource<XMLFile>("UI/DefaultStyle.xml");
    //Create console and debug HUD.
    CreateConsoleAndDebugHud();
    //Create the scene content
    CreateScene();
    //Create the UI content
    CreateUI();
    //Hook up to the frame update and render post-update events
    SubscribeToEvents();

    Sound* music = cache_->GetResource<Sound>("Resources/Music/GameOfDeath.ogg");
    music->SetLooped(true);
    Node* musicNode = world_.scene_->CreateChild("Music");
    SoundSource* musicSource = musicNode->CreateComponent<SoundSource>();
    musicSource->SetSoundType(SOUND_MUSIC);
    musicSource->Play(music);

}

void MasterControl::SubscribeToEvents()
{
    //Subscribe HandleUpdate() function for processing update events
    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(MasterControl, HandleUpdate));
    //Subscribe scene update event.
    SubscribeToEvent(E_SCENEUPDATE, URHO3D_HANDLER(MasterControl, HandleSceneUpdate));
}

void MasterControl::Stop()
{
    engine_->DumpResources(true);
}

void MasterControl::Exit()
{
    engine_->Exit();
}

void MasterControl::CreateConsoleAndDebugHud()
{
    // Create console
    Console* console = engine_->CreateConsole();
    console->SetDefaultStyle(defaultStyle_);
    console->GetBackground()->SetOpacity(0.8f);

    // Create debug HUD.
    DebugHud* debugHud = engine_->CreateDebugHud();
    debugHud->SetDefaultStyle(defaultStyle_);
}

void MasterControl::CreateUI()
{
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    assert(cache);
    UI* ui = GetSubsystem<UI>();
    assert(ui);
    //Create a Cursor UI element because we want to be able to hide and show it at will. When hidden, the mouse cursor will control the camera
    world_.cursor_.uiCursor_ = new Cursor(context_);
    world_.cursor_.uiCursor_->SetVisible(true);
    ui->SetCursor(world_.cursor_.uiCursor_);

    //Set starting position of the cursor at the rendering window center
    world_.cursor_.uiCursor_->SetPosition(graphics_->GetWidth()/2, graphics_->GetHeight()/2);

    //Construct new Text object, set string to display and font to use
    Text* instructionText = ui->GetRoot()->CreateChild<Text>();
    assert(instructionText);
    instructionText->SetText("Game Of Life Fighter");
    instructionText->SetHorizontalAlignment(HA_CENTER);
    instructionText->SetVerticalAlignment(VA_CENTER);
    instructionText->SetPosition(0, ui->GetRoot()->GetHeight()/2.1);
}

void MasterControl::CreateScene()
{
    world_.scene_ = new Scene(context_);
    assert(world_.scene_);

    //Create octree, use default volume (-1000, -1000, -1000) to (1000,1000,1000)
    world_.scene_->CreateComponent<Octree>();
    world_.physics_ = world_.scene_->CreateComponent<PhysicsWorld>();
//    world_.physics_->SetEnabled(false);
    world_.scene_->CreateComponent<DebugRenderer>();

    //Create static lights
    Node* blueLightNode = world_.scene_->CreateChild("Sun");
    blueLightNode->SetPosition(Vector3(10.0f, 2.0f, 0.0f));
    blueLightNode->LookAt(Vector3::ZERO);
    Light* blueLight = blueLightNode->CreateComponent<Light>();
    blueLight->SetLightType(LIGHT_DIRECTIONAL);
    blueLight->SetBrightness(1.0f);
    blueLight->SetColor(Color(0.1f, 0.5f, 1.0f));

    Node* redLightNode = world_.scene_->CreateChild("Sun");
    assert(redLightNode);
    redLightNode->SetPosition(Vector3(-10.0f, 2.0f, 0.0f));
    redLightNode->LookAt(Vector3::ZERO);
    Light* redLight = redLightNode->CreateComponent<Light>();
    assert(redLight);
    redLight->SetLightType(LIGHT_DIRECTIONAL);
    redLight->SetBrightness(1.0f);
    redLight->SetColor(Color(1.0f, 0.5f, 0.1f));

    cellMaster_ = new CellMaster(context_, this);

    //Create camera(s)
    world_.cameras_[static_cast<int>(golf::PlayerIndex::player1)] = new GOLFCam(context_, this, golf::PlayerIndex::player1);
    /*world_.cameras_[static_cast<int>(golf::PlayerIndex::player2)] = new GOLFCam(context_, this, golf::PlayerIndex::player2);

    int numViewports = Clamp(GetNumHumans(),1,4);
    renderer_->SetNumViewports(numViewports);
    // Set up the front camera viewport
    if (GetNumHumans()){
        for (int p = 0; p < human_.Size(); ++p){
            int playerIndex = static_cast<int>(p);
            int cameraIndex = 0;
            int viewportWidth = graphics_->GetWidth() / (1 + (GetNumHumans() > 1));
            int viewportHeight = graphics_->GetHeight() / (1 + (GetNumHumans() > 2));
            if (human_[playerIndex]){
                Viewport* player1Viewport{new Viewport(context_, world_.scene_, world_.cameras_[playerIndex]->camera_, IntRect(viewportWidth*(cameraIndex%2),viewportHeight*(cameraIndex>2), viewportWidth, viewportHeight))};
                renderer_->SetViewport(0, player1Viewport);
                ++cameraIndex;
            }
        }
    } else {
        SharedPtr<Viewport> viewport(new Viewport(context_, world_.scene_, world_.cameras_[static_cast<int>(golf::PlayerIndex::player1)]->camera_,IntRect(0,0, graphics_->GetWidth(), graphics_->GetHeight())));
        renderer_->SetViewport(0, viewport);
    }*/
}

Color MasterControl::RandomColor()
{
    float color = Random(3.0f);
    float red = Clamp(color, 0.0f, 1.0f)/100.0f;
    float green = Clamp(color-1.0f, 0.0f, 1.0f)/100.0f;
    float blue = Clamp(color-2.0f, 0.0f, 1.0f)/100.0f;
    return Color(red, green, blue);
}

int MasterControl::GetNumHumans() const
{
    int numHumans = 0;
    for (int p = 0; p < human_.Size(); ++p){
//        if (human_[p]) ++numHumans;
    }
    return numHumans;
}

void MasterControl::HandleUpdate(StringHash eventType, VariantMap &eventData)
{
    sinceStep_ += eventData[Update::P_TIMESTEP].GetFloat();
    if (sinceStep_ > stepInterval_) {
        game_->Next();
        cellMaster_->UpdateCells();
        sinceStep_ -= stepInterval_;
    }
}
void MasterControl::HandleSceneUpdate(StringHash eventType, VariantMap &eventData)
{
}
void MasterControl::HandlePostRenderUpdate(StringHash eventType, VariantMap &eventData)
{
}

bool MasterControl::OctreeRayCast(PODVector<RayQueryResult> &hitResults, Ray ray, float distance)
{
    RayOctreeQuery query(hitResults, ray, RAY_TRIANGLE, distance, DRAWABLE_GEOMETRY);
    world_.octree_->Raycast(query);
    if (hitResults.Size()) return true;
    else return false;
}

float MasterControl::MinAngle(float lhs, float rhs)
{
    float angle = lhs - rhs;
    if (angle > 180.0f) angle -= 360.0f;
    else if (angle < -180.0f) angle += 360.0f;
    return angle;
}
