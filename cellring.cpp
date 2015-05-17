#include <Urho3D/Urho3D.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Input/Input.h>
#include "cellmaster.h"
#include "cellring.h"
#include "cell.h"

CellRing::CellRing(Context *context, MasterControl *masterControl, CellMaster* cellMaster, int circumference, int ringNumber):
    Object(context),
    masterControl_{masterControl},
    ringNumber_{ringNumber}
{
    masterControl_ = masterControl;
    SubscribeToEvent(E_SCENEUPDATE, HANDLER(CellRing, HandleSceneUpdate));

    rootNode_ = cellMaster->rootNode_->CreateChild("CellRing");
    rootNode_->SetPosition(Vector3::FORWARD*10.0f);

    for (int i = 0; i < circumference; i++){
                Cell* newCell = new Cell(context_, masterControl_, this);
                newCell->rootNode_->RotateAround(rootNode_->GetPosition(), Quaternion(
                                                     360.0f*i/circumference, 0.0f, 0.0f), TS_WORLD);
                cellMaster->AddCellToMap(IntVector2(ringNumber_, i), newCell);
    }
}

void CellRing::HandleSceneUpdate(StringHash eventType, VariantMap &eventData)
{
    using namespace SceneUpdate;
    float timeStep = eventData[P_TIMESTEP].GetFloat();
    Input* input = GetSubsystem<Input>();
    if (input->GetKeyDown('W')) rootNode_->Rotate(Quaternion(-timeStep*(20.0f+20.0f*input->GetKeyDown(KEY_SHIFT)), Vector3::LEFT), TS_LOCAL);
    if (input->GetKeyDown('S')) rootNode_->Rotate(Quaternion( timeStep*(20.0f+20.0f*input->GetKeyDown(KEY_SHIFT)), Vector3::LEFT), TS_LOCAL);
}
