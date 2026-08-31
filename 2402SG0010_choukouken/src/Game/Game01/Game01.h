#pragma once
#include <System/Scene.h>
#include <System/Component/ComponentCamera.h>
#include <System/Component/ComponentModel.h>
#include <System/Component/ComponentObjectController.h>
#include <System/Component/ComponentCollisionModel.h>
#include <System/Component/ComponentCollisionCapsule.h>

namespace Game01
{
	enum class GameState
	{
		Start,
		Playing,
		GameOver
	};

	class Game01 : public Scene::Base
	{
	public:
		BP_CLASS_DECL( Game01, u8"Game02::Game01のシーン!!!" );

		bool Init() override;

		void Update() override;

		void LateDraw() override;

		void UpdateStart();

		void UpdatePlaying();

		void UpdateGameOver();

		GameState GetState() const
		{
			return state_;
		}

	private:
		GameState state_	 = GameState::Start;
		bool	  showManual = false;
	};

}	 // namespace Game01
