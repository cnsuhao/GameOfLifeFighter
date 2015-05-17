#include <Urho3D/Urho3D.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Viewport.h>
#include <Urho3D/Graphics/RenderPath.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Graphics/Zone.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/XMLFile.h>
#include <Urho3D/Math/MathDefs.h>
#include <Urho3D/Input/Input.h>

#include "golfcam.h"

GOLFCam::GOLFCam(Context *context, MasterControl *masterControl):
    Object(context),
    yaw_{0.0},
    pitch_{0.0},
    yawDelta_{0.0},
    pitchDelta_{0.0},
    forceMultiplier{1.0}
{
    masterControl_ = masterControl;
    SubscribeToEvent(E_SCENEUPDATE, HANDLER(GOLFCam, HandleSceneUpdate));

    float viewRange = 12.0f;

    //Create the camera. Limit far clip distance to match the fog
    rootNode_ = masterControl_->world.scene->CreateChild("CameraTarget");
    camNode_ = rootNode_->CreateChild("CameraNode");
    camera_ = camNode_->CreateComponent<Camera>();
    camera_->SetFarClip(viewRange);
    camera_->SetNearClip(0.0f);
    //Set an initial position for the camera scene node above the origin
    rootNode_->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
    camNode_->SetPosition(Vector3(0.0f, 0.0f, -23.0f));
    camNode_->LookAt(Vector3::ZERO);
    rigidBody_ = rootNode_->CreateComponent<RigidBody>();
    //rigidBody_->SetAngularDamping(0.0f);
    //rigidBody_->SetLinearDamping(0.0f);
    rigidBody_->SetMass(1.0f);
    rigidBody_->SetUseGravity(false);

    Light* light = camNode_->CreateChild("LightNode")->CreateComponent<Light>();
    light->SetColor(Color(1.0f, 1.0f, 1.0f));
    light->SetBrightness(0.2f);
    light->SetLightType(LIGHT_POINT);
    light->SetRange(50.0f);

    SetupViewport();

    zone_ = rootNode_->CreateComponent<Zone>();
    zone_->SetBoundingBox(BoundingBox(Vector3(-100.0f, -50.0f, -100.0f), Vector3(100.0f, 50.0f, 100.0f)));
    zone_->SetFogColor(Color(0.0f, 0.0f, 0.0f, 1.0f));
    zone_->SetFogStart(10.0f);
    zone_->SetFogEnd(viewRange);
}



void GOLFCam::Start()
{
}

void GOLFCam::Stop()
{
}

void GOLFCam::SetupViewport()
{
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    Renderer* renderer = GetSubsystem<Renderer>();

    //Set up a viewport to the Renderer subsystem so that the 3D scene can be seen
    SharedPtr<Viewport> viewport(new Viewport(context_, masterControl_->world.scene, camera_));
    viewport_ = viewport;

    //Add anti-asliasing and bloom
    effectRenderPath_ = viewport_->GetRenderPath()->Clone();
    effectRenderPath_->Append(cache->GetResource<XMLFile>("PostProcess/FXAA3.xml"));
    effectRenderPath_->SetEnabled("FXAA3", true);
    effectRenderPath_->Append(cache->GetResource<XMLFile>("PostProcess/Bloom.xml"));
    effectRenderPath_->SetShaderParameter("BloomThreshold", 0.5f);
    effectRenderPath_->SetEnabled("Bloom", true);    
    viewport_->SetRenderPath(effectRenderPath_);
    renderer->SetViewport(0, viewport);
}

Vector3 GOLFCam::GetWorldPosition()
{
    return rootNode_->GetWorldPosition();
}

Quaternion GOLFCam::GetRotation()
{
    return camNode_->GetRotation();
}

void GOLFCam::HandleSceneUpdate(StringHash eventType, VariantMap &eventData)
{
    using namespace Update;

    //Take the frame time step, which is stored as a double
    double timeStep = eventData[P_TIMESTEP].GetFloat();

    rigidBody_->ApplyTorque(Vector3::UP*100.0f);
}

