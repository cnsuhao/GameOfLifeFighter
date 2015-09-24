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

#ifndef CELLRING_H
#define CELLRING_H

#include "cellmaster.h"

namespace Urho3D {
class Drawable;
class Node;
class Scene;
class Camera;
}

using namespace Urho3D;

class CellRing : public Object
{
    OBJECT(CellRing);
    friend class CellMaster;
    friend class InputMaster;
    friend class Cell;
public:
    CellRing(Context *context, MasterControl *masterControl, CellMaster *cellMaster, int circumference, int ringNumber);
private:
    MasterControl* masterControl_;
    SharedPtr<Node> rootNode_;
    int ringNumber_;
    float rotation_;
    float targetRotation_;
    void Rotate(float rotation);
    void SetTargetRotation(float angle);
    void HandleUpdate(StringHash eventType, VariantMap &eventData);
};

#endif // CELLRING_H
