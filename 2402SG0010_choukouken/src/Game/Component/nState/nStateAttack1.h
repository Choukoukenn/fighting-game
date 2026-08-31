#pragma once
#include <System/Scene.h>
#include <System/Component/Component.h>

USING_PTR( nStateAttack1 );

class nStateAttack1 : public Component
{
public:
	BP_COMPONENT_DECL( nStateAttack1, u8"State Attack1*" );

	void Init() override;

	void Update() override;

	void GUI() override;

private:
	ComponentWeakPtr lefe_collsion_;
	ComponentWeakPtr right_collsion_;

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

CEREAL_CLASS_VERSION( nStateAttack1, 1 );
