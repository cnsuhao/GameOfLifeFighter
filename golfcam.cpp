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
    targetCoords_{IntVector2{50, 30}}
{
    masterControl_ = masterControl;
    SubscribeToEvent(E_SCENEUPDATE, HANDLER(GOLFCam, HandleSceneUpdate));

    float viewRange = 32.0f;

    //Create the camera. Limit far clip distance to match the fog
    rootNode_ = masterControl_->world_.scene_->CreateChild("CameraPivot");
    camNode_ = rootNode_->CreateChild("CameraNode");
    camera_ = camNode_->CreateComponent<Camera>();
    camera_->SetFarClip(viewRange);
    camera_->SetNearClip(23.0f);
    camera_->SetFov(23.0f);

    zone_ = rootNode_->CreateComponent<Zone>();
    zone_->SetBoundingBox(BoundingBox(Vector3(-100.0f, -50.0f, -100.0f), Vector3(100.0f, 50.0f, 100.0f)));
    zone_->SetFogColor(Color(0.0f, 0.0f, 0.0f, 1.0f));
    zone_->SetFogStart(30.0f);
    zone_->SetFogEnd(viewRange);

    rootNode_->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
    camNode_->SetPosition(Vector3(42.0f, 0.0f, 0.0f));
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
    light->SetCastShadows(false);

    SetupViewport();
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
    SharedPtr<Viewport> viewport(new Viewport(context_, masterControl_->world_.scene_, camera_));
//    viewport_ = viewport;

//    //Add anti-asliasing and bloom
//    effectRenderPath_ = viewport_->GetRenderPath()->Clone();
//    effectRenderPath_->Append(cache->GetResource<XMLFile>("PostProcess/FXAA3.xml"));
//    effectRenderPath_->SetEnabled("FXAA3", true);
//    effectRenderPath_->Append(cache->GetResource<XMLFile>("PostProcess/Bloom.xml"));
//    effectRenderPath_->SetShaderParameter("BloomThreshold", 0.5f);
//    effectRenderPath_->SetEnabled("Bloom", true);
//    viewport_->SetRenderPath(effectRenderPath_);
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
    rootNode_->SetRotation(Quaternion(Lerp(rootNode_->GetRotation().EulerAngles().y_, masterControl_->cellMaster_->ColumnToRotation(
                                          masterControl_->game_->GetPlayer(golf::PlayerIndex::player1).GetX()), timeStep*5.0f),
                                      Vector3::UP));
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
