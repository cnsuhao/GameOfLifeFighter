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


#include "cellmaster.h"


namespace Urho3D {
template <> unsigned MakeHash(const IntVector2& value)
  {
    return golf::IntVector2ToHash(value);
  }
}

CellMaster::CellMaster(Context *context, MasterControl *masterControl):
    Object(context),
    masterControl_{masterControl},
    width_{200},
    height_{60}
{
    rootNode_ = masterControl_->world_.scene_->CreateChild("CellMaster");
    rootNode_->SetPosition(Vector3::ZERO);

    for (int i = 0; i < width_; i++){
                CellRing* newRing = new CellRing(context_, masterControl_, this, height_, i);
                newRing->rootNode_->SetPosition(Vector3(0.0f, 0.0f, 10.0f));
                newRing->rootNode_->RotateAround(rootNode_->GetPosition(), Quaternion(0.0f, 360.0f*i/width_, 0.0f), TS_PARENT);
                rings_.Push(newRing);
    }
    Rotate(180.0f);
}

void CellMaster::AddCellToMaps(Cell* cell, IntVector2 coords)
{
    unsigned id = cell->GetID();
    cellsById_[id] = SharedPtr<Cell>(cell);
    cellCoords_[coords] = id;
}

void CellMaster::Rotate(float angle)
{
    for (unsigned r = 0; r < rings_.Size(); r++){
        rings_[r]->SetTargetRotation(angle);
    }
}

void CellMaster::UpdateCells()
{
    Vector<SharedPtr<Cell> > cells{cellsById_.Values()};
    for (unsigned c = 0; c < cells.Size(); c++){
        IntVector2 coords = cells[c]->GetCoords();
        cells[c]->SetType(masterControl_->game_->GetCell(coords.x_, coords.y_));
    }
}

Cell* CellMaster::GetCell(unsigned id)
{
    return cellsById_[id].Get();
}

float CellMaster::RowToRotation(int row)
{
    float normalizedRow = static_cast<float>(row) / static_cast<float>(height_);
    return normalizedRow * 360.0f;
}
float CellMaster::ColumnToRotation(int row)
{
    float normalizedColumn = static_cast<float>(row) / static_cast<float>(width_);
    return normalizedColumn * 360.0f - 90.0f;
}
