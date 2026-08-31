#include "Enemy.h"
#include "Player.h"

#include "Header/PlayerControllers.h"

namespace Game01
{
	float			Enemy::speed_ = 1.0f;	 // 任意の初期値
	constexpr float enemy_speed	  = 0.1f;	 //!< エネミー速度

	bool Enemy::Init()
	{
		Super::Init();

		// プレイヤー
		SetName( "Enemy" );
		//constexpr int size = 200;
		//float		  xpos = GetRand( size ) - size / 2;
		//float		  ypos = GetRand( size ) - size / 2;
		SetTranslate( { -20.0f, 5.0f, 0.0f } );
		SetScaleAxisXYZ( float3( 0.95f ) );

		auto col = AddComponent<ComponentCollisionCapsule>();	 //
		col->SetRadius( 3.0f );
		col->SetHeight( 13.0f );
		col->UseGravity();
		col->SetCollisionGroup( ComponentCollision::CollisionGroup::ENEMY );

		auto model = AddComponent<ComponentModel>( "data/Game/Models/Enemy/Enemy.mv1" );
		model->SetAnimation( {
			{	  "idle",	  "data/Game/Models/Enemy/Anim/Idle.mv1", 0, 1.0f},
			{	  "walk",	  "data/Game/Models/Enemy/Anim/Walk.mv1", 0, 1.0f},
			{	  "death",	   "data/Game/Models/Enemy/Anim/Death.mv1", 0, 1.0f},
			{	  "att1",	  "data/Game/Models/Enemy/Anim/natt1.mv1", 0, 2.0f},
			{	  "att2",	  "data/Game/Models/Enemy/Anim/natt2.mv1", 0, 1.0f},
			{	  "att3",	  "data/Game/Models/Enemy/Anim/natt3.mv1", 0, 1.0f},
			{	  "run",		 "data/Game/Models/Enemy/Anim/nrun.mv1", 0, 1.0f},
			{"floating", "data/Game/Models/Enemy/Anim/nfloating.mv1", 0, 1.0f},
			{	  "aim",		 "data/Game/Models/Enemy/Anim/naim.mv1", 0, 1.0f},
			{	  "hit",		 "data/Game/Models/Enemy/Anim/nhit.mv1", 0, 3.0f},
			{	  "guard",	   "data/Game/Models/Enemy/Anim/guard.mv1", 0, 4.0f},
			{ "jumpatt",	 "data/Game/Models/Enemy/Anim/njatt.mv1", 0, 1.0f},
			{  "runAtt",	"data/Game/Models/Enemy/Anim/nrunatt.mv1", 0, 1.5f},
			{ "counter",	 "data/Game/Models/Enemy/Anim/ncatt.mv1", 0, 2.0f},
		} );

		model->PlayAnimation( "idle", true );

		model_ = model;

		player_ctrl_ = std::make_shared<Player2Controller>();

		ai_ctrl_ = std::make_shared<EnemyAttackController>();

		controller_ = player_ctrl_;

		auto obj_ctrl = AddComponent<StateIdleWalk>();

		return true;
	}

	void Enemy::Update()
	{
		Super::Update();

		if( death_ )
			return;

		if( IsKeyDown( KEY_INPUT_U ) )
		{
			move_mode_ = !move_mode_;

			if( auto ai = std::dynamic_pointer_cast<EnemyAttackController>( ai_ctrl_ ) )
			{
				ai->StopMove();
				ai->SetAttack( false );
			}
		}

		if( move_mode_ )
		{
			controller_ = ai_ctrl_;
		}
		else
		{
			controller_ = player_ctrl_;
		}

		if( move_mode_ )
		{
			if( attack_cooldown_ > 0 )
				attack_cooldown_--;

			auto player = Scene::Object::Get<Object>( "Player" );
			if( !player )
				return;

			auto aiCtrl = std::dynamic_pointer_cast<EnemyAttackController>( controller_ );

			float3 diff	   = player->GetTranslate() - GetTranslate();
			diff.y		   = 0;
			float distance = length( diff );

			float attackRange = 15.0f;

			if( distance > attackRange )
			{
				attack_wait_ = 0;
				aiCtrl->SetMoveDir( diff );
				aiCtrl->SetAttack( false );
			}
			else
			{
				aiCtrl->StopMove();
				attack_wait_++;

				if( attack_wait_ > REACTION_TIME && attack_cooldown_ <= 0 )
				{
					auto model = GetComponent<ComponentModel>();
					model->GetRotationAxisXYZ();

					model->SetRotationToPosition( player->GetTranslate() );

					aiCtrl->SetAttack( true );
					attack_cooldown_ = ATTACK_COOLDOWN;
					attack_wait_	 = 0;
				}
				else
				{
					aiCtrl->SetAttack( false );
				}
			}
		}
	}

	void Enemy::GUI()
	{
		Super::GUI();
		//ImGui::Separator();

		//ImGui::DragFloat( u8"速度", &speed_, 0.01f, 0.01f, 10.0f, "%2.2f" );

		ImGui::Begin( GetName().data() );
		{
			ImGui::Separator();

			ImGui::DragFloat( "EnemySpeed", &speed_, 0.1f );

			ImGui::DragInt( "EnemyHp", &hp_, 0.1f );

			ImGui::Checkbox( "hit", &hit_ );
			ImGui::Checkbox( "guard", &guard_ );

			ImGui::DragInt( "g_count", &guard_count_, 0.1f );
		}
		ImGui::End();
	}

	void Enemy::Draw()
	{
		Super::Draw();

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
			DrawBox( (int)x, (int)y, (int)( x + barW * hpRate ), (int)( y + barH ), GetColor( 255, 0, 0 ), TRUE );

			if( move_mode_ )
			{
				const char* text = "AI操作中 Uキーで停止";

				int textW = GetDrawStringWidth( text, strlen( text ) );
				int textH = 16;

				int textX = (int)( x + barW * 0.5f - textW * 0.5f );
				int textY = (int)( y - textH - 4 );

				//SetFontSize( 16 );
				DrawString( textX, textY, text, GetColor( 0, 255, 255 ) );
			}
		}
	}

	void Enemy::OnHit( const ComponentCollision::HitInfo& hit_info )
	{
		Super::OnHit( hit_info );

		auto hit_object = hit_info.hit_collision_->GetOwner();
		auto col_name	= hit_info.hit_collision_->GetName();
		auto collision	= hit_info.hit_collision_;

		auto hit_object_name = hit_object->GetNameDefault();

		//当たり判定
		bool is_player_attack = col_name == "PlayerAttack" || col_name == "PlayerAttack_Right" || col_name == "PlayerAttack_Left"
							 || hit_object_name == "Bullet";

		if( is_player_attack )
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

				if( auto run = owner->GetComponent<StateRunAttack>() )
				{
					if( col_name == "PlayerAttack_Right" )
					{
						run->RemoveRightCollision();
						run->collision_used_r = true;
					}
					else if( col_name == "PlayerAttack_Left" )
					{
						run->RemoveLeftCollision();
						run->collision_used_l = true;
					}
				}
			}

			// ガード判定
			if( guard_ )
			{
				guard_count_ = 60;
				return;
			}
			// 受傷判定
			else if( !GetComponent<StateHit>() )
			{
				SetHit( true );

				RemoveComponent<StateIdleWalk>();
				AddComponent<StateHit>();
			}

			hp_--;
		}	 //printfDx( "HIT: %s\n", hit_object->GetNameDefault().data() );
	}

}	 // namespace Game01
