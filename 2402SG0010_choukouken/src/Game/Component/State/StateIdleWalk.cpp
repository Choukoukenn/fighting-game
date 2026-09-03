#pragma once
#include <Game/Component/State/StateIdleWalk.h>
#include <System/Component/ComponentSpringArm.h>
#include <System/Component/ComponentModel.h>
#include "StateAttack1.h"
#include "StateRunAttack.h"
#include "StateGuard.h"
#include <Game/Game01/Player.h>
#include <Game/Game01/Game01.h>
#include <Game/Game01/Bullet.h>
#include <System/Component/ComponentCollisionSphere.h>

using namespace Game01;

void StateIdleWalk::Init()
{
    __super::Init();

    auto owner = GetOwner();

    auto name = owner->GetName();

    if(name == "Player") {
        if(auto mdl = owner->GetComponent<ComponentModel>())
            mdl->PlayAnimationNoSame("idle", true);
    }
}

void StateIdleWalk::Update()
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

    //エイム
    if(ctrl->Aim() && !run_flag_) {
        aim_flag_ = true;
    }
    else {
        aim_flag_ = false;
    }

    int key_dir = 0;    // 0=なし, 1=前, 2=後, 3=左, 4=右

    // 入力方向判定
    if(dir.z < -DEAD)
        key_dir = 1;
    else if(dir.z > DEAD)
        key_dir = 2;
    else if(dir.x < -DEAD)
        key_dir = 3;
    else if(dir.x > DEAD)
        key_dir = 4;

    // 新しい方向入力（前の方向を押したままでもOK）
    bool is_press = (key_dir != 0 && key_dir != prev_key_dir_);

    // まだ走っていない時だけ、二度押しを判定
    if(!run_flag_ && is_press && !aim_flag_) {
        if(key_dir == log_key_ && run_count_ > 0) {
            run_flag_ = true;    // 同じ方向を2回押した → 走る
        }
        else {
            run_flag_  = false;
            run_count_ = RUN_COUNT_MAX;    // 1回目の入力
        }

        log_key_ = key_dir;    // 最後に押した方向を記録
    }

    // 二度押しタイマー減少
    if(run_count_ > 0) {
        run_count_--;
    }

    // 入力がないフレーム数をカウント
    if(key_dir == 0) {
        no_input_count_++;
    }
    else {
        no_input_count_ = 0;
    }

    // 一定時間入力がなければ走り解除
    if(run_flag_ && no_input_count_ > NO_INPUT_GRACE) {
        run_flag_ = false;
    }

    // 前フレームの方向を保存
    prev_key_dir_ = key_dir;

    // 移動キーが押されているか?
    if((float)length(dir) > 0.0f) {
        // 斜めが押されていることを考慮し、
        // その方向の移動スピードを1とし、スピードを掛け合わせる
        dir = normalize(dir);

        if(!aim_flag_) {
            // キャラのローカル方向で移動をさせる
            owner->AddTranslate(dir * move_speed_, true);
        }

        // モデルを移動の方向に向けます
        if(auto mdl = owner->GetComponent<ComponentModel>()) {
            auto rot = quaternion::rotation_axis({0, 1, 0}, front_rot_ * DegToRad);    //< Y軸1度回転

            mdl->SetRotationToVectorWithLimit(mul(dir, rot), rot_speed_);

            if(!run_flag_ && !aim_flag_) {
                mdl->PlayAnimationNoSame("walk", true);
                SetMoveSpeed(0.5f);
            }
            else if(run_flag_) {
                mdl->PlayAnimationNoSame("run", true);
                SetMoveSpeed(1.0f);
            }
            else if(aim_flag_) {
                mdl->PlayAnimationNoSame("aim", true);
            }
        }
    }

    //エイム
    else if(aim_flag_) {
        if(auto mdl = owner->GetComponent<ComponentModel>()) {
            //敵
            //auto enemy = Scene::Object::Get<Object>( "Enemy" );

            auto chars = Scene::Object::GetArray<CharacterBase>();

            float3 player_rot = mdl->GetRotationAxisXYZ();

            float3 player_pos = owner->GetTranslate();

            //内積で目標を探す
            float3 front;
            front.x = 1.0f * sinf(radians(player_rot.y));
            front.y = 0.0f;
            front.z = 1.0 * cosf(radians(player_rot.y));
            front   = normalize(front);

            for(auto& c : chars) {
                //自分を除く
                if(c.get() == owner)
                    continue;

                //敵の位置を習得する
                float3 emy_pos = c->GetTranslate();

                float3 target = emy_pos - player_pos;
                target.y      = 0.0f;
                target        = normalize(target);

                float now_dot = dot(front, target);

                if(now_dot < -0.7f) {
                    //敵の方向を設置する
                    mdl->SetRotationToPosition(emy_pos);

                    break;
                }
            }

            mdl->PlayAnimationNoSame("aim", true);
        }
    }
    else {
        // モデルを移動の方向に向けます
        if(auto mdl = owner->GetComponent<ComponentModel>())
            mdl->PlayAnimationNoSame("idle", true);
    }

    if(bullet_cout < 60) {
        bullet_cout++;
    }

    //弾を出る
    if(aim_flag_ && ctrl->AttackDown() && bullet_cout >= 60) {
        bullet_cout = 0;

        auto   obj = Scene::Object::Create<Bullet>();
        float3 pos = owner->GetTranslate() + float3(0, 10.0f, 0);
        obj->SetTranslate(pos);

        // モデルの前方向を取得
        auto model = owner->GetComponent<ComponentModel>();
        auto dir   = -model->GetWorldMatrix().axisZ();

        obj->SetDirection(dir);

        auto col = obj->GetComponent<ComponentCollisionSphere>();

        if(owner->GetName() == "Player") {
            col->SetHitCollisionGroup((u32)ComponentCollision::CollisionGroup::ENEMY);
        }
        else {
            col->SetHitCollisionGroup((u32)ComponentCollision::CollisionGroup::PLAYER);
        }
    }

    //攻撃
    if(ctrl->AttackDown()) {
        if(aim_flag_) {
            return;
        }

        if(!run_flag_) {
            owner->RemoveComponent(shared_from_this());

            if(!owner->GetComponent<StateAttack1>()) {
                owner->AddComponent<StateAttack1>();
            }
        }
        else if(run_flag_) {
            owner->RemoveComponent(shared_from_this());
            owner->AddComponent<StateRunAttack>();
        }
    }

    //ジャンプ
    if(ctrl->JumpDown()) {
        auto owner = GetOwner();

        auto name = owner->GetName();

        if(auto chr = dynamic_cast<CharacterBase*>(owner)) {
            chr->SetJump(true);
        }

        owner->RemoveComponent(shared_from_this());
        owner->AddComponent<StateJump>();
        owner->GetComponent<StateJump>()->SetJumpMoveSpeed(move_speed_);
        owner->GetComponent<StateJump>()->SetMoveDir(dir);
    }

    //ガード
    if(ctrl->GuardDown()) {
        //ガードモードに切り替え
        if(auto chr = dynamic_cast<CharacterBase*>(owner)) {
            chr->SetGuard(true);
        }

        owner->RemoveComponent(shared_from_this());
        owner->AddComponent<StateGuard>();
    }
}

