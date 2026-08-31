#include "Game01.h"
#include "Ground.h"
#include "Camera.h"
#include "Player.h"
#include "Enemy.h"

namespace Game01
{
	bool Game01::Init()
	{
		state_ = GameState::Start;

		// グランド
		Scene::Object::Create<Ground>();

		// プレイヤー
		Scene::Object::Create<Player>();

		// エネミー
		Scene::Object::Create<Enemy>();

		// カメラオブジェクト
		Scene::Object::Create<Camera>();

		// -----------------------------------------------------------------------------------------
		// 空オブジェクト(SkyDome)の追加 ④
		// -----------------------------------------------------------------------------------------
		{
			auto obj = Scene::Object::Create<Object>()	  //< Object作成
						   ->SetName( "Sky" );

			// オブジェクトにモデル能力を追加します
			obj->AddComponent<ComponentModel>( "data/Sample/SwordBout/Stage/Stage00_sky.mv1" );
		}

		return true;
	}

	void Game01::Update()
	{
		//--------------------------------------------------------------
		// 雲を動かすように空をY軸で少しづつ回転させます　⑤
		//--------------------------------------------------------------
		if( auto sky = Scene::Object::Get<Object>( "Sky" ) )
		{
			sky->AddRotationAxisXYZ( { 0, 0.1f, 0 } );
		}
		//--------------------------------------------------------------

		switch( state_ )
		{
		case GameState::Start:
			UpdateStart();

			break;

		case GameState::Playing:
			UpdatePlaying();
			break;

		case GameState::GameOver:
			UpdateGameOver();
			break;
		}
	}

