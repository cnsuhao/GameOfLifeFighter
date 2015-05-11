#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Graphics/Material.h>
#include "cell.h"
#include "cellring.h"

Cell::Cell(Context *context, MasterControl *masterControl, CellRing* cellRing):
    Object(context),
    masterControl_{masterControl}
{
    masterControl_ = masterControl;
    SubscribeToEvent(E_SCENEUPDATE, HANDLER(Cell, HandleSceneUpdate));

    rootNode_ = cellRing ->rootNode_->CreateChild("Cell");
    rootNode_->SetPosition(0.0f, 0.0f, 1.0f);
    rootNode_->Rotate(Quaternion(90.0f, 0.0f, 0.0f));
    rootNode_->SetScale(0.4f);
    model_ = rootNode_->CreateComponent<StaticModel>();
    model_->SetModel(masterControl_->cache_->GetResource<Model>("Resources/Cell.mdl"));
    model_->SetMaterial(masterControl_->cache_->GetTempResource<Material>("Resources/Basic.xml"));
}

void Cell::HandleSceneUpdate(StringHash eventType, VariantMap &eventData)
{
    /*float alpha = Clamp((0.1f*rootNode_->GetPosition().Length())-1.0f, 0.0f, 1.0f);
    model_->GetMaterial(0)->SetShaderParameter("MatDiffColor", Color(1.0f, 1.0f, 1.0f, alpha));*/
}
