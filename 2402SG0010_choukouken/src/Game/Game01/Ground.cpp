#include "Ground.h"
#include <System/Component/ComponentModel.h>
#include <System/Component/ComponentCollisionModel.h>

namespace Game01 {
bool Ground::Init()
{
    Super::Init();

    // グランド
    //auto ground = Scene::Object::Create<Object>();
    SetName("Ground");
    AddComponent<ComponentModel>("data/Sample/SwordBout/Stage/Stage00.mv1");

    AddComponent<ComponentCollisionModel>()->SetCollisionGroup(ComponentCollision::CollisionGroup::GROUND)->AttachToModel();

    return true;
}

void Ground::Update()
{
    Super::Update();
}
}    // namespace Game01
