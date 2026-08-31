#include "CharacterBase.h"

namespace Game01
{

	bool CharacterBase::Init()
	{
		Super::Init();

		return true;
	}

	void CharacterBase::Update()
	{
		Super::Update();

		auto scene = Scene::GetCurrentScene<Game01>();
		if( scene->GetState() != GameState::Playing )
		{
			if( GetComponent<StateIdleWalk>() )
			{
				RemoveComponent<StateIdleWalk>();
			}
			return;
		}
		else
		{
			if( !GetComponent<StateIdleWalk>() && !first_ )
			{
				AddComponent<StateIdleWalk>();
				first_ = true;
			}
		}

		//ジャンプの時
		if( is_jump_ )
		{
			AddTranslate( { 0, jump_speed_, 0 } );
		}

		if( guard_count_ > 0 )
		{
			guard_count_--;
		}

		if( guard_count_ > 0 && controller_->AttackDown() )
		{
			if( GetComponent<StateGuard>() )
			{
				RemoveComponent<StateGuard>();
				AddComponent<StateCounter>();
			}
		}

		if( hp_ <= 0 && !death_ )
		{
			death_ = true;

			RemoveComponent<StateHit>();
			RemoveComponent<StateIdleWalk>();

			if( auto model = GetComponent<ComponentModel>() )
			{
				if( GetComponent<ComponentCollisionCapsule>() )
				{
					RemoveComponent<ComponentCollisionCapsule>();
				}

				model->PlayAnimation( "death", false );
			}
		}

		float3 pos = GetTranslate();

		float minX = -100.0f;
		float maxX = 100.0f;
		float minZ = -100.0f;
		float maxZ = 100.0f;

		if( pos.x < minX )
			pos.x = minX;
		if( pos.x > maxX )
			pos.x = maxX;
		if( pos.z < minZ )
			pos.z = minZ;
		if( pos.z > maxZ )
			pos.z = maxZ;

		SetTranslate( pos );
	}

	void CharacterBase::OnHit( const ComponentCollision::HitInfo& hit_info )
	{
		Super::OnHit( hit_info );

		auto hit_owner_name = hit_info.hit_collision_->GetOwner()->GetName();

		if( hit_owner_name == "Ground" )
		{
			if( is_jump_ )
			{
				is_jump_ = false;
				RemoveComponent<StateJump>();

				if( auto attack_state = GetComponent<StateAttackJump>() )
				{
					attack_state->OnRemove();
					RemoveComponent<StateAttackJump>();
				}

				if( !GetComponent<StateIdleWalk>() )
				{
					AddComponent<StateIdleWalk>();
				}
			}
		}
	}

}	 // namespace Game01
