#pragma once
#include <System/Scene.h>

namespace Game01 {
USING_PTR(Ground);

class Ground : public Object
{
public:
    BP_OBJECT_DECL(Ground, u8"Game01 の Ground");

    bool Init() override;

    void Update() override;

private:
};

}    // namespace Game01
