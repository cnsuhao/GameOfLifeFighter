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

#ifndef CELL_H
#define CELL_H

#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/AnimatedModel.h>
#include "mastercontrol.h"
#include "cellring.h"

namespace Urho3D {
class Drawable;
class Node;
class Scene;
class Sprite;
class Viewport;
class RenderPath;
class Camera;
}

using namespace Urho3D;

class CellRing;

class Cell : public Object
{
    OBJECT(Cell);
    friend class CellMaster;
    friend class CellRing;
public:
    Cell(Context *context, MasterControl *masterControl, CellRing *cellRing);
    bool IsAlive() { return type_ == golf::CellType::alive; }
private:
    MasterControl* masterControl_;
    float randomizer_;
    golf::CellType type_;
    golf::CellType previousType_;
    SharedPtr<Node> rootNode_;
    SharedPtr<Node> fillNode_ ;
    SharedPtr<StaticModel> cellModel_;
    SharedPtr<AnimatedModel> fillModel_;
    void HandleUpdate(StringHash eventType, VariantMap &eventData);
    void SetType(golf::CellType type);
    float CalculateScale();
};

#endif // CELL_H
