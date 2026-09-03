#pragma once
#include "StateCounter.h"
#include "StateAttack2.h"
#include "StateIdleWalk.h"
#include <Game/Game01/Player.h>
#include <System/Component/ComponentModel.h>
#include <System/Component/ComponentCollisionSphere.h>
#include "StateAttackbase.h"

void StateCounter::Init()
{
    __super::Init();
    auto owner = GetOwner();

    auto name = owner->GetName();

    if(name == "Player") {
        attack_cfg = {0.2f, 0.80f, 0.0f, 0.0f, "mixamorig:RightForeArm", "PlayerAttack", ComponentCollision::CollisionGroup::ENEMY};
    }

    if(name == "Enemy") {
        attack_cfg = {0.5f, 1.0f, 0.0f, 0.0f, "mixamorig:RightHand", "NpcAttack", ComponentCollision::CollisionGroup::PLAYER};
    }

    if(auto model = owner->GetComponent<ComponentModel>()) {
        model->PlayAnimation("counter");
    }
}

void StateCounter::Update()
{
    __super::Update();

    // オーナー(自分がAddComponentされたObject)を取得します
    // 処理されるときは必ずOwnerは存在しますので基本的にnullptrチェックは必要ありません
    auto owner = GetOwner();

    if(auto model = owner->GetComponent<ComponentModel>()) {
        float           time           = model->GetAnimationTime();
        constexpr float hit_time_start = 0.20f;
        constexpr float hit_time_end   = 0.80f;

        if(time > attack_cfg.hit_time_start && time < attack_cfg.hit_time_end && !collision_used_) {
            auto owner = GetOwner();

            auto name = owner->GetName();

            if(right_collsion_.expired()) {
                auto col = owner->AddComponent<ComponentCollisionSphere>();
                col->AttachToModel(attack_cfg.bone_name);
                col->SetRadius(4.0f);
                col->SetName(attack_cfg.collision_name);
                col->SetHitCollisionGroup((u32)attack_cfg.hit_group);

                right_collsion_ = col;
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
            //ガードモードをオフにする
            if(auto player = dynamic_cast<Game01::Player*>(owner)) {
                player->SetGuard(false);
            }

            RemoveThisComponent();

            if(!owner->GetComponent<StateIdleWalk>()) {
                owner->AddComponent<StateIdleWalk>();
            }
        }
    }
}

void StateCounter::GUI()
{
    __super::GUI();

    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();
        if(ImGui::TreeNode(u8"State Counter*")) {
            ImGui::TreePop();
        }
    }
    ImGui::End();
}

CEREAL_REGISTER_TYPE(StateCounter)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, StateCounter)
