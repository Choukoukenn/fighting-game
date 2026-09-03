#pragma once
#include <System/Scene.h>
#include "CharacterBase.h"

namespace Game01 {
USING_PTR(Player);

class Player : public CharacterBase
{
public:
    BP_OBJECT_DECL(Player, u8"Game01 の Player");

    bool Init() override;

    void Update() override;

    void OnHit(const ComponentCollision::HitInfo& hit_info) override;

    void Draw() override;

    void GUI() override;

private:
};

}    // namespace Game01
