#include "Player.h"
#include "Bullet.h"
#include <DxLib.h>

#include <Game/Component/ComponentGameCamera.h>
#include <Game/Game01/Header/PlayerControllers.h>

namespace Game01
{
	bool Player::Init()
	{
		Super::Init();

		// プレイヤー
		SetName( "Player" );
		SetTranslate( { 0.0f, 0.0f, 0.0f } );

		auto col = AddComponent<ComponentCollisionCapsule>();	 //
		col->SetRadius( 3.0f );
		col->SetHeight( 13.0f );
		col->UseGravity();
		col->SetCollisionGroup( ComponentCollision::CollisionGroup::PLAYER );

		//auto model = AddComponent<ComponentModel>( "data/Sample/Player/Model.mv1" );
		auto model = AddComponent<ComponentModel>( "data/Game/Models/Player1/ch1.mv1" );
		model->SetAnimation( {
			{	  "idle",		  "data/Game/Models/Player1/Ani/Idle.mv1", 1, 1.0f},
			{	  "walk",	  "data/Game/Models/Player1/Ani/Walking.mv1", 1, 1.0f},
			{	  "att1",	  "data/Game/Models/Player1/Ani/Attack1.mv1", 1, 1.0f},
			{	  "att2",	  "data/Game/Models/Player1/Ani/Attack2.mv1", 1, 1.0f},
			{	  "att3",	  "data/Game/Models/Player1/Ani/Attack3.mv1", 1, 1.0f},
			{"floating",	  "data/Game/Models/Player1/Ani/Floating.mv1", 1, 1.0f},
			{ "jumpatt", "data/Game/Models/Player1/Ani/KickAttack.mv1", 1, 1.0f},
			{	  "run",		 "data/Game/Models/Player1/Ani/Run.mv1", 1, 1.0f},
			{  "runAtt",	"data/Game/Models/Player1/Ani/RunAttack.mv1", 1, 1.5f},
			{	  "guard",	   "data/Game/Models/Player1/Ani/guard.mv1", 1, 4.0f},
			{	  "hit",		 "data/Game/Models/Player1/Ani/hit.mv1", 1, 3.0f},
			{ "counter",	 "data/Game/Models/Player1/Ani/Counter.mv1", 1, 2.0f},
			{	  "aim",		 "data/Game/Models/Player1/Ani/aim.mv1", 1, 1.0f},
			{	  "death",	   "data/Game/Models/Player1/Ani/death.mv1", 1, 1.0f},
		} );

		model->PlayAnimation( "idle", true );

		model_ = model;

		controller_ = std::make_shared<Player1Controller>();

		auto obj_ctrl = AddComponent<StateIdleWalk>();

		return true;
	}

	void Player::Update()
	{
		Super::Update();
	}
	//当たり判定
	void Player::OnHit( const ComponentCollision::HitInfo& hit_info )
	{
		Super::OnHit( hit_info );

		auto hit_owner_name = hit_info.hit_collision_->GetOwner()->GetName();
		auto enemy_col_name = hit_info.hit_collision_->GetName();

		auto hit_object		 = hit_info.hit_collision_->GetOwner();
		auto hit_object_name = hit_object->GetNameDefault();

		auto collision = hit_info.hit_collision_;

		if( enemy_col_name == "NpcAttack" || hit_object_name == "Bullet" )
		{
			const std::string eff	 = "data/Sample/Effects/hit_eff.efkefc";
			const float3	  pos	 = hit_info.hit_position_;
			auto			  effect = ComponentEffect::Object::Create( eff, pos );

			effect->SetScaleAxisXYZ( 2.0f );

			if( auto owner = collision->GetOwner() )
			{
				owner->RemoveComponent( collision );

				if( auto state = owner->GetComponent<StateAttackBase>() )
				{
					state->collision_used_ = true;
				}
			}

			//ガードのでNPCの攻撃を無視する
			if( guard_ )
			{
				guard_count_ = 60;

				return;
			}
			//		攻撃を受ける処理
			else if( !GetComponent<StateHit>() )
			{
				SetHit( true );

				if( GetComponent<StateIdleWalk>() )
				{
					RemoveComponent<StateIdleWalk>();
				}

				if( !GetComponent<StateHit>() )
				{
					AddComponent<StateHit>();
				}
			}

			hp_--;
		}
	}
	void Player::Draw()
	{
		__super::Draw();

		// 3D座標（頭の上）
		VECTOR worldPos;
		worldPos.x = GetTranslate().x;
		worldPos.y = GetTranslate().y + 20.0f;	  // 頭の上
		worldPos.z = GetTranslate().z;

		// 3D → 2D
		VECTOR screen = ConvWorldPosToScreenPos( worldPos );

		// カメラ裏なら描かない
		if( screen.z < 0.0f )
			return;

		float barW = 60.0f;
		float barH = 8.0f;

		const int HP_MAX = 20;

		float hpRate = (float)hp_ / (float)HP_MAX;
		hpRate		 = std::clamp( hpRate, 0.0f, 1.0f );

		float x = screen.x - barW * 0.5f;
		float y = screen.y;

		if( !death_ )
		{
			// 枠
			DrawBox( (int)( x - 1 ), (int)( y - 1 ), (int)( x + barW + 1 ), (int)( y + barH + 1 ), GetColor( 255, 255, 255 ), TRUE );

			// 背景
			DrawBox( (int)x, (int)y, (int)( x + barW ), (int)( y + barH ), GetColor( 0, 0, 0 ), TRUE );

			// HPバー
			DrawBox( (int)x, (int)y, (int)( x + barW * hpRate ), (int)( y + barH ), GetColor( 0, 255, 0 ), TRUE );
		}
	}

	void Player::GUI()
	{
		__super::GUI();

		// GUI内に出現させる
		ImGui::Begin( GetName().data() );
		{
			ImGui::Separator();

			// 移動の基本情報
			ImGui::Checkbox( "hit", &hit_ );
			ImGui::Checkbox( "guard", &guard_ );
		}
		ImGui::End();
	}

}	 // namespace Game01
