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

#pragma once

#include <Urho3D/Graphics/StaticModel.h>
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

class Cell : public Object
{
    OBJECT(Cell);
    friend class CellMaster;
    friend class CellRing;
public:
    Cell(Context *context, MasterControl *masterControl, CellRing *cellRing);
private:
    MasterControl* masterControl_;
    SharedPtr<Node> rootNode_;
    SharedPtr<StaticModel> model_;
    void HandleSceneUpdate(StringHash eventType, VariantMap &eventData);
};
