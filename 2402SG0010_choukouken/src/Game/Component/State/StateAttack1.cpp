#pragma once
#include "StateAttack1.h"
#include "StateAttack2.h"
#include "StateIdleWalk.h"
#include <Game/Game01/Player.h>
#include <System/Component/ComponentModel.h>
#include <System/Component/ComponentCollisionSphere.h>

using namespace Game01;

void StateAttack1::Init()
{
	__super::Init();
	auto owner = GetOwner();

	auto name = owner->GetName();

	if( name == "Player" )
	{
		attack_cfg = { 0.0f, 0.75f, 0.7f, 1.5f, "mixamorig:RightFoot", "PlayerAttack", ComponentCollision::CollisionGroup::ENEMY };
	}

	if( name == "Enemy" )
	{
		attack_cfg = { 0.3f, 1.0f, 0.5f, 1.5f, "mixamorig:RightHand", "NpcAttack", ComponentCollision::CollisionGroup::PLAYER };
	}

	if( auto model = owner->GetComponent<ComponentModel>() )
	{
		model->PlayAnimation( "att1" );
	}
}

void StateAttack1::Update()
{
	__super::Update();

	// オーナー(自分がAddComponentされたObject)を取得します
	// 処理されるときは必ずOwnerは存在しますので基本的にnullptrチェックは必要ありません
	auto owner = GetOwner();

	auto name = owner->GetName();

	if( auto model = owner->GetComponent<ComponentModel>() )
	{
		auto ch = dynamic_cast<CharacterBase*>( owner );
		if( !ch || !ch->controller_ )
			return;

		auto& ctrl = ch->controller_;

		{
			//if( name == "Player" )
			//{
			//	float			time		   = model->GetAnimationTime();
			//	constexpr float hit_time_start = 0.00f;
			//	constexpr float hit_time_end   = 0.75f;

			//	//時間内しか出ないコリジョン
			//	if( time > hit_time_start && time < hit_time_end )
			//	{
			//		auto player = Scene::Object::Get<Game01::Player>( "Player" );

			//		bool hit_check = player->GetHit();
			//		//打たれた時中断
			//		if( hit_check )
			//		{
			//			owner->RemoveComponent( shared_from_this() );
			//			return;
			//		}

			//		if( right_collsion_.expired() && !collision_used_ )
			//		{
			//			auto col = owner->AddComponent<ComponentCollisionSphere>();
			//			col->AttachToModel( "mixamorig:RightFoot" );
			//			col->SetRadius( 2.0f );
			//			col->SetName( "PlayerAttack" );
			//			col->SetMass( 5000.0f );

			//			//コリジョングループ
			//			col->SetHitCollisionGroup( (u32)ComponentCollision::CollisionGroup::ENEMY );

			//			//col2->used_ = false;

			//			right_collsion_ = col;
			//		}
			//	}
			//	//時間外ならコリジョンを消す
			//	else
			//	{
			//		if( auto right_collsion = right_collsion_.lock() )
			//		{
			//			owner->RemoveComponent( right_collsion );
			//		}
			//	}

			//			//次のモーションに進む
			//	if( Input::IsKeyDown( KEY_INPUT_X ) )
			//	{
			//		constexpr float start_time = 0.7f;
			//		constexpr float end_time   = 1.5f;

			//		if( time >= start_time && time <= end_time )
			//		{
			//			owner->RemoveComponent( shared_from_this() );

			//			if( auto right_collsion = right_collsion_.lock() )
			//			{
			//				owner->RemoveComponent( right_collsion );
			//			}

			//			owner->AddComponent<StateAttack2>();
			//		}
			//	}
			//}

			//if( name == "Enemy" )
			//{
			//	//----
			//	float			time		   = model->GetAnimationTime();
			//	constexpr float hit_time_start = 0.50f;
			//	constexpr float hit_time_end   = 1.5f;

			//	//時間内しか出ないコリジョン
			//	if( time > hit_time_start && time < hit_time_end )
			//	{
			//		if( right_collsion_.expired() && !collision_used_ )
			//		{
			//			auto col = owner->AddComponent<ComponentCollisionSphere>();
			//			col->AttachToModel( "mixamorig:RightHand" );
			//			col->SetRadius( 2.0f );
			//			col->SetName( "NpcAttack" );
			//			col->SetMass( 5000.0f );

			//			//コリジョングループ
			//			col->SetHitCollisionGroup( (u32)ComponentCollision::CollisionGroup::PLAYER );

			//			//col2->used_ = false;

			//			right_collsion_ = col;
			//		}
			//	}
			//	//時間外ならコリジョンを消す
			//	else
			//	{
			//		if( auto right_collsion = right_collsion_.lock() )
			//		{
			//			owner->RemoveComponent( right_collsion );
			//		}
			//	}
			//}
		}

		float time = model->GetAnimationTime();

		if( time > attack_cfg.hit_time_start && time < attack_cfg.hit_time_end && !collision_used_ )
		{
			auto owner = GetOwner();

			auto chr = dynamic_cast<CharacterBase*>( owner );

			bool hit_check = chr->GetHit();
			//打たれた時中断
			if( hit_check )
			{
				if( auto col = right_collsion_.lock() )
					owner->RemoveComponent( col );

				RemoveThisComponent();

				return;
			}

			if( right_collsion_.expired() )
			{
				auto col = owner->AddComponent<ComponentCollisionSphere>();
				col->AttachToModel( attack_cfg.bone_name );
				col->SetRadius( 4.0f );
				col->SetName( attack_cfg.collision_name );
				col->SetHitCollisionGroup( (u32)attack_cfg.hit_group );

				right_collsion_ = col;
			}
		}
		else
		{
			if( auto col = right_collsion_.lock() )
				owner->RemoveComponent( col );
		}

		//次のモーションに進む
		if( ctrl->AttackDown() )
		{
			//constexpr float start_time = attack_cfg.next_time_start;
			//constexpr float end_time   = 1.5f;

			if( time >= attack_cfg.next_time_start && time <= attack_cfg.next_time_end )
			{
				owner->RemoveComponent( shared_from_this() );

				if( auto right_collsion = right_collsion_.lock() )
				{
					owner->RemoveComponent( right_collsion );
				}

				owner->AddComponent<StateAttack2>();
			}
		}

		//アクションが終わったら
		if( !model->IsPlaying() )
		{
			if( !owner->GetComponent<StateIdleWalk>() )
			{
				owner->AddComponent<StateIdleWalk>();
			}

			RemoveThisComponent();
		}
	}
}

void StateAttack1::GUI()
{
	__super::GUI();

	// GUI内に出現させる
	ImGui::Begin( GetOwner()->GetName().data() );
	{
		ImGui::Separator();
		if( ImGui::TreeNode( u8"State Attack1*" ) )
		{
			ImGui::TreePop();
		}
	}
	ImGui::End();
}

CEREAL_REGISTER_TYPE( StateAttack1 )
CEREAL_REGISTER_POLYMORPHIC_RELATION( Component, StateAttack1 )
