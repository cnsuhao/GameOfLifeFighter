#pragma once

#include "mastercontrol.h"

namespace Urho3D {
class Drawable;
class Node;
class Scene;
class Camera;
}

using namespace Urho3D;

class Cell;

class CellMaster : public Object
{
    friend class CellRing;
    OBJECT(CellMaster);
public:
    CellMaster(Context *context, MasterControl *masterControl);
    void AddCellToMap(IntVector2 coords, Cell *cell);
private:
    MasterControl* masterControl_;
    SharedPtr<Node> rootNode_;

    HashMap<IntVector2, SharedPtr<Cell> > cellMap_;
    void HandleSceneUpdate(StringHash eventType, VariantMap &eventData);
};
