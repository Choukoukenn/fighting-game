#include "Camera.h"
#include <System/Component/ComponentModel.h>
#include <System/Component/ComponentCollisionModel.h>
#include <System/Component/ComponentSpringArm.h>
#include <Game/Game01/Player.h>
#include <Game/Game01/Enemy.h>

namespace Game01 {
bool Camera::Init()
{
    __super::Init();

    // カメラオブジェクト
    //auto camera = Scene::Object::Create<Object>();
    SetName("Camera");
    auto cam = AddComponent<ComponentCamera>();
    cam->SetPositionAndTarget({0.0f, 45.0f, -50.0f}, {0.0f, 20.0f, 0.0f});

    return true;
}

void Camera::Update()
{
    Super::Update();

    auto player = Scene::Object::Get<Object>("Player");
    auto enemy  = Scene::Object::Get<Object>("Enemy");
    if(!player || !enemy)
        return;

    // プレイヤーと敵の位置を取得
    auto pPos = player->GetTranslate();
    auto ePos = enemy->GetTranslate();

    // 両者の中間点（カメラの注視点）
    auto center  = (pPos + ePos) * 0.5f;
    center.y    += 3.0f;

    // プレイヤーと敵の距離
    float dist = length((pPos - ePos));
    dist       = std::clamp(dist * 1.2f, 80.0f, 250.0f);

    // カメラの仰角
    float angle = 30.0f * DegToRad;
    float y     = sin(angle) * dist;
    float z     = -cos(angle) * dist;

    // カメラ位置を計算
    float3 camPos = center + float3(0.0f, y, z);

    auto cam = GetComponent<ComponentCamera>();
    cam->SetPositionAndTarget(camPos, center);
}
}    // namespace Game01
