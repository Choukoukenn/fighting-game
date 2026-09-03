#pragma once
#include <Game/Component/State/StateAttack3.h>
#include <System/Component/ComponentModel.h>
#include <System/Component/ComponentCollisionSphere.h>
#include "StateIdleWalk.h"
#include <Game/Game01/Player.h>

void StateAttack3::Init()
{
    __super::Init();
    auto owner = GetOwner();

    auto name = owner->GetName();

    if(name == "Player") {
        attack_cfg = {0.5f, 0.90f, 0.0f, 0.0f, "mixamorig:RightFoot", "PlayerAttack", ComponentCollision::CollisionGroup::ENEMY};
    }

    if(name == "Enemy") {
        attack_cfg = {0.3f, 1.0f, 0.0f, 0.0f, "mixamorig:RightFoot", "NpcAttack", ComponentCollision::CollisionGroup::PLAYER};
    }

    if(auto model = owner->GetComponent<ComponentModel>()) {
        model->PlayAnimation("att3");
    }
}

void StateAttack3::Update()
{
    __super::Update();

    // オーナー(自分がAddComponentされたObject)を取得します
    // 処理されるときは必ずOwnerは存在しますので基本的にnullptrチェックは必要ありません
    auto owner = GetOwner();

    if(auto model = owner->GetComponent<ComponentModel>()) {
        float time = model->GetAnimationTime();
        if(time > attack_cfg.hit_time_start && time < attack_cfg.hit_time_end && !collision_used_) {
            auto chr = dynamic_cast<Game01::CharacterBase*>(owner);

            bool hit_check = chr->GetHit();
            //打たれた時中断
            if(hit_check) {
                owner->RemoveComponent(shared_from_this());

                if(auto col = collsion_.lock())
                    owner->RemoveComponent(col);

                return;
            }

            if(collsion_.expired()) {
                auto col = owner->AddComponent<ComponentCollisionSphere>();
                col->AttachToModel(attack_cfg.bone_name);
                col->SetRadius(4.0f);
                col->SetName(attack_cfg.collision_name);
                col->SetHitCollisionGroup((u32)attack_cfg.hit_group);

                collsion_ = col;
            }
        }
        else {
            owner->RemoveComponent(collsion_.lock());
        }
    }

    if(auto model = owner->GetComponent<ComponentModel>()) {
        float time = model->GetAnimationTime();

        if(!model->IsPlaying()) {
            if(!owner->GetComponent<StateIdleWalk>()) {
                owner->AddComponent<StateIdleWalk>();
            }
            RemoveThisComponent();
        }
    }
}

void StateAttack3::GUI()
{
    __super::GUI();

    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();
        if(ImGui::TreeNode(u8"State Attack3***")) {
            ImGui::TreePop();
        }
    }
    ImGui::End();
}

CEREAL_REGISTER_TYPE(StateAttack3)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, StateAttack3)
