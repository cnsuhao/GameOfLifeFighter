//
// Copyright (c) 2008-2015 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

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
    paused_(false)
{
}


void MasterControl::Setup()
{
    // Modify engine startup parameters.
    //Set custom window title and icon.
    engineParameters_["WindowTitle"] = "G.O.L.F.";
    engineParameters_["LogName"] = GetSubsystem<FileSystem>()->GetAppPreferencesDir("urho3d", "logs")+"golf.log";
    engineParameters_["FullScreen"] = true;
    engineParameters_["Headless"] = false;
    engineParameters_["WindowWidth"] = 1920;
    engineParameters_["WindowHeight"] = 1080;
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

    /*Sound* music = cache_->GetResource<Sound>("Resources/Music/Macroform_-_Root.ogg");
    music->SetLooped(true);
    Node* musicNode = world.scene->CreateChild("Music");
    SoundSource* musicSource = musicNode->CreateComponent<SoundSource>();
    musicSource->SetSoundType(SOUND_MUSIC);
    musicSource->Play(music);*/
}
void MasterControl::Stop()
{
    engine_->DumpResources(true);
}

void MasterControl::SubscribeToEvents()
{
    //Subscribe HandleUpdate() function for processing update events
    SubscribeToEvent(E_UPDATE, HANDLER(MasterControl, HandleUpdate));
    //Subscribe scene update event.
    SubscribeToEvent(E_SCENEUPDATE, HANDLER(MasterControl, HandleSceneUpdate));
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
    world.cursor.uiCursor->SetVisible(false);
    ui->SetCursor(world.cursor.uiCursor);

    //Set starting position of the cursor at the rendering window center
    world.cursor.uiCursor->SetPosition(graphics_->GetWidth()/2, graphics_->GetHeight()/2);

    //Construct new Text object, set string to display and font to use
    Text* instructionText = ui->GetRoot()->CreateChild<Text>();
    instructionText->SetText("Game Of Life Fighter");
    //instructionText->SetFont(cache->GetResource<Font>("Resources/Fonts/Riau.ttf"), 32);
    //The text has multiple rows. Center them in relation to each other
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
    blueLightNode->SetPosition(10.0f, 2.0f, 0.0f);
    blueLightNode->LookAt(Vector3::ZERO);
    Light* blueLight = blueLightNode->CreateComponent<Light>();
    blueLight->SetLightType(LIGHT_DIRECTIONAL);
    blueLight->SetBrightness(0.5f);
    blueLight->SetRange(23.0f);
    blueLight->SetColor(Color(0.1f, 0.5f, 1.0f));
    blueLight->SetCastShadows(false);

    Node* redLightNode = world.scene->CreateChild("Sun");
    redLightNode->SetPosition(-10.0f, 2.0f, 0.0f);
    redLightNode->LookAt(Vector3::ZERO);
    Light* redLight = redLightNode->CreateComponent<Light>();
    redLight->SetLightType(LIGHT_DIRECTIONAL);
    redLight->SetBrightness(0.5f);
    redLight->SetRange(23.0f);
    redLight->SetColor(Color(1.0f, 0.5f, 0.1f));
    redLight->SetCastShadows(false);

    // Finally create one model (pyramid shape) and a StaticModel to display it from scratch
        // Note: there are duplicated vertices to enable face normals. We will calculate normals programmatically
    int width = 12;
    int height = 50;
    float offset = 1.0f;

    /*for (int i = 0; i < width; i++)    {
        for (int j = 0; j < height/2; j++)    {
                //if ((i < width/2.5 || i > width - width/2.5) || (k < depth/2.5 || k > depth - depth/2.5))
                CreateFromScratchObject(Vector3((i * offset) - (offset*width/2.0f)+Random(-0.5f, 0.5f),
                                   (j * offset) - (offset*height/2.0f)+Random(-0.5f, 0.5f),
                                   0.0f));
        }
    }*/
    //CreateFromScratchObject(Vector3(0.0f,2.0f,0.0f));

    //CreateFromScratchObject(Vector3::ZERO);

    new CellMaster(context_, this);

    //Set cascade splits at 10, 50, 200 world unitys, fade shadows at 80% of maximum shadow distance
    //light->SetShadowCascade(CascadeParameters(7.0f, 23.0f, 42.0f, 500.0f, 0.8f));

    //Create camera
    world.camera = new GOLFCam(context_, this);
}

