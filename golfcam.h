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

#include "urho3dhelper.h"
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
    OBJECT(GOLFCam);
    friend class MasterControl;
    friend class InputMaster;
public:
    GOLFCam(Context *context, MasterControl* masterControl, golf::PlayerIndex player);

    virtual void Start();
    virtual void Stop();

    SharedPtr<Camera> camera_;
    SharedPtr<Viewport> viewport_;
    SharedPtr<RenderPath> effectRenderPath_;

    SharedPtr<Node> rootNode_;

    Vector3 GetWorldPosition();
    Quaternion GetRotation();
    IntVector2 CenterCoords();
private:
    MasterControl* masterControl_;
    void HandleSceneUpdate(StringHash eventType, VariantMap &eventData);

    SharedPtr<Node> camNode_;

    golf::PlayerIndex player_;
    SharedPtr<RigidBody> rigidBody_;
    Zone* zone_;
    IntVector2 targetCoords_;
    void SetupViewport();
};

#endif // GOLFCAM_H
