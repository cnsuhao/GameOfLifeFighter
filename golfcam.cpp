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

#include "golfcam.h"

GOLFCam::GOLFCam(Context *context, MasterControl *masterControl, golf::PlayerIndex player):
    Object(context),
    player_{player},
    targetRotation_{0.0f-static_cast<float>(player_)*180.0f}
{
    masterControl_ = masterControl;
    SubscribeToEvent(E_SCENEUPDATE, URHO3D_HANDLER(GOLFCam, HandleSceneUpdate));

    float viewRange = 37.0f;

    //Create the camera
    rootNode_ = masterControl_->world_.scene_->CreateChild("CameraPivot");
    camNode_ = rootNode_->CreateChild("CameraNode");
    camera_ = camNode_->CreateComponent<Camera>();
    camera_->SetFarClip(viewRange);
    camera_->SetNearClip(5.0f);
    camera_->SetFov(23.0f);

    //Create a zone with the fog end equal to the viewRange
    zone_ = rootNode_->CreateComponent<Zone>();
    zone_->SetBoundingBox(BoundingBox(Vector3(-100.0f, -50.0f, -100.0f), Vector3(100.0f, 50.0f, 100.0f)));
    zone_->SetFogColor(Color(0.0f, 0.0f, 0.0f, 1.0f));
    zone_->SetFogStart(30.0f);
    zone_->SetFogEnd(viewRange-5.0f);

    rootNode_->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
    camNode_->SetPosition(Vector3(42.0f, 0.0f, 0.0f));
    camNode_->LookAt(Vector3::ZERO);
    rigidBody_ = rootNode_->CreateComponent<RigidBody>();
    rigidBody_->SetMass(1.0f);
    rigidBody_->SetUseGravity(false);

    Light* light = camNode_->CreateChild("LightNode")->CreateComponent<Light>();
    light->SetColor(Color(1.0f, 1.0f, 1.0f));
    light->SetBrightness(0.2f);
    light->SetLightType(LIGHT_POINT);
    light->SetRange(50.0f);
    light->SetCastShadows(false);

    SetupViewport();
}

void GOLFCam::SetupViewport()
{
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    Renderer* renderer = GetSubsystem<Renderer>();

    //Set up a viewport to the Renderer subsystem so that the 3D scene can be seen
    SharedPtr<Viewport> viewport(new Viewport(context_, masterControl_->world_.scene_, camera_));
    viewport_ = viewport;

    //Add anti-asliasing and bloom
    effectRenderPath_ = viewport_->GetRenderPath()->Clone();
    effectRenderPath_->Append(cache->GetResource<XMLFile>("PostProcess/FXAA3.xml"));
    effectRenderPath_->SetEnabled("FXAA3", true);
    effectRenderPath_->Append(cache->GetResource<XMLFile>("PostProcess/Bloom.xml"));
    effectRenderPath_->SetShaderParameter("BloomThreshold", 0.42f);
    effectRenderPath_->SetShaderParameter("BloomMix", Vector2(1.0f, 1.23f));
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

void GOLFCam::Rotate(float angle)
{
    rotation_ += angle;
    rotation_ = LucKey::Cycle(rotation_, 0.0f, 360.0f);
    rootNode_->RotateAround(Vector3::ZERO, Quaternion(angle, Vector3::UP), TS_WORLD);
}

void GOLFCam::HandleSceneUpdate(StringHash eventType, VariantMap &eventData)
{
    float difference = targetRotation_ - rotation_;
    if (abs(difference - 360.0f) < abs(difference)) difference -= 360.0f;
    if (abs(difference + 360.0f) < abs(difference)) difference += 360.0f;

    Rotate(Lerp(0.0f, difference, eventData[SceneUpdate::P_TIMESTEP].GetFloat()* 5.0f));
}

IntVector2 GOLFCam::CenterCoords()
{
    PODVector<RayQueryResult> results{};
    Ray camRay{rootNode_->GetPosition(), rootNode_->GetDirection()};
    if (masterControl_->OctreeRayCast(results, camRay, 16.0f)){
        for (unsigned r = 0; r < results.Size(); ++r) {
            unsigned id = results[r].node_->GetID();
            return masterControl_->cellMaster_->GetCell(id)->GetCoords();
        }
    }
}
