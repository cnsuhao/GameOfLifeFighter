/* G.O.L.F.
// Copyright (C) 2015 LucKey Productions (luckeyproductions.nl)
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


#include "inputmaster.h"
#include "golfcam.h"

InputMaster::InputMaster(Context* context, MasterControl* masterControl) : Object(context),
    masterControl_{masterControl},
    input_{GetSubsystem<Input>()}
{
    keyBindings_[KEY_W] = keyBindings_[KEY_UP]    = golf::Key::up1;
    keyBindings_[KEY_D] = keyBindings_[KEY_RIGHT] = golf::Key::left1;
    keyBindings_[KEY_S] = keyBindings_[KEY_DOWN]  = golf::Key::down1;
    keyBindings_[KEY_A] = keyBindings_[KEY_LEFT]  = golf::Key::right1;
    keyBindings_[KEY_C] = keyBindings_[KEY_LSHIFT] = golf::Key::toggle_cell1;
    keyBindings_[KEY_V] = golf::Key::toggle_hangar1;
    keyBindings_[KEY_1] = keyBindings_[KEY_CTRL]   = golf::Key::pattern_a1;
    keyBindings_[KEY_2] = keyBindings_[KEY_ALT]   = golf::Key::pattern_b1;
    keyBindings_[KEY_3] = keyBindings_[KEY_SPACE]   = golf::Key::pattern_c1;

    keyBindings_[KEY_KP_8]    = golf::Key::up2;
    keyBindings_[KEY_KP_6]    = golf::Key::right2;
    keyBindings_[KEY_KP_5]    = golf::Key::down2;
    keyBindings_[KEY_KP_4]    = golf::Key::left2;
    keyBindings_[KEY_RETURN]  = golf::Key::toggle_cell2;
    keyBindings_[KEY_RSHIFT]  = golf::Key::toggle_hangar2;

    SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(InputMaster, HandleKeyDown));
    SubscribeToEvent(E_KEYUP, URHO3D_HANDLER(InputMaster, HandleKeyUp));
    SubscribeToEvent(E_JOYSTICKBUTTONDOWN, URHO3D_HANDLER(InputMaster, HandleJoyButtonDown));
    SubscribeToEvent(E_JOYSTICKBUTTONUP, URHO3D_HANDLER(InputMaster, HandleJoyButtonUp));
    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(InputMaster, HandleUpdate));
}


void InputMaster::HandleUpdate(StringHash eventType, VariantMap &eventData)
{
    //Read joystick input
    ReadJoysticks();

    if (!pressedKeys_.empty()){
        golf::Game* game = masterControl_->game_;
        CellMaster* cellMaster = masterControl_->cellMaster_;
        game->PressKeys(pressedKeys_);

        cellMaster->SetTargetRoll(cellMaster->RowToRotation(game->GetPlayer(golf::PlayerIndex::player1).GetY()));
        masterControl_->world_.cameras_[0]->SetTargetRotation(cellMaster->ColumnToRotation(game->GetPlayer(golf::PlayerIndex::player1).GetX()));
    }
}

void InputMaster::ReadJoysticks()
{
    float deadZone = 0.23f;
    JoystickState* joystickPlayer1 = input_->GetJoystickByIndex(0);
    JoystickState* joystickPlayer2 = input_->GetJoystickByIndex(1);
    if (joystickPlayer1){
        float axis0 = -joystickPlayer1->GetAxisPosition(0);
        if (axis0 > deadZone){
            pressedKeys_.insert(golf::Key::right1);
            pressedKeys_.erase(golf::Key::left1);
        } else if (axis0 < -deadZone){
            pressedKeys_.insert(golf::Key::left1);
            pressedKeys_.erase(golf::Key::right1);
        } else {
            pressedKeys_.erase(golf::Key::right1);
            pressedKeys_.erase(golf::Key::left1);
        }
        float axis1 = -joystickPlayer1->GetAxisPosition(1);
        if (axis1 > deadZone){
            pressedKeys_.insert(golf::Key::up1);
            pressedKeys_.erase(golf::Key::down1);
        } else if (axis1 < -deadZone){
            pressedKeys_.insert(golf::Key::down1);
            pressedKeys_.erase(golf::Key::up1);
        } else {
            pressedKeys_.erase(golf::Key::up1);
            pressedKeys_.erase(golf::Key::down1);
        }
    }
    if (joystickPlayer2){
        float axis0 = -joystickPlayer2->GetAxisPosition(0);
        if (axis0 > deadZone){
            pressedKeys_.insert(golf::Key::right2);
            pressedKeys_.erase(golf::Key::left2);
        } else if (axis0 < -deadZone){
            pressedKeys_.insert(golf::Key::left2);
            pressedKeys_.erase(golf::Key::right2);
        } else {
            pressedKeys_.erase(golf::Key::right2);
            pressedKeys_.erase(golf::Key::left2);
        }
        float axis1 = -joystickPlayer2->GetAxisPosition(1);
        if (axis1 > deadZone){
            pressedKeys_.insert(golf::Key::up2);
            pressedKeys_.erase(golf::Key::down2);
        } else if (axis1 < -deadZone){
            pressedKeys_.insert(golf::Key::down2);
            pressedKeys_.erase(golf::Key::up2);
        } else {
            pressedKeys_.erase(golf::Key::up2);
            pressedKeys_.erase(golf::Key::down2);
        }
    }
}

void InputMaster::HandleKeyDown(StringHash eventType, VariantMap &eventData)
{
    int key = eventData[KeyDown::P_KEY].GetInt();
    if (keyBindings_.Contains(key)) {
        pressedKeys_.insert(keyBindings_[key]);
    }

    switch (key){
    case KEY_ESC:{
        masterControl_->Exit();
    } break;
    case KEY_9:{
        Image screenshot(context_);
        Graphics* graphics = GetSubsystem<Graphics>();
        graphics->TakeScreenShot(screenshot);
        //Here we save in the Data folder with date and time appended
        String fileName = GetSubsystem<FileSystem>()->GetProgramDir() + "Screenshots/Screenshot_" +
                Time::GetTimeStamp().Replaced(':', '_').Replaced('.', '_').Replaced(' ', '_')+".png";
        //Log::Write(1, fileName);
        screenshot.SavePNG(fileName);
    } break;
    default: break;
    }
}

void InputMaster::HandleKeyUp(StringHash eventType, VariantMap &eventData)
{
    int key = eventData[KeyUp::P_KEY].GetInt();
    if (keyBindings_.Contains(key)) pressedKeys_.erase(keyBindings_[key]);
}

void InputMaster::HandleJoyButtonDown(StringHash eventType, VariantMap &eventData)
{
    JoystickButton button = static_cast<JoystickButton>(eventData[JoystickButtonDown::P_BUTTON].GetInt());
}
void InputMaster::HandleJoyButtonUp(StringHash eventType, VariantMap &eventData)
{
    JoystickButton button = static_cast<JoystickButton>(eventData[JoystickButtonUp::P_BUTTON].GetInt());
}
