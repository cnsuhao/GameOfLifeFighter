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
