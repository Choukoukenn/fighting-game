#pragma once
#include <System/Scene.h>
#include <System/Component/Component.h>
#include "StateJump.h"

USING_PTR( StateIdleWalk );

class StateIdleWalk : public Component
{
public:
	BP_COMPONENT_DECL( StateIdleWalk, u8"state 移動、停止" );

	void Init() override;

	void Update() override;

	StateIdleWalkPtr SetMoveSpeed( const float speed );

	StateIdleWalkPtr SetRotateSpeed( const float speed );

	StateIdleWalkPtr SetKeys( int up, int down, int left, int right, int aim, int att, int def, int jump );

	const float GetMoveSpeed() const;
	const float GetRotateSpeed() const;

	inline const void SetFrontRotate( float rotate )
	{
		front_rot_ = rotate;
	}
	inline const float GetFrontRotate() const
	{
		return front_rot_;
	}

	void GUI() override;

private:
	static constexpr int RUN_COUNT_MAX = 30;

	float move_speed_ = 0.5f;
	float rot_speed_  = 20.0f;

	bool run_flag_	= false;
	int	 run_count_ = 0;
	int	 log_key_	= -1;

	int key_up_	   = KEY_INPUT_UP;
	int key_down_  = KEY_INPUT_DOWN;
	int key_left_  = KEY_INPUT_LEFT;
	int key_right_ = KEY_INPUT_RIGHT;
	int key_aim_   = KEY_INPUT_NUMPAD1;
	int key_att_   = KEY_INPUT_NUMPAD2;
	int key_def_   = KEY_INPUT_NUMPAD3;
	int key_jump_  = KEY_INPUT_NUMPAD0;

	float front_rot_ = 0.0f;	//!<前方ベクトルの回転角度(0-360度)

	bool aim_flag_ = false;

	float debug = 0.0f;

	int prev_key_dir_ = 0;

	static constexpr float DEAD = 0.2f;

	int					 no_input_count_ = 0;
	static constexpr int NO_INPUT_GRACE	 = 30;

	int bullet_cout = 60;

	//--------------------------------------------------------------------
	//! @name Cereal処理
	//--------------------------------------------------------------------
	//@{

	//! @brief セーブ
	// @param arc アーカイバ
	// @param ver バージョン
	CEREAL_SAVELOAD( arc, ver )
	{
		arc( cereal::make_nvp( "speed_", move_speed_ ),
			 CEREAL_NVP( rot_speed_ ),

			 CEREAL_NVP( key_up_ ),
			 CEREAL_NVP( key_down_ ),
			 CEREAL_NVP( key_left_ ),
			 CEREAL_NVP( key_right_ ),

			 CEREAL_NVP( front_rot_ ) );

		arc( cereal::make_nvp( "Component", cereal::base_class<Component>( this ) ) );
	}
};
CEREAL_CLASS_VERSION( StateIdleWalk, 1 );
