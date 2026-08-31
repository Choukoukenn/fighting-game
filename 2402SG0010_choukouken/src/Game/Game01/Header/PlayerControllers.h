#pragma once
#include "IController.h"
using namespace Game01;

class Player1Controller : public IController
{
public:
	float MoveX() override
	{
		return Input::IsKey( KEY_INPUT_RIGHT ) - Input::IsKey( KEY_INPUT_LEFT );
	}
	float MoveZ() override
	{
		return Input::IsKey( KEY_INPUT_UP ) - Input::IsKey( KEY_INPUT_DOWN );
	}
	bool JumpDown() override
	{
		return Input::IsKeyDown( KEY_INPUT_NUMPAD0 );
	}
	bool AttackDown() override
	{
		return Input::IsKeyDown( KEY_INPUT_NUMPAD2 );
	}
	bool GuardDown() override
	{
		return Input::IsKeyDown( KEY_INPUT_NUMPAD3 );
	}
	bool Aim() override
	{
		return Input::IsKey( KEY_INPUT_NUMPAD1 );
	}
};

class Player2Controller : public IController
{
public:
	float MoveX() override
	{
		return Input::IsKey( KEY_INPUT_D ) - Input::IsKey( KEY_INPUT_A );
	}
	float MoveZ() override
	{
		return Input::IsKey( KEY_INPUT_W ) - Input::IsKey( KEY_INPUT_S );
	}
	bool JumpDown() override
	{
		return Input::IsKeyDown( KEY_INPUT_M );
	}
	bool AttackDown() override
	{
		return Input::IsKeyDown( KEY_INPUT_B );
	}
	bool GuardDown() override
	{
		return Input::IsKeyDown( KEY_INPUT_N );
	}
	bool Aim() override
	{
		return Input::IsKey( KEY_INPUT_V );
	}
};

class EnemyAttackController : public IController
{
public:
	float MoveX() override
	{
		return move_.x;
	}
	float MoveZ() override
	{
		return move_.z;
	}

	bool AttackDown() override
	{
		return attack_;
	}
	bool Aim() override
	{
		return false;
	}
	bool JumpDown() override
	{
		return false;
	}
	bool GuardDown() override
	{
		return false;
	}

	void SetMoveDir( const float3& dir )
	{
		move_ = normalize( dir );
	}

	void StopMove()
	{
		move_ = float3{ 0, 0, 0 };
	}

	void SetAttack( bool b )
	{
		attack_ = b;
	}

private:
	float3 move_{ 0, 0, 0 };
	bool   attack_ = false;
};
