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
#include "cellmaster.h"


namespace Urho3D {
template <> unsigned MakeHash(const IntVector2& value)
  {
    return GOLF::IntVector2ToHash(value);
  }
}

CellMaster::CellMaster(Context *context, MasterControl *masterControl):
    Object(context),
    masterControl_{masterControl}
{
    rootNode_ = masterControl_->world.scene->CreateChild("CellMaster");
    rootNode_->SetPosition(Vector3::ZERO);

    int circumference = 200;
    for (int i = 0; i < circumference; i++){
                CellRing* newRing = new CellRing(context_, masterControl_, this, 60, i);
                newRing->rootNode_->SetPosition(Vector3(0.0f, 0.0f, 10.0f));
                newRing->rootNode_->RotateAround(rootNode_->GetPosition(), Quaternion(0.0f, 360.0f*i/circumference, 0.0f), TS_PARENT);
                rings_.Push(newRing);
    }

    //For testing purposes
    for (int i = 0; i < 1000; i++)
    {
        cellMap_[IntVector2(Random(200), Random(60))]->rootNode_->SetEnabled(false);
    }
}

void CellMaster::AddCellToMap(IntVector2 coords, Cell* cell)
{
    cellMap_[coords] = SharedPtr<Cell>(cell);
}

void CellMaster::Rotate(float angle)
{
    for (unsigned r = 0; r < rings_.Size(); r++)
        rings_[r]->Rotate(angle);
}
