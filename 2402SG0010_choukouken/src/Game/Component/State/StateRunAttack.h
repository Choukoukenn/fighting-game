#pragma once
#include <System/Scene.h>
#include <System/Component/Component.h>
#include "StateAttackbase.h"

USING_PTR(StateRunAttack);

class StateRunAttack : public StateAttackBase
{
public:
    BP_COMPONENT_DECL(StateRunAttack, u8"State Attack3***");

    void Init() override;

    void Update() override;

    void GUI() override;

    void RemoveRightCollision() { right_collsion_.reset(); }

    void RemoveLeftCollision() { left_collsion_.reset(); }

private:
    //--------------------------------------------------------------------
    //! @name Cereal処理
    //--------------------------------------------------------------------
    //@{
    ComponentWeakPtr left_collsion_;
    ComponentWeakPtr right_collsion_;

    ComponentWeakPtr collsion_;

    //! @brief セーブ
    // @param arc アーカイバ
    // @param ver バージョン
    CEREAL_SAVELOAD(arc, ver) { arc(cereal::make_nvp("Component", cereal::base_class<Component>(this))); }
};

CEREAL_CLASS_VERSION(StateRunAttack, 1);
