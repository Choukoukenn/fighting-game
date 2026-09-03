#pragma once
#include <System/Scene.h>

namespace Game01 {
USING_PTR(HitEff);

class HitEff : public Object
{
public:
    BP_OBJECT_DECL(HitEff, u8"Game01 の HitEff");

    bool Init() override;

    void Update() override;

    void Draw() override;

    void GUI() override;

private:
    float count_ = 100;
};

}    // namespace Game01
