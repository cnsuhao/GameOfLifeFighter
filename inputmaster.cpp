#include <Urho3D/Urho3D.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/OctreeQuery.h>
#include <Urho3D/IO/FileSystem.h>

#include "inputmaster.h"
#include "golfcam.h"

InputMaster::InputMaster(Context* context, MasterControl* masterControl) : Object(context),
    masterControl_{masterControl},
    input_{GetSubsystem<Input>()}
{
    //Subscribe mouse down event
    SubscribeToEvent(E_SCENEUPDATE, HANDLER(InputMaster, HandleSceneUpdate));
    //Subscribe key down event.
    SubscribeToEvent(E_KEYDOWN, HANDLER(InputMaster, HandleKeyDown));
    //Subscribe mouse down event
    SubscribeToEvent(E_MOUSEBUTTONDOWN, HANDLER(InputMaster, HandleMouseDown));
    //Subscribe mouse up event
    SubscribeToEvent(E_MOUSEBUTTONUP, HANDLER(InputMaster, HandleMouseUp));
}

void InputMaster::HandleSceneUpdate(StringHash eventType, VariantMap &eventData)
{
    using namespace SceneUpdate;
    float timeStep = eventData[P_TIMESTEP].GetFloat();

    Input* input = GetSubsystem<Input>();
    //Rotate camera left and right
    if (input->GetKeyDown('D')) masterControl_->world.camera->rootNode_->Rotate(Quaternion(0.0f,-timeStep*(23.0f+64.0f*input->GetKeyDown(KEY_SHIFT)), 0.0f));
    if (input->GetKeyDown('A')) masterControl_->world.camera->rootNode_->Rotate(Quaternion(0.0f, timeStep*(23.0f+64.0f*input->GetKeyDown(KEY_SHIFT)), 0.0f));
    //Zoom in and out
    if (input->GetKeyDown('Q')) masterControl_->world.camera->camNode_->Translate(0.0f, 0.0f, timeStep*(23.0f+32.0f*input->GetKeyDown(KEY_SHIFT)));
    if (input->GetKeyDown('E')) masterControl_->world.camera->camNode_->Translate(0.0f, 0.0f, -timeStep*(23.0f+32.0f*input->GetKeyDown(KEY_SHIFT)));

    //Rotate camera left and right
    /*if (input->GetKeyDown('D')) masterControl_->world.camera->rigidBody_->ApplyTorque(-timeStep*(420.0f+640.0f*input->GetKeyDown(KEY_SHIFT))*Vector3::UP);
    if (input->GetKeyDown('A')) masterControl_->world.camera->rigidBody_->ApplyTorque( timeStep*(420.0f+640.0f*input->GetKeyDown(KEY_SHIFT))*Vector3::UP);*/
    //Zoom in and out
    //if (input->GetKeyDown('W')) masterControl_->world.camera->camNode_->Translate(0.0f, 0.0f, timeStep*(23.0f+32.0f*input->GetKeyDown(KEY_SHIFT)));
    //if (input->GetKeyDown('S')) masterControl_->world.camera->camNode_->Translate(0.0f, 0.0f, -timeStep*(23.0f+32.0f*input->GetKeyDown(KEY_SHIFT)));
}

void InputMaster::HandleMouseDown(StringHash eventType, VariantMap &eventData)
{
    /*using namespace MouseButtonDown;
    int button = eventData[P_BUTTON].GetInt();*/
}

void InputMaster::HandleMouseUp(StringHash eventType, VariantMap &eventData)
{
    /*using namespace MouseButtonUp;
    int button = eventData[P_BUTTON].GetInt();*/
}

void InputMaster::HandleKeyDown(StringHash eventType, VariantMap &eventData)
{
    using namespace KeyDown;
    int key = eventData[P_KEY].GetInt();

    //Exit when ESC is pressed
    if (key == KEY_ESC) masterControl_->Exit();
    //Take screenshot
    if (key == KEY_9){
        Graphics* graphics = GetSubsystem<Graphics>();
        Image screenshot(context_);
        graphics->TakeScreenShot(screenshot);
        //Here we save in the Data folder with date and time appended
        String fileName = GetSubsystem<FileSystem>()->GetProgramDir() + "Screenshots/Screenshot_" +
                Time::GetTimeStamp().Replaced(':', '_').Replaced('.', '_').Replaced(' ', '_')+".png";
        //Log::Write(1, fileName);
        screenshot.SavePNG(fileName);
    }
}

