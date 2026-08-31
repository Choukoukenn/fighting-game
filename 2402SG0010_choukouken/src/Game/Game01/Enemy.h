#pragma once
#include <System/Scene.h>
#include "CharacterBase.h"

namespace Game01
{
	USING_PTR( Enemy );

	class Enemy : public CharacterBase
	{
	public:
		BP_OBJECT_DECL( Enemy, u8"Game01 の Enemy" );

		//! @brief 初期化
		//! @return 成功失敗
		bool Init() override;

		//! @brief 更新
		void Update() override;

		//! @brief GUI
		void GUI() override;

		void Draw() override;

		void OnHit( const ComponentCollision::HitInfo& hit_info ) override;

		bool GetAI() const
		{
			return move_mode_;
		}

	private:
		static float speed_;

		int attack_cooldown_ = 0;
		int attack_wait_	 = 0;

		const int ATTACK_COOLDOWN = 60;
		const int REACTION_TIME	  = 20;

		bool move_mode_ = false;

		int erase_count_ = 60 * 5;

		std::shared_ptr<IController> player_ctrl_;
		std::shared_ptr<IController> ai_ctrl_;
	};

}	 // namespace Game01
