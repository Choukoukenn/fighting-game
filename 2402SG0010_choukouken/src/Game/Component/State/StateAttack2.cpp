#pragma once
#include <Game/Component/State/StateAttack2.h>
#include <System/Component/ComponentModel.h>
#include <System/Component/ComponentCollisionSphere.h>
#include "StateIdleWalk.h"
#include "StateAttack3.h"
#include <Game/Game01/Player.h>

using namespace Game01;

void StateAttack2::Init()
{
	__super::Init();

	auto owner = GetOwner();

	auto name = owner->GetName();

	if( name == "Player" )
	{
		attack_cfg = { 0.0f, 0.75f, 0.75f, 1.5f, "mixamorig:LeftFoot", "PlayerAttack", ComponentCollision::CollisionGroup::ENEMY };
	}

	if( name == "Enemy" )
	{
		attack_cfg = { 0.2f, 0.6f, 0.4f, 2.0f, "mixamorig:RightHand", "NpcAttack", ComponentCollision::CollisionGroup::PLAYER };
	}

	if( auto model = owner->GetComponent<ComponentModel>() )
	{
		model->PlayAnimation( "att2" );
	}
}

void StateAttack2::Update()
{
	__super::Update();

	// オーナー(自分がAddComponentされたObject)を取得します
	// 処理されるときは必ずOwnerは存在しますので基本的にnullptrチェックは必要ありません
	auto owner = GetOwner();

	auto ch = dynamic_cast<CharacterBase*>( owner );
	if( !ch || !ch->controller_ )
		return;

	auto& ctrl = ch->controller_;

	if( auto model = owner->GetComponent<ComponentModel>() )
	{
		float time = model->GetAnimationTime();
		if( time > attack_cfg.hit_time_start && time < attack_cfg.hit_time_end && !collision_used_ )
		{
			auto owner = GetOwner();

			auto chr = dynamic_cast<CharacterBase*>( owner );

			bool hit_check = chr->GetHit();
			//打たれた時中断
			if( hit_check )
			{
				owner->RemoveComponent( shared_from_this() );

				if( auto col = left_collsion_.lock() )
					owner->RemoveComponent( col );

				return;
			}

			if( left_collsion_.expired() )
			{
				auto col = owner->AddComponent<ComponentCollisionSphere>();
				col->AttachToModel( attack_cfg.bone_name );
				col->SetRadius( 4.0f );
				col->SetName( attack_cfg.collision_name );
				col->SetHitCollisionGroup( (u32)attack_cfg.hit_group );

				left_collsion_ = col;
			}
		}
		else
		{
			if( auto left_collsion = left_collsion_.lock() )
			{
				owner->RemoveComponent( left_collsion );
			}
		}

		if( !model->IsPlaying() )
		{
			if( !owner->GetComponent<StateIdleWalk>() )
			{
				owner->AddComponent<StateIdleWalk>();
			}
			RemoveThisComponent();
		}

		if( ctrl->AttackDown() )
		{
			float start_time = attack_cfg.next_time_start;
			float end_time	 = attack_cfg.next_time_end;

			float time = model->GetAnimationTime();
			if( time >= start_time && time <= end_time )
			{
				owner->RemoveComponent<ComponentCollisionSphere>();
				owner->RemoveComponent( shared_from_this() );
				owner->AddComponent<StateAttack3>();
			}
		}
	}
}

void StateAttack2::GUI()
{
	__super::GUI();

	// GUI内に出現させる
	ImGui::Begin( GetOwner()->GetName().data() );
	{
		ImGui::Separator();
		if( ImGui::TreeNode( u8"State Attack2**" ) )
		{
			ImGui::TreePop();
		}
	}
	ImGui::End();
}

CEREAL_REGISTER_TYPE( StateAttack2 )
CEREAL_REGISTER_POLYMORPHIC_RELATION( Component, StateAttack2 )
