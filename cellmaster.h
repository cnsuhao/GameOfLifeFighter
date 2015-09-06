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

#ifndef CELLMASTER_H
#define CELLMASTER_H

#include "mastercontrol.h"
#include "cellmaster.h"
#include "cellring.h"
#include "cell.h"
#include "helper.h"

namespace Urho3D {
class Drawable;
class Node;
class Scene;
class Camera;
}

using namespace Urho3D;

class Cell;
class CellRing;

class CellMaster : public Object
{
    friend class CellRing;
    OBJECT(CellMaster);
public:
    CellMaster(Context *context, MasterControl *masterControl);
    void AddCellToMap(IntVector2 coords, Cell *cell);
    void Rotate(float angle);
private:
    MasterControl* masterControl_;
    SharedPtr<Node> rootNode_;

    Vector<CellRing*> rings_;
    HashMap<IntVector2, SharedPtr<Cell> > cellMap_;
};

#endif // CELLMASTER_H
