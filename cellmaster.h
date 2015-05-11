#pragma once

#include "mastercontrol.h"

namespace Urho3D {
class Drawable;
class Node;
class Scene;
class Camera;
}

using namespace Urho3D;

class CellMaster : public Object
{
    friend class CellRing;
    OBJECT(CellMaster);
public:
    CellMaster(Context *context, MasterControl *masterControl);
private:
    MasterControl* masterControl_;
    SharedPtr<Node> rootNode_;
    void HandleSceneUpdate(StringHash eventType, VariantMap &eventData);
};