void MasterControl::CreateFromScratchObject(Vector3 position)
{

    SharedPtr<Model> fromScratchModel = CreateCube();
    /*int shapeRandomizer = Random(2);
    switch (shapeRandomizer){
    case 0: fromScratchModel = CreateOctaedron(); break;
    case 1: fromScratchModel = CreateCube(); break;
    case 2: fromScratchModel = CreatePyramid(); break;
    default: break;
    }*/
    SharedPtr<ValueAnimation> diffColorAnimation(new ValueAnimation(context_));
    Color firstAndLastcolor = Color(Random(0.05f), Random(0.05f), Random(0.05f), Random(0.1f, 0.2f));
    float speed = 1.0f;
    diffColorAnimation->SetKeyFrame(0.0f, firstAndLastcolor);
    diffColorAnimation->SetKeyFrame(0.25f/speed, RandomColor());
    diffColorAnimation->SetKeyFrame(0.5f/speed, RandomColor());
    diffColorAnimation->SetKeyFrame(0.75/speed, RandomColor());
    diffColorAnimation->SetKeyFrame(1.0f/speed, firstAndLastcolor);

    Quaternion rotation(Random(5.0f), Random(3) * 60.0f, Random(5.0f));
    Vector3 scale(Random(0.25f)+0.5f,Random(0.25f)+0.5f,Random(0.25f)+0.5f);

    Node* nodeOne = world.scene->CreateChild("FromScratchObject");
    nodeOne->SetPosition(position);
    nodeOne->SetRotation(rotation);
    nodeOne->SetScale(scale);
    StaticModel* staticModelOne = nodeOne->CreateComponent<StaticModel>();
    staticModelOne->SetModel(fromScratchModel);

    staticModelOne->SetMaterial(cache_->GetTempResource<Material>("Resources/Materials/Basic.xml"));

    // Optionally associate material with scene to make sure shader parameter animation respects scene time scale
    staticModelOne->GetMaterial(0)->SetScene(world.scene);
    staticModelOne->GetMaterial(0)->SetShaderParameterAnimation("MatDiffColor", diffColorAnimation);
}

SharedPtr<Model> MasterControl::CreatePyramid()
{
    const unsigned numVertices = 18;

    float vertexData[] = {
        // Position             Normal
        0.0f, 0.5f, 0.0f,       0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f,      0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,     0.0f, 0.0f, 0.0f,

        0.0f, 0.5f, 0.0f,       0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f,     0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f,      0.0f, 0.0f, 0.0f,

        0.0f, 0.5f, 0.0f,       0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f,     0.0f, 0.0f, 0.0f,

        0.0f, 0.5f, 0.0f,       0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,     0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 0.0f,

        0.5f, -0.5f, -0.5f,     0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f,      0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f,     0.0f, 0.0f, 0.0f,

        0.5f, -0.5f, -0.5f,     0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f,     0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 0.0f
    };

    const unsigned short indexData[] = {
        0, 1, 2,
        3, 4, 5,
        6, 7, 8,
        9, 10, 11,
        12, 13, 14,
        15, 16, 17
    };

    // Calculate face normals now
    for (unsigned i = 0; i < numVertices; i += 3)
    {
        Vector3& v1 = *(reinterpret_cast<Vector3*>(&vertexData[6 * i]));
        Vector3& v2 = *(reinterpret_cast<Vector3*>(&vertexData[6 * (i + 1)]));
        Vector3& v3 = *(reinterpret_cast<Vector3*>(&vertexData[6 * (i + 2)]));
        Vector3& n1 = *(reinterpret_cast<Vector3*>(&vertexData[6 * i + 3]));
        Vector3& n2 = *(reinterpret_cast<Vector3*>(&vertexData[6 * (i + 1) + 3]));
        Vector3& n3 = *(reinterpret_cast<Vector3*>(&vertexData[6 * (i + 2) + 3]));

        Vector3 edge1 = v1 - v2;
        Vector3 edge2 = v1 - v3;
        n1 = n2 = n3 = edge1.CrossProduct(edge2).Normalized();
    }

    SharedPtr<Model> fromScratchModel(new Model(context_));
    SharedPtr<VertexBuffer> vb(new VertexBuffer(context_));
    SharedPtr<IndexBuffer> ib(new IndexBuffer(context_));
    SharedPtr<Geometry> geom(new Geometry(context_));

    // Shadowed buffer needed for raycasts to work, and so that data can be automatically restored on device loss
    vb->SetShadowed(false);
    vb->SetSize(numVertices, MASK_POSITION|MASK_NORMAL);
    vb->SetData(vertexData);

    ib->SetShadowed(false);
    ib->SetSize(numVertices, false);
    ib->SetData(indexData);

    geom->SetVertexBuffer(0, vb);
    geom->SetIndexBuffer(ib);
    geom->SetDrawRange(TRIANGLE_LIST, 0, numVertices);

    fromScratchModel->SetNumGeometries(1);
    fromScratchModel->SetGeometry(0, 0, geom);
    fromScratchModel->SetBoundingBox(BoundingBox(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f)));

    return fromScratchModel;
}

