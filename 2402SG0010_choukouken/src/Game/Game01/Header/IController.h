#pragma once

class IController
{
public:
	virtual float MoveX() = 0;
	virtual float MoveZ() = 0;

	virtual bool JumpDown()	  = 0;
	virtual bool AttackDown() = 0;
	virtual bool GuardDown()  = 0;
	virtual bool Aim()		  = 0;
};
