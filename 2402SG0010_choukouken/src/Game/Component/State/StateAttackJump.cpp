#pragma once
#include <Game/Component/State/StateAttackJump.h>
#include <System/Component/ComponentModel.h>
#include <System/Component/ComponentCollisionSphere.h>
#include <System/Component/ComponentCollisionCapsule.h>
#include "StateJump.h"
#include "StateIdleWalk.h"
#include <Game/Game01/Player.h>
using namespace Game01;

void StateAttackJump::Init()
{
    __super::Init();
    auto owner = GetOwner();

    auto name = owner->GetName();

    if(name == "Player") {
        attack_cfg = {0.0f, 0.75f, 0.0f, 0.0f, "mixamorig:LeftFoot", "PlayerAttack", ComponentCollision::CollisionGroup::ENEMY};
    }

    if(name == "Enemy") {
        attack_cfg = {0.0f, 0.75f, 0.0f, 0.0f, "mixamorig:LeftFoot", "NpcAttack", ComponentCollision::CollisionGroup::PLAYER};
    }

    if(auto model = owner->GetComponent<ComponentModel>()) {
        model->PlayAnimation("jumpatt", false);
    }
}

void StateAttackJump::Update()
{
    __super::Update();

    // オーナー(自分がAddComponentされたObject)を取得します
    // 処理されるときは必ずOwnerは存在しますので基本的にnullptrチェックは必要ありません
    auto owner = GetOwner();

    if(auto model = owner->GetComponent<ComponentModel>()) {
        float3 pos = owner->GetTranslate();
        float3 dir = -model->GetWorldMatrix().axisZ();

        dir = normalize(dir);

        pos += dir;
        owner->SetTranslate(pos);

        float time           = model->GetAnimationTime();
        float hit_time_start = attack_cfg.hit_time_start;
        float hit_time_end   = attack_cfg.hit_time_end;

        if(time > hit_time_start && time < hit_time_end) {
            if(collsion_.expired() && !collision_used_) {
                auto col = owner->AddComponent<ComponentCollisionSphere>();
                col->AttachToModel(attack_cfg.bone_name);
                col->SetRadius(4.0f);
                col->SetName(attack_cfg.collision_name);
                col->SetHitCollisionGroup((u32)attack_cfg.hit_group);
                collsion_ = col;
            }
        }
        else {
            if(auto collsion = collsion_.lock()) {
                owner->RemoveComponent(collsion);
                collsion_.reset();
            }
        }
    }

    //重力を消す
    if(auto col = owner->GetComponent<ComponentCollisionCapsule>()) {
        //col->UseGravity( false );
    }

    //ジャンプモードを消す
    if(auto chr = dynamic_cast<CharacterBase*>(owner)) {
        //chr->SetJump( false );
    }

    if(auto model = owner->GetComponent<ComponentModel>()) {
        //アクションが終わったら
        if(model->GetPlayAnimationName() != "jumpatt") {
            //owner->AddComponent<StateIdleWalk>();
            owner->AddComponent<StateJump>();
            owner->GetComponent<StateJump>()->SetFlag(true);

            if(auto collsion = collsion_.lock()) {
                owner->RemoveComponent(collsion);
                collsion_.reset();
            }

            if(auto col = owner->GetComponent<ComponentCollisionCapsule>()) {
                col->UseGravity(true);

                if(auto chr = dynamic_cast<CharacterBase*>(owner)) {
                    chr->SetJump(true);
                }
            }

            RemoveThisComponent();
        }
    }
}

void StateAttackJump::GUI()
{
    __super::GUI();

    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();
        if(ImGui::TreeNode(u8"State jumpatt***")) {
            ImGui::TreePop();
        }
    }
    ImGui::End();
}

void StateAttackJump::OnRemove()
{
    if(auto collsion = collsion_.lock()) {
        GetOwner()->RemoveComponent(collsion);
        collsion_.reset();
    }

    collision_used_ = false;
}

CEREAL_REGISTER_TYPE(StateAttackJump)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, StateAttackJump)
