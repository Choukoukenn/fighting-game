#pragma once
#include "nStateAttack1.h"
#include <System/Component/ComponentModel.h>
#include <System/Component/ComponentCollisionSphere.h>

void nStateAttack1::Init()
{
    __super::Init();
    auto owner = GetOwner();

    if(auto model = owner->GetComponent<ComponentModel>()) {
        model->PlayAnimation("natt1");
    }
}

void nStateAttack1::Update()
{
    __super::Update();

    // オーナー(自分がAddComponentされたObject)を取得します
    // 処理されるときは必ずOwnerは存在しますので基本的にnullptrチェックは必要ありません
    auto owner = GetOwner();

    if(auto model = owner->GetComponent<ComponentModel>()) {
        float           time           = model->GetAnimationTime();
        constexpr float hit_time_start = 1.20f;
        constexpr float hit_time_end   = 1.70f;

        //時間内しか出ないコリジョン
        if(time >= hit_time_start && time < hit_time_end) {
            if(right_collsion_.expired()) {
                auto col2 = owner->AddComponent<ComponentCollisionSphere>();
                col2->AttachToModel("mixamorig:RightHand");
                col2->SetRadius(3.0f);
                col2->SetName("NpcAttack");
                col2->SetMass(10000.0f);

                //コリジョングループ
                col2->SetHitCollisionGroup((u32)ComponentCollision::CollisionGroup::PLAYER);
                right_collsion_ = col2;
            }
        }
        //時間外ならコリジョンを消す
        else {
            if(auto right_collsion = right_collsion_.lock()) {
                owner->RemoveComponent(right_collsion);
            }
        }

        //アクションが終わったら
        if(!model->IsPlaying()) {
            RemoveThisComponent();

            if(auto right_collsion = right_collsion_.lock()) {
                owner->RemoveComponent(right_collsion);
            }

            owner->GetComponent<ComponentModel>()->PlayAnimation("walk", true);
        }

        //次のモーションに進む
    }
}

void nStateAttack1::GUI()
{
    __super::GUI();

    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();
        if(ImGui::TreeNode(u8"State Attack1*")) {
            ImGui::TreePop();
        }
    }
    ImGui::End();
}

CEREAL_REGISTER_TYPE(nStateAttack1)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, nStateAttack1)
