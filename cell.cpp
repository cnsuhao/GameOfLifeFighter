#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Graphics/Material.h>
#include "cell.h"
#include "cellring.h"
#include "golfcam.h"

Cell::Cell(Context *context, MasterControl *masterControl, CellRing* cellRing):
    Object(context),
    masterControl_{masterControl}
{
    masterControl_ = masterControl;
    SubscribeToEvent(E_SCENEUPDATE, HANDLER(Cell, HandleSceneUpdate));

    rootNode_ = cellRing ->rootNode_->CreateChild("Cell");
    rootNode_->SetPosition(0.0f, 0.0f, 5.5f);
    rootNode_->Rotate(Quaternion(-90.0f, 0.0f, 0.0f));
    rootNode_->SetScale(0.23f);
    model_ = rootNode_->CreateComponent<StaticModel>();
    model_->SetModel(masterControl_->cache_->GetResource<Model>("Resources/Cell.mdl"));
    model_->SetMaterial(masterControl_->cache_->GetResource<Material>("Resources/Basic.xml"));
}

void Cell::HandleSceneUpdate(StringHash eventType, VariantMap &eventData)
{
    float cameraAngle = rootNode_->GetWorldDirection().DotProduct(rootNode_->GetWorldPosition() - masterControl_->world.camera->rootNode_->GetWorldPosition());
    bool facingCamera = cameraAngle > -8.0f && cameraAngle < 8.0f;
    if (!facingCamera) rootNode_->SetEnabledRecursive(false);
    else rootNode_->SetEnabledRecursive(true);
}