SharedPtr<Model> MasterControl::CreateCube()
{
    const unsigned numVertices = 36;

    float vertexData[] = {
        // Position             Normal
        //Bottom plane
        0.5f, -0.5f, -0.5f,     0.0f, 0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,     0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,     0.0f, 0.0f, 0.0f,

        0.5f, -0.5f, -0.5f,     0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,     0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,     0.0f, 0.0f, 0.0f,

        //Top plane
        0.5f,  0.5f,  0.5f,     0.0f, 0.0f, 0.0f,
        0.5f,  0.5f, -0.5f,     0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,     0.0f, 0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,     0.0f, 0.0f, 0.0f,
        0.5f,  0.5f, -0.5f,     0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,     0.0f, 0.0f, 0.0f,

        //Front plane
        0.5f, -0.5f, -0.5f,     0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,     0.0f, 0.0f, 0.0f,
        0.5f,  0.5f, -0.5f,     0.0f, 0.0f, 0.0f,

        -0.5f,  0.5f, -0.5f,     0.0f, 0.0f, 0.0f,
        0.5f,  0.5f, -0.5f,     0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,     0.0f, 0.0f, 0.0f,

        //Back plane
        -0.5f, -0.5f,  0.5f,     0.0f, 0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,     0.0f, 0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,     0.0f, 0.0f, 0.0f,

        0.5f,  0.5f,  0.5f,     0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,     0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,     0.0f, 0.0f, 0.0f,

        //Left plane
        -0.5f, -0.5f, -0.5f,     0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,     0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,     0.0f, 0.0f, 0.0f,

        -0.5f,  0.5f, -0.5f,     0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,     0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,     0.0f, 0.0f, 0.0f,

        //Right plane
        0.5f, -0.5f,  0.5f,     0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,     0.0f, 0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,     0.0f, 0.0f, 0.0f,

        0.5f, -0.5f, -0.5f,     0.0f, 0.0f, 0.0f,
        0.5f,  0.5f, -0.5f,     0.0f, 0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,     0.0f, 0.0f, 0.0f
    };

    const unsigned short indexData[] = {
        0, 1, 2,
        3, 4, 5,
        6, 7, 8,
        9, 10, 11,
        12, 13, 14,
        15, 16, 17,
        18, 19, 20,
        21, 22, 23,
        24, 25, 26,
        27, 28, 29,
        30, 31, 32,
        33, 34, 35
    };

    // Calculate face normals now
    for (unsigned i = 0; i < numVertices; i += 3)
    {
        Vector3& v1 = *(reinterpret_cast<Vector3*>(&vertexData[6 * i]));
        Vector3& v2 = *(reinterpret_cast<Vector3*>(&vertexData[6 * (i + 1)]));
        Vector3& v3 = *(reinterpret_cast<Vector3*>(&vertexData[6 * (i + 2)]));
        Vector3& n1 = *(reinterpret_cast<Vector3*>(&vertexData[6 * i + 3]));
        Vector3& n2 = *(reinterpret_cast<Vector3*>(&vertexData[6 * (i + 1) + 3]));
        Vector3& n3 = *(reinterpret_cast<Vector3*>(&vertexData[6 * (i + 2) + 3]));

        Vector3 edge1 = v1 - v2;
        Vector3 edge2 = v1 - v3;
        n1 = n2 = n3 = edge1.CrossProduct(edge2).Normalized();
    }

    SharedPtr<Model> fromScratchModel(new Model(context_));
    SharedPtr<VertexBuffer> vb(new VertexBuffer(context_));
    SharedPtr<IndexBuffer> ib(new IndexBuffer(context_));
    SharedPtr<Geometry> geom(new Geometry(context_));

    // Shadowed buffer needed for raycasts to work, and so that data can be automatically restored on device loss
    vb->SetShadowed(false);
    vb->SetSize(numVertices, MASK_POSITION|MASK_NORMAL);
    vb->SetData(vertexData);

    ib->SetShadowed(false);
    ib->SetSize(numVertices, false);
    ib->SetData(indexData);

    geom->SetVertexBuffer(0, vb);
    geom->SetIndexBuffer(ib);
    geom->SetDrawRange(TRIANGLE_LIST, 0, numVertices);

    fromScratchModel->SetNumGeometries(1);
    fromScratchModel->SetGeometry(0, 0, geom);
    fromScratchModel->SetBoundingBox(BoundingBox(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f)));

    return fromScratchModel;
}

