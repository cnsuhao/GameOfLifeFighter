#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/SceneEvents.h>
#include "cellmaster.h"
#include "cellring.h"
#include "cell.h"

CellMaster::CellMaster(Context *context, MasterControl *masterControl):
    Object(context),
    masterControl_{masterControl}
{
    masterControl_ = masterControl;
    SubscribeToEvent(E_SCENEUPDATE, HANDLER(CellMaster, HandleSceneUpdate));

    rootNode_ = masterControl_->world.scene->CreateChild("CellMaster");
    rootNode_->SetPosition(Vector3::ZERO);

    int circumference = 200;
    for (int i = 0; i < circumference; i++){
                CellRing* newRing = new CellRing(context_, masterControl_, this, 60);
                newRing->rootNode_->SetPosition(0.0f, 0.0f, 10.0f);
                newRing->rootNode_->RotateAround(rootNode_->GetPosition(), Quaternion(0.0f, 360.0f*i/circumference, 0.0f), TS_PARENT);
    }
}

void CellMaster::HandleSceneUpdate(StringHash eventType, VariantMap &eventData)
{

}
