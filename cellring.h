#pragma once

#include "cellmaster.h"
#include "mastercontrol.h"

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
    friend class Cell;
public:
    CellRing(Context *context, MasterControl *masterControl, CellMaster *cellMaster, int circumference, int ringNumber);
private:
    MasterControl* masterControl_;
    SharedPtr<Node> rootNode_;
    int ringNumber_;
    void HandleSceneUpdate(StringHash eventType, VariantMap &eventData);
};
