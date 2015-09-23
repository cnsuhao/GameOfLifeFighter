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

#ifndef MASTERCONTROL_H
#define MASTERCONTROL_H

#include <Urho3D/Urho3D.h>

#include "urho3dhelper.h"

namespace Urho3D {
class Drawable;
class Node;
class Scene;
class Sprite;
}

using namespace Urho3D;

class GOLFCam;
class InputMaster;
class CellMaster;

typedef struct GameWorld
{
    HashMap<int, GOLFCam*> cameras_;
    SharedPtr<Scene> scene_;
    SharedPtr<PhysicsWorld> physics_;
    SharedPtr<Octree> octree_;
    SharedPtr<Node> backgroundNode_;
    SharedPtr<Node> voidNode_;
    struct {
        SharedPtr<Node> sceneCursor_;
        SharedPtr<Cursor> uiCursor_;
        PODVector<RayQueryResult> hitResults_;
    } cursor_;
} GameWorld;

typedef struct HitInfo
{
    Vector3 position_;
    Vector3 hitNormal_;
    Node* hitNode_;
    Drawable* drawable_;
} HitInfo;

namespace {
StringHash const N_VOID = StringHash("Void");
StringHash const N_CURSOR = StringHash("Cursor");
StringHash const N_TILEPART = StringHash("TilePart");
StringHash const N_SLOT = StringHash("Slot");
}

class MasterControl : public Application
{
    OBJECT(MasterControl);
    friend class InputMaster;
public:
    MasterControl(Context* context);
    golf::Game* game_;
    GameWorld world_;
    SharedPtr<ResourceCache> cache_;
    SharedPtr<Graphics> graphics_;
    SharedPtr<CellMaster> cellMaster_;
    Renderer* renderer_;

    virtual void Setup();
    virtual void Start();
    virtual void Stop();
    void Exit();
    SharedPtr<Model> CreatePyramid();
    SharedPtr<Model> CreateCube();
    SharedPtr<Model> CreateOctaedron();
    void CreateFromScratchObject(Vector3 position);
    Color RandomColor();
    float GetStepProgress() { return sinceStep_/stepInterval_; }
    int GetNumHumans() const;

    bool OctreeRayCast(PODVector<RayQueryResult> &hitResults, Ray ray, float distance);
private:
    float stepInterval_;
    float sinceStep_;
    SharedPtr<UI> ui_;
    SharedPtr<XMLFile> defaultStyle_;
    HashMap<int, bool> human_;

    void SetWindowTitleAndIcon();
    void CreateConsoleAndDebugHud();

    void CreateScene();
    void CreateUI();
    void SubscribeToEvents();

    void HandleSceneUpdate(StringHash eventType, VariantMap& eventData);
    void HandleUpdate(StringHash eventType, VariantMap& eventData);
    void HandlePostRenderUpdate(StringHash eventType, VariantMap& eventData);

    void CreatePlatform(const Vector3 pos);
    void UpdateCursor(double timeStep);

    bool paused_;
};

#endif // MASTERCONTROL_H
