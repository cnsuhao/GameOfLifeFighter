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
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Graphics/Material.h>
#include "cell.h"
#include "cellring.h"
#include "golfcam.h"

Cell::Cell(Context *context, MasterControl *masterControl, CellRing* cellRing):
    Object(context),
    masterControl_{masterControl}
{
    masterControl_ = masterControl;

    rootNode_ = cellRing->rootNode_->CreateChild("Cell");
    rootNode_->SetPosition(Vector3(0.0f, 0.0f, 5.5f));
    rootNode_->Rotate(Quaternion(-90.0f, 0.0f, 0.0f));
    rootNode_->SetScale(0.23f);
    model_ = rootNode_->CreateComponent<StaticModel>();
    model_->SetModel(masterControl_->cache_->GetResource<Model>("Resources/Cell.mdl"));
    model_->SetMaterial(masterControl_->cache_->GetResource<Material>("Resources/Basic.xml"));
}