SharedPtr<Model> MasterControl::CreateOctaedron()
{
    const unsigned numVertices = 24;

    float vertexData[] = {
        // Position             Normal
        //Bottom
        -0.5f,  0.0f, -0.5f,     0.0f, 0.0f, 0.0f,
        0.5f,  0.0f, -0.5f,     0.0f, 0.0f, 0.0f,
        0.0f, -1.0f,  0.0f,     0.0f, 0.0f, 0.0f,

        0.5f,  0.0f,  0.5f,     0.0f, 0.0f, 0.0f,
        -0.5f,  0.0f,  0.5f,     0.0f, 0.0f, 0.0f,
        0.0f, -1.0f,  0.0f,     0.0f, 0.0f, 0.0f,

        0.5f,  0.0f, -0.5f,     0.0f, 0.0f, 0.0f,
        0.5f,  0.0f,  0.5f,     0.0f, 0.0f, 0.0f,
        0.0f, -1.0f,  0.0f,     0.0f, 0.0f, 0.0f,

        -0.5f,  0.0f,  0.5f,     0.0f, 0.0f, 0.0f,
        -0.5f,  0.0f, -0.5f,     0.0f, 0.0f, 0.0f,
        0.0f, -1.0f,  0.0f,     0.0f, 0.0f, 0.0f,

        //Top
        0.5f,  0.0f, -0.5f,     0.0f, 0.0f, 0.0f,
        -0.5f,  0.0f, -0.5f,     0.0f, 0.0f, 0.0f,
        0.0f,  1.0f,  0.0f,     0.0f, 0.0f, 0.0f,

        -0.5f,  0.0f,  0.5f,     0.0f, 0.0f, 0.0f,
        0.5f,  0.0f,  0.5f,     0.0f, 0.0f, 0.0f,
        0.0f,  1.0f,  0.0f,     0.0f, 0.0f, 0.0f,

        0.5f,  0.0f,  0.5f,     0.0f, 0.0f, 0.0f,
        0.5f,  0.0f, -0.5f,     0.0f, 0.0f, 0.0f,
        0.0f,  1.0f,  0.0f,     0.0f, 0.0f, 0.0f,

        -0.5f,  0.0f, -0.5f,     0.0f, 0.0f, 0.0f,
        -0.5f,  0.0f,  0.5f,     0.0f, 0.0f, 0.0f,
        0.0f,  1.0f,  0.0f,     0.0f, 0.0f, 0.0f
    };

    const unsigned short indexData[] = {
        0, 1, 2,
        3, 4, 5,
        6, 7, 8,
        9, 10, 11,
        12, 13, 14,
        15, 16, 17,
        18, 19, 20,
        21, 22, 23,
    };

    // Calculate face normals now
    for (unsigned i = 0; i < numVertices; i += 3)
    {
        Vector3& v1 = *(reinterpret_cast<Vector3*>(&vertexData[6 * i]));
        Vector3& v2 = *(reinterpret_cast<Vector3*>(&vertexData[6 * (i + 1)]));
        Vector3& v3 = *(reinterpret_cast<Vector3*>(&vertexData[6 * (i + 2)]));
        Vector3& n1 = *(reinterpret_cast<Vector3*>(&vertexData[6 * i + 3]));
        Vector3& n2 = *(reinterpret_cast<Vector3*>(&vertexData[6 * (i + 1) + 3]));
        Vector3& n3 = *(reinterpret_cast<Vector3*>(&vertexData[6 * (i + 2) + 3]));

        Vector3 edge1 = v1 - v2;
        Vector3 edge2 = v1 - v3;
        n1 = n2 = n3 = edge1.CrossProduct(edge2).Normalized();
    }

    SharedPtr<Model> fromScratchModel(new Model(context_));
    SharedPtr<VertexBuffer> vb(new VertexBuffer(context_));
    SharedPtr<IndexBuffer> ib(new IndexBuffer(context_));
    SharedPtr<Geometry> geom(new Geometry(context_));

    // Shadowed buffer needed for raycasts to work, and so that data can be automatically restored on device loss
    vb->SetShadowed(false);
    vb->SetSize(numVertices, MASK_POSITION|MASK_NORMAL);
    vb->SetData(vertexData);

    ib->SetShadowed(false);
    ib->SetSize(numVertices, false);
    ib->SetData(indexData);

    geom->SetVertexBuffer(0, vb);
    geom->SetIndexBuffer(ib);
    geom->SetDrawRange(TRIANGLE_LIST, 0, numVertices);

    fromScratchModel->SetNumGeometries(1);
    fromScratchModel->SetGeometry(0, 0, geom);
    fromScratchModel->SetBoundingBox(BoundingBox(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f)));

    return fromScratchModel;
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

}

void MasterControl::HandleSceneUpdate(StringHash eventType, VariantMap &eventData)
{
    /*using namespace Update;
    double timeStep = eventData[P_TIMESTEP].GetFloat();*/
}

void MasterControl::Exit()
{
    engine_->Exit();
}

void MasterControl::HandlePostRenderUpdate(StringHash eventType, VariantMap &eventData)
{
    //world.scene->GetComponent<PhysicsWorld>()->DrawDebugGeometry(true);
}





































