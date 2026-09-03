#pragma once
#include <System/Scene.h>
#include <System/Component/Component.h>
#include "StateAttackbase.h"

USING_PTR(StateAttack3);

class StateAttack3 : public StateAttackBase
{
public:
    BP_COMPONENT_DECL(StateAttack3, u8"State Attack3***");

    void Init() override;

    void Update() override;

    void GUI() override;

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

CEREAL_CLASS_VERSION(StateAttack3, 1);
