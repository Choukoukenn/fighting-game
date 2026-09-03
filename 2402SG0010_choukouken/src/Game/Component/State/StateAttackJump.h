#pragma once
#include <System/Scene.h>
#include <System/Component/Component.h>
#include <System/Component/ComponentCollision.h>
#include "StateAttackbase.h"

USING_PTR(StateAttackJump);

class StateAttackJump : public StateAttackBase
{
public:
    BP_COMPONENT_DECL(StateAttackJump, u8"State Jumpatt***");

    void Init() override;

    void Update() override;

    void GUI() override;

    void OnRemove();

private:
    //--------------------------------------------------------------------
    //! @name Cereal処理
    //--------------------------------------------------------------------
    //@{
    ComponentWeakPtr collsion_;
    //! @brief セーブ
    // @param arc アーカイバ
    // @param ver バージョン
    CEREAL_SAVELOAD(arc, ver) { arc(cereal::make_nvp("Component", cereal::base_class<Component>(this))); }
};

CEREAL_CLASS_VERSION(StateAttackJump, 1);
