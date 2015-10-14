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

#include "cell.h"
#include "cellring.h"
#include "golfcam.h"

Cell::Cell(Context *context, MasterControl *masterControl, CellRing* cellRing, IntVector2 coords):
    Object(context),
    masterControl_{masterControl},
    coords_{coords},
    type_{golf::CellType::empty},
    previousType_{type_},
    randomizer_{Random()},
    heart_{false}
{
    masterControl_ = masterControl;
    heart_ = masterControl_->game_->IsHeart(coords_.x_, coords_.y_);

    rootNode_ = cellRing->rootNode_->CreateChild("Cell");
    rootNode_->SetPosition(Vector3(0.0f, 0.0f, 5.5f));
    rootNode_->SetScale(0.23f);
    fillNode_ = rootNode_->CreateChild("Life");
    fillNode_->SetScale(0.0f);
    cellModel_ = rootNode_->CreateComponent<AnimatedModel>();
    cellModel_->SetModel(heart_
                         ? masterControl_->cache_->GetResource<Model>("Resources/Models/Heart.mdl")
                         : masterControl_->cache_->GetResource<Model>("Resources/Models/Cell.mdl"));
    cellModel_->SetMaterial(masterControl_->cache_->GetResource<Material>("Resources/Materials/Basic.xml"));
    fillModel_ = fillNode_->CreateComponent<AnimatedModel>();
    fillModel_->SetModel(masterControl_->cache_->GetResource<Model>("Resources/Models/Life.mdl"));
    fillModel_->SetMaterial(masterControl_->cache_->GetResource<Material>("Resources/Materials/Life.xml"));
    SubscribeToEvent(E_UPDATE, HANDLER(Cell, HandleUpdate));

//    rootNode_->CreateComponent<RigidBody>();
//    CollisionShape* collider = rootNode_->CreateComponent<CollisionShape>();
//    collider->SetBox(Vector3(2.01f, 2.33f, 0.611f));
}

void Cell::HandleUpdate(StringHash eventType, VariantMap &eventData)
{
    float newScale = CalculateScale();
    if (newScale <= 0.23f){
        if (fillNode_->IsEnabled()) {
            fillNode_->SetEnabled(false);
            return;
        }
        else return;
    }
    //else if (!fillNode_->IsEnabled()) fillNode_->SetEnabled(true);
    fillNode_->SetScale(newScale);

    if (fillNode_->IsEnabled()){
        fillNode_->Rotate(Quaternion(eventData[Update::P_TIMESTEP].GetFloat() * (randomizer_-0.23f) * 42.0f, Vector3::FORWARD));
        float toCycle = (3.0f+2.0f*randomizer_)*masterControl_->world_.scene_->GetElapsedTime() + randomizer_*2.0f;
        float morphWeight1 = LucKey::Cycle(toCycle, 0.0f, 2.0f);
        float morphWeight2 = LucKey::Cycle(toCycle*1.23f+randomizer_, 0.0f, 2.0f);
        morphWeight1 = (morphWeight1 + fillModel_->GetMorphWeight(0))*0.5f;
        morphWeight1 = morphWeight1 > 1.0f ? 1.0f-(morphWeight1-1.0f) : morphWeight1;
        fillModel_->SetMorphWeight(0, morphWeight1);
        morphWeight2 = (morphWeight2 + fillModel_->GetMorphWeight(1))*0.5f;
        morphWeight2 = morphWeight2 > 1.0f ? 1.0f-(morphWeight2-1.0f) : morphWeight2;
        fillModel_->SetMorphWeight(1, morphWeight2);
    }
}

void Cell::SetType(golf::CellType type)
{
    previousType_ = type_;
    type_ = type;
    if (type_ == golf::CellType::alive) {
        fillNode_->SetEnabled(true);
        if (heart_) cellModel_->SetMorphWeight(0, 0.0f);
    } else if (heart_) cellModel_->SetMorphWeight(0, 1.0f);
}

float Cell::CalculateScale()
{
    float scale = fillNode_->GetScale().x_;
    float stepProgress = 2.0f*masterControl_->GetStepProgress();
    switch (type_){
    case golf::CellType::empty: {
        scale = Lerp(scale, 0.0f, stepProgress);
    } break;
    case golf::CellType::alive: {
        scale = Lerp(scale, 1.0f, stepProgress);
    } break;
    default: break;
    }
    return Clamp(scale, 0.0f, 1.0f);
}
