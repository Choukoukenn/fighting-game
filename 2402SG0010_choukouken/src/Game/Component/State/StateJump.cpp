#pragma once
#include <Game/Component/State/StateJump.h>
#include <System/Component/ComponentModel.h>
#include <System/Component/ComponentCollisionSphere.h>
#include <Game/Game01/Player.h>
#include "StateIdleWalk.h"
#include "StateAttackJump.h"

using namespace Game01;

void StateJump::Init()
{
    __super::Init();
    auto owner = GetOwner();
    if(auto model = owner->GetComponent<ComponentModel>()) {
        model->PlayAnimation("floating", true);
    }
}

void StateJump::Update()
{
    __super::Update();

    // オーナー(自分がAddComponentされたObject)を取得します
    // 処理されるときは必ずOwnerは存在しますので基本的にnullptrチェックは必要ありません
    auto owner = GetOwner();

    auto ch = dynamic_cast<CharacterBase*>(owner);
    if(!ch || !ch->controller_)
        return;

    auto& ctrl = ch->controller_;

    // 移動方向
    float x = ctrl->MoveX();    // 左右
    float z = ctrl->MoveZ();    // 前後

    float3 dir{x, 0, z};

    // 移動キーが押されているか?
    if((float)length(move_dir_) > 0.0f) {
        // 固定移動方向で前進
        owner->AddTranslate(normalize(move_dir_) * move_speed_, true);
    }

    // 面は入力方向に向ける
    if((float)length(dir) > 0.0f) {
        if(auto mdl = owner->GetComponent<ComponentModel>()) {
            float3 flat_dir = normalize(float3{dir.x, 0, dir.z});
            mdl->SetRotationToVectorWithLimit(flat_dir, rot_speed_);
        }
    }

    if(ctrl->AttackDown() && !attack_) {
        RemoveThisComponent();
        owner->AddComponent<StateAttackJump>();
    }
}

void StateJump::GUI()
{
    __super::GUI();

    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();
        if(ImGui::TreeNode(u8"State Jump***")) {
            ImGui::TreePop();
        }
    }
    ImGui::End();
}

StateJumpPtr StateJump::SetJumpMoveSpeed(const float speed)
{
    move_speed_ = speed;
    return std::dynamic_pointer_cast<StateJump>(shared_from_this());
}

CEREAL_REGISTER_TYPE(StateJump)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, StateJump)
