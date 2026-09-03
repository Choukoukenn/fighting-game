#include "HitEff.h"
#include <System/Component/ComponentCollisionSphere.h>
#include <System/Component/ComponentEffect.h>

namespace Game01 {

bool HitEff::Init()
{
    Super::Init();

    // グランド
    SetName("HitEff");

    return true;
}

void HitEff::Update()
{
    Super::Update();

    count_--;

    if(count_ <= 0)

        Scene::Object::Release(SharedThis());
}

void HitEff::Draw()
{
    Super::Draw();

    VECTOR pos3D = cast(GetTranslate());

    VECTOR pos = ConvWorldPosToScreenPos(pos3D);

    DrawCircle((int)pos.x, (int)pos.y, 300, GetColor(255, 255, 0), 1);
    int a = 0;

    auto p = pos;
    printf("hit: %f %f %f\n", p.x, p.y, p.z);
}

void HitEff::GUI()
{
    Super::GUI();
    ImGui::Begin(GetName().data());
    {
        ImGui::SeparatorText(u8"パラメータ");
    }
    ImGui::End();
}

}    // namespace Game01
