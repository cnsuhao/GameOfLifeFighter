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
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Input/Input.h>
#include "cellring.h"
#include "cell.h"

CellRing::CellRing(Context *context, MasterControl *masterControl, CellMaster* cellMaster, int circumference, int ringNumber):
    Object(context),
    masterControl_{masterControl},
    ringNumber_{ringNumber}
{
    masterControl_ = masterControl;

    rootNode_ = cellMaster->rootNode_->CreateChild("CellRing");
    rootNode_->SetPosition(Vector3::FORWARD*10.0f);

    for (int i = 0; i < circumference; i++){
        Cell* newCell = new Cell(context_, masterControl_, this);
        newCell->rootNode_->RotateAround(rootNode_->GetPosition(), Quaternion(
                                             360.0f*i/circumference, 0.0f, 0.0f), TS_WORLD);
        cellMaster->AddCellToMap(IntVector2(ringNumber_, i), newCell);
    }
}

void CellRing::Rotate(float angle)
{
    rootNode_->Rotate(Quaternion(angle, Vector3::LEFT), TS_LOCAL);
}
