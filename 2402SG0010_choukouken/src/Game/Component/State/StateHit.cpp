#pragma once
#include "StateHit.h"
#include "StateIdleWalk.h"

#include <Game/Game01/Player.h>
#include <System/Component/ComponentModel.h>
#include <System/Component/ComponentCollisionSphere.h>
#include <Game/Game01/Enemy.h>

void StateHit::Init()
{
	__super::Init();
	auto owner = GetOwner();

	if( auto model = owner->GetComponent<ComponentModel>() )
	{
		model->PlayAnimation( "hit" );
	}
}

void StateHit::Update()
{
	__super::Update();

	// オーナー(自分がAddComponentされたObject)を取得します
	// 処理されるときは必ずOwnerは存在しますので基本的にnullptrチェックは必要ありません
	auto owner = GetOwner();

	auto name = owner->GetName();

	if( auto model = owner->GetComponent<ComponentModel>() )
	{
		//アクションが終わったら
		if( model->GetPlayAnimationName() != "hit" )
		{
			if( !owner->GetComponent<StateIdleWalk>() )
			{
				owner->AddComponent<StateIdleWalk>();
			}
			if( auto chr = dynamic_cast<Game01::CharacterBase*>( owner ) )
			{
				chr->SetHit( false );
			}

			RemoveThisComponent();
		}
	}
}

void StateHit::GUI()
{
	__super::GUI();

	// GUI内に出現させる
	ImGui::Begin( GetOwner()->GetName().data() );
	{
		ImGui::Separator();
		if( ImGui::TreeNode( u8"State Hit*" ) )
		{
			ImGui::TreePop();
		}
	}
	ImGui::End();
}

CEREAL_REGISTER_TYPE( StateHit )
CEREAL_REGISTER_POLYMORPHIC_RELATION( Component, StateHit )
