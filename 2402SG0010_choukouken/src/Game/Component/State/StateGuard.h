#pragma once
#include <System/Scene.h>
#include <System/Component/Component.h>

USING_PTR( StateGuard );

class StateGuard : public Component
{
public:
	BP_COMPONENT_DECL( StateGuard, u8"State Attack3***" );

	void Init() override;

	void Update() override;

	void GUI() override;

private:
	//--------------------------------------------------------------------
	//! @name Cereal処理
	//--------------------------------------------------------------------
	//@{

	//! @brief セーブ
	// @param arc アーカイバ
	// @param ver バージョン
	CEREAL_SAVELOAD( arc, ver )
	{
		arc( cereal::make_nvp( "Component", cereal::base_class<Component>( this ) ) );
	}
};

CEREAL_CLASS_VERSION( StateGuard, 1 );
