#pragma once
#include "StateAttackBase.h"
#include "StateAttack2.h"
#include "StateIdleWalk.h"
#include <Game/Game01/Player.h>
#include <System/Component/ComponentModel.h>
#include <System/Component/ComponentCollisionSphere.h>

CEREAL_REGISTER_TYPE( StateAttackBase )
CEREAL_REGISTER_POLYMORPHIC_RELATION( Component, StateAttackBase )