	void Game01::LateDraw()
	{
		int old_size = GetFontSize();

		//int screenW = 1280;	   //画面幅
		//int screenH = 720;	   //画面高さ

		int screenW = 2560;	   //画面幅
		int screenH = 1440;	   //画面高さ

		const int offset = 50;

		static float blink_timer  = 0.0f;
		blink_timer				 += 1.0f / 60.0f;
		bool showText			  = ( sin( blink_timer * 3.0f ) > 0 );

		auto DrawOutlinedText = [ & ]( int x, int y, const char* text, uint32_t color ) {
			DrawString( x - 2, y - 2, text, GetColor( 0, 0, 0 ) );
			DrawString( x + 2, y - 2, text, GetColor( 0, 0, 0 ) );
			DrawString( x - 2, y + 2, text, GetColor( 0, 0, 0 ) );
			DrawString( x + 2, y + 2, text, GetColor( 0, 0, 0 ) );
			DrawString( x, y, text, color );
		};

		switch( state_ )
		{
		case GameState::Start:
			{
				SetFontSize( 48 );	  // 48pxに設定
				int baseY = screenH - 120;
				if( showText )
				{
					const char* text  = "PRESS ENTER TO START";
					int			textW = GetDrawStringWidth( text, strlen( text ) );
					int			x	  = ( screenW - textW ) / 2;
					int			y	  = baseY;
					DrawOutlinedText( x, y, text, GetColor( 255, 255, 255 ) );
				}

				// ==== マニュアル表示 ====

				int manualY = baseY - 400;

				const char* title  = "操作説明";
				int			titleW = GetDrawStringWidth( title, strlen( title ) );
				int			titleX = ( screenW - titleW ) / 2;
				int			titleY = manualY - 100;

				DrawOutlinedText( titleX, titleY, title, GetColor( 0, 255, 200 ) );
				SetFontSize( old_size );

				int leftX  = screenW / 2 - 500;
				int rightX = screenW / 2 + 200;

				SetFontSize( 32 );
				// ---- P2(左)
				DrawOutlinedText( leftX, manualY, "P2", GetColor( 255, 255, 0 ) );
				DrawOutlinedText( leftX, manualY + 44, "移動    ：WASD", GetColor( 255, 255, 255 ) );
				DrawOutlinedText( leftX, manualY + 88, "攻撃    ：B", GetColor( 255, 255, 255 ) );
				DrawOutlinedText( leftX, manualY + 120, "エイム  ：V", GetColor( 255, 255, 255 ) );
				DrawOutlinedText( leftX, manualY + 160, "防御    ：N", GetColor( 255, 255, 255 ) );
				DrawOutlinedText( leftX, manualY + 200, "ジャンプ：M", GetColor( 255, 255, 255 ) );

				// ---- P1(右)
				DrawOutlinedText( rightX, manualY, "P1", GetColor( 255, 255, 0 ) );
				DrawOutlinedText( rightX, manualY + 40, "移動    ：十字キー", GetColor( 255, 255, 255 ) );
				DrawOutlinedText( rightX, manualY + 80, "攻撃    ：数字キー2", GetColor( 255, 255, 255 ) );
				DrawOutlinedText( rightX, manualY + 120, "エイム  ：数字キー1", GetColor( 255, 255, 255 ) );
				DrawOutlinedText( rightX, manualY + 160, "防御    ：数字キー3", GetColor( 255, 255, 255 ) );
				DrawOutlinedText( rightX, manualY + 200, "ジャンプ：数字キー0", GetColor( 255, 255, 255 ) );

				int infoY = manualY + 240;

				DrawOutlinedText( screenW / 2 - 260, infoY, "エイム   ＋ 攻撃：弾を発射", GetColor( 0, 255, 200 ) );
				DrawOutlinedText( screenW / 2 - 260, infoY + 40, " 防御    ＋ 攻撃：カウンター", GetColor( 0, 255, 200 ) );
				DrawOutlinedText( screenW / 2 - 260, infoY + 80, "ダッシュ ＋ 攻撃：突進攻撃", GetColor( 0, 255, 200 ) );
				DrawOutlinedText( screenW / 2 - 260, infoY + 120, "方向キー連打    ：ダッシュ", GetColor( 0, 255, 200 ) );

				auto enemy = Scene::Object::Get<Enemy>( "Enemy" );

				if( enemy )
				{
					bool move_mode = enemy->GetAI();

					const char* hintText	= "Uキーで切り替え";
					const char* p2ModeText	= move_mode ? "（AI操作）" : "（手動操作）";
					auto		p2ModeColor = move_mode ? GetColor( 0, 255, 255 ) : GetColor( 255, 200, 0 );

					DrawOutlinedText( leftX + 60, manualY, p2ModeText, p2ModeColor );

					DrawOutlinedText( leftX, manualY - 32, hintText, GetColor( 255, 255, 255 ) );
				}

				SetFontSize( old_size );

				break;
			}

		case GameState::Playing:

			SetFontSize( 48 );

			if( showManual )
			{
				// 左上：P2
				int leftX = 20;
				int topY  = 20;

				DrawOutlinedText( leftX, topY, "P2", GetColor( 255, 255, 0 ) );
				DrawOutlinedText( leftX, topY + 44, "移動  ：WASD", GetColor( 255, 255, 255 ) );
				DrawOutlinedText( leftX, topY + 88, "攻撃  ：B", GetColor( 255, 255, 255 ) );
				DrawOutlinedText( leftX, topY + 132, "エイム：V", GetColor( 255, 255, 255 ) );
				DrawOutlinedText( leftX, topY + 176, "防御  ：N", GetColor( 255, 255, 255 ) );

				// 右上：P1
				int rightX = screenW - 400;

				DrawOutlinedText( rightX, topY, "P1", GetColor( 255, 255, 0 ) );
				DrawOutlinedText( rightX, topY + 44, "移動  ：十字キー", GetColor( 255, 255, 255 ) );
				DrawOutlinedText( rightX, topY + 88, "攻撃  ：数字2", GetColor( 255, 255, 255 ) );
				DrawOutlinedText( rightX, topY + 132, "エイム：数字1", GetColor( 255, 255, 255 ) );
				DrawOutlinedText( rightX, topY + 176, "防御  ：数字3", GetColor( 255, 255, 255 ) );

				int infoX = screenW / 2 - 300;
				int infoY = 20;

				DrawOutlinedText( infoX, infoY, "エイム  ＋攻撃：弾を発射", GetColor( 0, 255, 200 ) );
				DrawOutlinedText( infoX, infoY + 44, " 防御   ＋攻撃：カウンター", GetColor( 0, 255, 200 ) );
				DrawOutlinedText( infoX, infoY + 88, "ダッシュ＋攻撃：突進攻撃", GetColor( 0, 255, 200 ) );
				DrawOutlinedText( infoX, infoY + 132, "方向キー連打  ：ダッシュ", GetColor( 0, 255, 200 ) );
			}
			DrawOutlinedText( screenW - 600, screenH - 88, "U：AI操作 切替(P2のみ)", GetColor( 255, 200, 0 ) );
			DrawOutlinedText( screenW - 600, screenH - 44, "P：操作説明 表示/非表示", GetColor( 150, 150, 150 ) );

			SetFontSize( old_size );

			break;

		case GameState::GameOver:
			{
				if( showText )
				{
					SetFontSize( 48 );

					const char* text1 = "GAME OVER";
					const char* text2 = "PRESS R TO RESTART";

					int text1W = GetDrawStringWidth( text1, strlen( text1 ) );
					int text2W = GetDrawStringWidth( text2, strlen( text2 ) );
					int textH  = 48;
					int x1	   = ( screenW - text1W ) / 2;
					int x2	   = ( screenW - text2W ) / 2;

					int y2 = screenH - textH - offset;
					int y1 = y2 - textH - 10;

					DrawOutlinedText( x1, y1, text1, GetColor( 255, 0, 0 ) );
					DrawOutlinedText( x2, y2, text2, GetColor( 255, 255, 255 ) );

					SetFontSize( old_size );
					break;
				}
			}
		}
	}

	void Game01::UpdateStart()
	{
		if( CheckHitKey( KEY_INPUT_RETURN ) )
		{
			state_ = GameState::Playing;
		}
	}

	void Game01::UpdatePlaying()
	{
		if( CheckHitKey( KEY_INPUT_I ) )
		{
			//state_ = GameState::GameOver;
		}

		if( IsKeyDown( KEY_INPUT_P ) )
		{
			showManual = !showManual;
		}

		auto chrs = Scene::Object::GetArray<CharacterBase>();
		for( auto& chara : chrs )
		{
			if( chara->GetDeal() )
			{
				state_ = GameState::GameOver;
				break;
			}
		}
	}

	void Game01::UpdateGameOver()
	{
		if( CheckHitKey( KEY_INPUT_R ) )
		{
			Scene::Change( Scene::GetScene<Game01>() );
		}
	}

}	 // namespace Game01
