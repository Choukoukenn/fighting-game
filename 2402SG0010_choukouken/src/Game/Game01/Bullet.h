#pragma once
#include <System/Scene.h>

namespace Game01 {
USING_PTR(Bullet);

class Bullet : public Object
{
public:
    BP_OBJECT_DECL(Bullet, u8"Game01 の Bullet");

    bool Init() override;

    void Update() override;

    void Draw() override;

    void GUI() override;

    void OnHit(const ComponentCollision::HitInfo& hit_info) override;

    //! @brief 弾の方向を設定
    //! @param dir 方向
    void SetDirection(float3 dir);

private:
    float3       direction_ = {1, 0, 0};
    static float speed_;
    static float radius_;

    ComponentCollisionSphereWeakPtr collision_;
};

}    // namespace Game01
