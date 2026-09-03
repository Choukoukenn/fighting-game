#pragma once
#include <System/Scene.h>
#include "Header/IController.h"
#include <Game/Component/State/StateIdleWalk.h>
#include <Game/Component/State/StateAttack1.h>
#include <Game/Component/State/StateAttackJump.h>
#include <Game/Component/State/StateHit.h>
#include <Game/Component/State/StateGuard.h>
#include <Game/Component/State/StateCounter.h>
#include <Game/Component/State/StateAttackBase.h>
#include <Game/Component/State/StateRunAttack.h>
#include <Game/Game01/HitEff.h>
#include <Game/Game01/Game01.h>

#include <System/Component/ComponentCollisionModel.h>
#include <System/Component/ComponentModel.h>
#include <System/Component/ComponentCollisionCapsule.h>
#include <System/Component/ComponentEffect.h>

namespace Game01 {
USING_PTR(CharacterBase);

class CharacterBase : public Object
{
public:
    BP_OBJECT_DECL(CharacterBase, u8"CharacterBase");

    bool Init() override;

    void Update() override;

    void OnHit(const ComponentCollision::HitInfo& hit_info) override;

    bool GetJump() const { return is_jump_; }

    bool GetHit() const { return hit_; }

    void SetHit(bool hit) { hit_ = hit; }

    void SetGuard(bool guard) { guard_ = guard; }

    //
    void SetJump(bool is_jump)
    {
        is_jump_ = is_jump;
        if(is_jump_) {
            AddTranslate({0, jump_speed_, 0});
        }
    }

    bool                         GetDeal() const { return death_; }
    std::shared_ptr<IController> controller_;

public:
    bool first_ = false;

protected:
    bool  is_jump_     = false;
    float jump_speed_  = 1.0f;
    bool  hit_         = false;
    bool  guard_       = false;
    int   hp_          = 20;
    int   guard_count_ = 0;
    bool  death_       = false;

    ComponentModelWeakPtr model_;
};

}    // namespace Game01
