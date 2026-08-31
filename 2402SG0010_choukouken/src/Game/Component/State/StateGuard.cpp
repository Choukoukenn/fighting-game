#pragma once
#include <Game/Component/State/StateGuard.h>
#include <System/Component/ComponentModel.h>
#include <System/Component/ComponentCollisionSphere.h>
#include <Game/Game01/Player.h>
#include "StateIdleWalk.h"

void StateGuard::Init()
{
	__super::Init();
	auto owner = GetOwner();
	if( auto model = owner->GetComponent<ComponentModel>() )
	{
		model->PlayAnimation( "guard" );
	}
}

void StateGuard::Update()
{
	__super::Update();

	// オーナー(自分がAddComponentされたObject)を取得します
	// 処理されるときは必ずOwnerは存在しますので基本的にnullptrチェックは必要ありません
	auto owner = GetOwner();

	if( auto model = owner->GetComponent<ComponentModel>() )
	{
		float time = model->GetAnimationTime();

		if( !model->IsPlaying() )
		{
			//終わりの時に
			if( auto chr = dynamic_cast<Game01::CharacterBase*>( owner ) )
			{
				chr->SetGuard( false );
			}

			RemoveThisComponent();
			owner->AddComponent<StateIdleWalk>();
		}
	}
}

void StateGuard::GUI()
{
	__super::GUI();

	// GUI内に出現させる
	ImGui::Begin( GetOwner()->GetName().data() );
	{
		ImGui::Separator();
		if( ImGui::TreeNode( u8"State Guard***" ) )
		{
			ImGui::TreePop();
		}
	}
	ImGui::End();
}

CEREAL_REGISTER_TYPE( StateGuard )
CEREAL_REGISTER_POLYMORPHIC_RELATION( Component, StateGuard )
