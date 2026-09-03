#include "Bullet.h"
#include <System/Component/ComponentCollisionSphere.h>
#include <System/Component/ComponentEffect.h>

namespace Game01 {
float Bullet::speed_  = 3.0f;
float Bullet::radius_ = 2.0f;

bool Bullet::Init()
{
    Super::Init();

    // グランド
    SetName("Bullet");

    auto collision = AddComponent<ComponentCollisionSphere>();
    collision->SetRadius(radius_);

    return true;
}

void Bullet::Update()
{
    Super::Update();

    auto pos = GetTranslate() + (direction_ * speed_);
    SetTranslate(pos);
    AddTranslate(direction_ * speed_);

    if(auto collision = GetComponent<ComponentCollisionSphere>())
        collision->SetRadius(radius_);
    if(auto collision = collision_.lock()) {
        collision->SetRadius(radius_);
    }
}

void Bullet::Draw()
{
    auto pos   = cast(GetTranslate());
    auto scale = GetScaleAxisXYZ();
    auto color = GetColor(255, 255, 255);
    DrawSphere3D(pos, radius_, 20, color, color, TRUE);
}

void Bullet::GUI()
{
    Super::GUI();
    ImGui::Begin(GetName().data());
    {
        ImGui::SeparatorText(u8"パラメータ");
        ImGui::DragFloat("Shot Speed", &speed_, 0.01f, 0.1f, 10.0f, "%.2f");
        ImGui::DragFloat("Shot Radius", &radius_, 0.01f, 0.1f, 10.0f, "%.2f");
    }
    ImGui::End();
}

void Bullet::OnHit(const ComponentCollision::HitInfo& hit_info)
{
    Super::OnHit(hit_info);

    //auto hit_object = hit_info.hit_collision_->GetOwner();
    //printfDx( "HIT: %s\n", hit_object->GetNameDefault().data() );

    //const std::string eff	 = "data/Sample/Effects/Laser01.efkefc";
    //const float3	  pos	 = hit_info.hit_position_;
    //auto			  effect = ComponentEffect::Object::Create( eff, pos );

    Scene::Object::Release(SharedThis());
}

void Bullet::SetDirection(float3 dir)
{
    direction_ = dir;
}

}    // namespace Game01
