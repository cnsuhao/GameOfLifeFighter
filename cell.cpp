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

#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Graphics/Material.h>
#include "cell.h"
#include "cellring.h"
#include "golfcam.h"

Cell::Cell(Context *context, MasterControl *masterControl, CellRing* cellRing):
    Object(context),
    masterControl_{masterControl},
    type_{golf::CellType::empty},
    previousType_{type_}
{
    masterControl_ = masterControl;

    rootNode_ = cellRing->rootNode_->CreateChild("Cell");
    rootNode_->SetPosition(Vector3(0.0f, 0.0f, 5.5f));
    rootNode_->SetScale(0.23f);
    fillNode_ = rootNode_->CreateChild("Fill");
    fillNode_->SetScale(0.0f);
    cellModel_ = rootNode_->CreateComponent<StaticModel>();
    cellModel_->SetModel(masterControl_->cache_->GetResource<Model>("Resources/Models/Cell.mdl"));
    cellModel_->SetMaterial(masterControl_->cache_->GetResource<Material>("Resources/Materials/Basic.xml"));
    fillModel_ = fillNode_->CreateComponent<StaticModel>();
    fillModel_->SetModel(masterControl_->cache_->GetResource<Model>("Resources/Models/Fill.mdl"));
    fillModel_->SetMaterial(masterControl_->cache_->GetResource<Material>("Resources/Materials/Fill.xml"));
//    model_->SetCastShadows(true);
    SubscribeToEvent(E_UPDATE, HANDLER(Cell, HandleUpdate));
}

void Cell::HandleUpdate(StringHash eventType, VariantMap &eventData)
{
//    if (previousType_ != type_){
        fillNode_->SetScale(CalculateScale());
//    }
}

void Cell::SetType(golf::CellType type)
{
//    if (type == previousType_ && previousType_ == golf::CellType::empty){
//        fillNode_->SetEnabled(false);
//    }
//    else fillNode_->SetEnabled(true);

    previousType_ = type_;
    type_ = type;
}

float Cell::CalculateScale()
{
    float scale = fillNode_->GetScale().x_;
    float stepProgress = 1.5f*masterControl_->GetStepProgress();
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
