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

#include <Urho3D/Urho3D.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Engine/Console.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Engine/DebugHud.h>
#include <Urho3D/DebugNew.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/ValueAnimation.h>
#include <Urho3D/Physics/PhysicsWorld.h>
#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/Graphics/Geometry.h>
#include <Urho3D/Graphics/VertexBuffer.h>
#include <Urho3D/Graphics/IndexBuffer.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Technique.h>
#include <Urho3D/Graphics/RenderPath.h>
#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/XMLFile.h>
#include <Urho3D/Resource/Resource.h>
#include <Urho3D/Audio/Sound.h>
#include <Urho3D/Audio/SoundSource.h>

#include <Urho3D/IO/Log.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/OctreeQuery.h>

#include "mastercontrol.h"
#include "golfcam.h"
#include "inputmaster.h"
#include "cellmaster.h"

DEFINE_APPLICATION_MAIN(MasterControl);

MasterControl::MasterControl(Context *context):
    Application(context),
    cache_{GetSubsystem<ResourceCache>()},
    renderer_{GetSubsystem<Renderer>()},
    paused_(false),
    stepInterval_{0.5f},
    sinceStep_{stepInterval_}
{
}


void MasterControl::Setup()
{
    // Modify engine startup parameters.
    //Set custom window title and icon.
    engineParameters_["WindowTitle"] = "G.O.L.F.";
    engineParameters_["LogName"] = GetSubsystem<FileSystem>()->GetAppPreferencesDir("urho3d", "logs")+"golf.log";
    engineParameters_["FullScreen"] = true;
//    engineParameters_["Headless"] = false;
//    engineParameters_["WindowWidth"] = 960;
//    engineParameters_["WindowHeight"] = 600;
}
void MasterControl::Start()
{
    new InputMaster(context_, this);
    graphics_ = GetSubsystem<Graphics>();

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
    Node* musicNode = world.scene->CreateChild("Music");
    SoundSource* musicSource = musicNode->CreateComponent<SoundSource>();
    musicSource->SetSoundType(SOUND_MUSIC);
    musicSource->Play(music);

    game_ = new golf::Game();
}

void MasterControl::SubscribeToEvents()
{
    //Subscribe HandleUpdate() function for processing update events
    SubscribeToEvent(E_UPDATE, HANDLER(MasterControl, HandleUpdate));
    //Subscribe scene update event.
    SubscribeToEvent(E_SCENEUPDATE, HANDLER(MasterControl, HandleSceneUpdate));
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
    UI* ui = GetSubsystem<UI>();

    //Create a Cursor UI element because we want to be able to hide and show it at will. When hidden, the mouse cursor will control the camera
    world.cursor.uiCursor = new Cursor(context_);
    world.cursor.uiCursor->SetVisible(true);
    ui->SetCursor(world.cursor.uiCursor);

    //Set starting position of the cursor at the rendering window center
    world.cursor.uiCursor->SetPosition(graphics_->GetWidth()/2, graphics_->GetHeight()/2);

    //Construct new Text object, set string to display and font to use
    Text* instructionText = ui->GetRoot()->CreateChild<Text>();
    instructionText->SetText("Game Of Life Fighter");
    instructionText->SetHorizontalAlignment(HA_CENTER);
    instructionText->SetVerticalAlignment(VA_CENTER);
    instructionText->SetPosition(0, ui->GetRoot()->GetHeight()/2.1);
}

void MasterControl::CreateScene()
{
    world.scene = new Scene(context_);

    //Create octree, use default volume (-1000, -1000, -1000) to (1000,1000,1000)
    world.scene->CreateComponent<Octree>();
    PhysicsWorld* physicsWorld = world.scene->CreateComponent<PhysicsWorld>();
    physicsWorld->SetGravity(Vector3::ZERO);
    world.scene->CreateComponent<DebugRenderer>();

    //Create static lights
    Node* blueLightNode = world.scene->CreateChild("Sun");
    blueLightNode->SetPosition(Vector3(10.0f, 2.0f, 0.0f));
    blueLightNode->LookAt(Vector3::ZERO);
    Light* blueLight = blueLightNode->CreateComponent<Light>();
    blueLight->SetLightType(LIGHT_DIRECTIONAL);
    blueLight->SetBrightness(1.0f);
    blueLight->SetColor(Color(0.1f, 0.5f, 1.0f));
//    blueLight->SetCastShadows(true);

    Node* redLightNode = world.scene->CreateChild("Sun");
    redLightNode->SetPosition(Vector3(-10.0f, 2.0f, 0.0f));
    redLightNode->LookAt(Vector3::ZERO);
    Light* redLight = redLightNode->CreateComponent<Light>();
    redLight->SetLightType(LIGHT_DIRECTIONAL);
    redLight->SetBrightness(1.0f);
    redLight->SetColor(Color(1.0f, 0.5f, 0.1f));
//    redLight->SetCastShadows(true);

    cellMaster_ = new CellMaster(context_, this);

    //Create camera
    world.camera = new GOLFCam(context_, this);
}

Color MasterControl::RandomColor()
{
    float color = Random(3.0f);
    float red = Clamp(color, 0.0f, 1.0f)/100.0f;
    float green = Clamp(color-1.0f, 0.0f, 1.0f)/100.0f;
    float blue = Clamp(color-2.0f, 0.0f, 1.0f)/100.0f;
    return Color(red, green, blue);
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
