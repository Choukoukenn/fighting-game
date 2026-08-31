#pragma once
#include <Game/Component/State/StateRunAttack.h>
#include <System/Component/ComponentModel.h>
#include <System/Component/ComponentCollisionSphere.h>
#include <Game/Game01/Player.h>
#include "StateIdleWalk.h"

void StateRunAttack::Init()
{
	__super::Init();
	auto owner = GetOwner();

	auto name = owner->GetName();

	if( name == "Enemy" )
	{
		attack_cfg = { 0.2f, 0.8f, 0.0f, 0.0f, "mixamorig:LeftFoot", "NpcAttack", ComponentCollision::CollisionGroup::PLAYER };
	}

	if( auto model = owner->GetComponent<ComponentModel>() )
	{
		model->PlayAnimation( "runAtt" );
	}
}

void StateRunAttack::Update()
{
	__super::Update();

	// オーナー(自分がAddComponentされたObject)を取得します
	// 処理されるときは必ずOwnerは存在しますので基本的にnullptrチェックは必要ありません
	auto owner = GetOwner();

	if( auto model = owner->GetComponent<ComponentModel>() )
	{
		auto name = owner->GetName();

		float time = model->GetAnimationTime();

		auto chr = dynamic_cast<Game01::CharacterBase*>( owner );

		bool hit_check = chr->GetHit();
		//打たれた時中断
		if( hit_check )
		{
			owner->RemoveComponent( shared_from_this() );

			if( auto col = left_collsion_.lock() )
				owner->RemoveComponent( col );

			if( auto col = right_collsion_.lock() )
				owner->RemoveComponent( col );

			return;
		}

		if( name == "Player" )
		{
			constexpr float hit_time_start = 0.30f;
			constexpr float hit_time_end   = 0.56f;

			if( time >= hit_time_start && time < hit_time_end )
			{
				if( right_collsion_.expired() && !collision_used_r )
				{
					auto col = owner->AddComponent<ComponentCollisionSphere>();
					col->AttachToModel( "mixamorig:RightLeg" );
					col->SetRadius( 2.0f );
					col->SetName( "PlayerAttack_Right" );
					col->SetHitCollisionGroup( (u32)ComponentCollision::CollisionGroup::ENEMY );
					col->SetMass( 10000.0f );
					right_collsion_ = col;
				}
			}
			else
			{
				owner->RemoveComponent( right_collsion_.lock() );
			}

			//二番めのコリジョン
			time							= model->GetAnimationTime();
			constexpr float hit_time_start2 = 0.60f;
			constexpr float hit_time_end2	= 0.75f;

			if( time >= hit_time_start2 && time < hit_time_end2 )
			{
				if( left_collsion_.expired() && !collision_used_l )
				{
					auto col = owner->AddComponent<ComponentCollisionSphere>();
					col->AttachToModel( "mixamorig:LeftFoot" );
					col->SetRadius( 2.0f );
					col->SetName( "PlayerAttack_Left" );
					col->SetHitCollisionGroup( (u32)ComponentCollision::CollisionGroup::ENEMY );
					col->SetMass( 10000.0f );
					left_collsion_ = col;
				}
			}
		}
		else if( name == "Enemy" )
		{
			if( time > attack_cfg.hit_time_start && time < attack_cfg.hit_time_end && !collision_used_ )
			{
				if( collsion_.expired() )
				{
					auto col = owner->AddComponent<ComponentCollisionSphere>();
					col->AttachToModel( attack_cfg.bone_name );
					col->SetRadius( 2.0f );
					col->SetName( attack_cfg.collision_name );
					col->SetHitCollisionGroup( (u32)attack_cfg.hit_group );

					collsion_ = col;
				}
			}
			else
			{
				if( auto col = collsion_.lock() )
					owner->RemoveComponent( col );
			}
		}
	}

	if( auto model = owner->GetComponent<ComponentModel>() )
	{
		float time = model->GetAnimationTime();

		if( !model->IsPlaying() )
		{
			RemoveThisComponent();
			owner->RemoveComponent<ComponentCollisionSphere>();
			owner->AddComponent<StateIdleWalk>();
		}
	}
}
void StateRunAttack::GUI()
{
	__super::GUI();

	// GUI内に出現させる
	ImGui::Begin( GetOwner()->GetName().data() );
	{
		ImGui::Separator();
		if( ImGui::TreeNode( u8"State RunAttack***" ) )
		{
			ImGui::TreePop();
		}
	}
	ImGui::End();
}

CEREAL_REGISTER_TYPE( StateRunAttack )
CEREAL_REGISTER_POLYMORPHIC_RELATION( Component, StateRunAttack )
