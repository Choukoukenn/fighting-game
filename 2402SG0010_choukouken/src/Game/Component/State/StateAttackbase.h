#pragma once
#include <System/Scene.h>
#include <System/Component/Component.h>

USING_PTR( StateAttackBase );

class StateAttackBase : public Component
{
public:
	BP_COMPONENT_DECL( StateAttackBase, u8"StateAttackBase*" );

	bool collision_used_ = false;

	bool collision_used_r = false;

	bool collision_used_l = false;

	struct AttackConfig
	{
		float							   hit_time_start;
		float							   hit_time_end;
		float							   next_time_start;
		float							   next_time_end;
		std::string						   bone_name;
		std::string						   collision_name;
		ComponentCollision::CollisionGroup hit_group;
	};

	AttackConfig attack_cfg;

private:
	//--------------------------------------------------------------------
	//! @name Cereal処理
	//--------------------------------------------------------------------
	//@{

	//! @brief セーブ
	// @param arc アーカイバ
	// @param ver バージョン
	CEREAL_SAVELOAD( arc, ver )
	{
		arc( cereal::make_nvp( "Component", cereal::base_class<Component>( this ) ) );
	}
};

CEREAL_CLASS_VERSION( StateAttackBase, 1 );