StateIdleWalkPtr StateIdleWalk::SetMoveSpeed(const float speed)
{
    move_speed_ = speed;
    return std::dynamic_pointer_cast<StateIdleWalk>(shared_from_this());
}
StateIdleWalkPtr StateIdleWalk::SetRotateSpeed(const float speed)
{
    rot_speed_ = speed;
    return std::dynamic_pointer_cast<StateIdleWalk>(shared_from_this());
}

const float StateIdleWalk::GetMoveSpeed() const
{
    return move_speed_;
}

const float StateIdleWalk::GetRotateSpeed() const
{
    return rot_speed_;
}

StateIdleWalkPtr StateIdleWalk::SetKeys(int up, int down, int left, int right, int aim, int att, int def, int jump)
{
    // 移動キーの設定
    key_up_    = up;
    key_down_  = down;
    key_left_  = left;
    key_right_ = right;
    key_aim_   = aim;
    key_att_   = att;
    key_def_   = def;
    key_jump_  = jump;
    return std::dynamic_pointer_cast<StateIdleWalk>(shared_from_this());
}

void StateIdleWalk::GUI()
{
    __super::GUI();

    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();
        if(ImGui::TreeNode(u8"State 移動、停止")) {
            if(ImGui::Button(u8"削除"))
                GetOwner()->RemoveComponent(shared_from_this());

            // 移動の基本情報
            ImGui::DragFloat(u8"移動速度", &move_speed_, 0.1f);
            ImGui::DragFloat(u8"移動回転角度", &rot_speed_, 1.0f);
            ImGui::DragInt(u8"run_count_", &run_count_, 1.0f);
            ImGui::DragInt(u8"log_key", &log_key_);
            ImGui::Checkbox(u8"走る", &run_flag_);
            ImGui::Checkbox(u8"aim", &aim_flag_);
            ImGui::DragFloat(u8"debug", &debug, 1.0f);

            // 移動の基本情報
            ImGui::DragFloat(u8"オブジェクト オフセット回転", &front_rot_, 1.0f);

            ImGui::TreePop();
        }
    }
    ImGui::End();
}

CEREAL_REGISTER_TYPE(StateIdleWalk)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, StateIdleWalk)
