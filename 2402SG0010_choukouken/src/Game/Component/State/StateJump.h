#pragma once
#include <System/Scene.h>
#include <System/Component/Component.h>
#include <System/Component/ComponentCollision.h>

USING_PTR( StateJump );

class StateJump : public Component
{
public:
	BP_COMPONENT_DECL( StateJump, u8"State Jump***" );

	void Init() override;

	void Update() override;

	void GUI() override;

	void SetFlag( bool flag )
	{
		attack_ = flag;
	}

	void SetMoveDir( float3 dir )
	{
		move_dir_ = dir;
	}

	StateJumpPtr SetJumpMoveSpeed( const float speed );

private:
	//--------------------------------------------------------------------
	//! @name Cereal処理
	//--------------------------------------------------------------------
	//@{

	float move_speed_ = 0.5f;
	float rot_speed_  = 20.0f;

	bool attack_ = false;

	float3 move_dir_;
	//! @brief セーブ
	// @param arc アーカイバ
	// @param ver バージョン
	CEREAL_SAVELOAD( arc, ver )
	{
		arc( cereal::make_nvp( "Component", cereal::base_class<Component>( this ) ) );
	}
};

CEREAL_CLASS_VERSION( StateJump, 1 );
