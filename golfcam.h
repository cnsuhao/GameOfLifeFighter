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

#ifndef GOLFCAM_H
#define GOLFCAM_H

#include <Urho3D/Urho3D.h>
#include "mastercontrol.h"
#include "cellmaster.h"

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

class GOLFCam : public Object
{
    URHO3D_OBJECT(GOLFCam, Object);
    friend class MasterControl;
    friend class InputMaster;
public:
    GOLFCam(Context *context, MasterControl* masterControl, golf::PlayerIndex player);

    SharedPtr<Camera> camera_;
    SharedPtr<Viewport> viewport_;
    SharedPtr<RenderPath> effectRenderPath_;

    SharedPtr<Node> rootNode_;

    Vector3 GetWorldPosition();
    Quaternion GetRotation();

    void SetTargetRotation(float angle) { targetRotation_ = LucKey::Cycle(angle, 0.0f, 360.0f);}
    IntVector2 CenterCoords();
private:
    MasterControl* masterControl_;
    SharedPtr<Node> camNode_;
    golf::PlayerIndex player_;
    SharedPtr<RigidBody> rigidBody_;
    Zone* zone_;
    float rotation_;
    float targetRotation_;

    void Rotate(float rotation);

    void SetupViewport();
    void HandleSceneUpdate(StringHash eventType, VariantMap &eventData);
};

#endif // GOLFCAM_H
