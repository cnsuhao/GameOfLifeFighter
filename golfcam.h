#pragma once

#include "mastercontrol.h"

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
    GOLFCam(Context *context, MasterControl* masterControl);

    virtual void Start();
    virtual void Stop();

    SharedPtr<Camera> camera_;
    SharedPtr<Viewport> viewport_;
    SharedPtr<RenderPath> effectRenderPath_;

    Vector3 GetWorldPosition();
    Quaternion GetRotation();
private:
    MasterControl* masterControl_;
    void HandleSceneUpdate(StringHash eventType, VariantMap &eventData);
    SharedPtr<Node> rootNode_;
    SharedPtr<Node> camNode_;

    SharedPtr<RigidBody> rigidBody_;
    Zone* zone_;
    double yaw_;
    double pitch_;
    //double roll_ = 0.0;
    double yawDelta_;
    double pitchDelta_;
    double forceMultiplier;
    void SetupViewport();
};
