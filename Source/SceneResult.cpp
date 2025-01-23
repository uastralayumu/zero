#include "System/Graphics.h"
#include"SceneResult.h"
#include "SceneTitle.h"
#include"System/Input.h"
#include"SceneGame.h"
#include"SceneManager.h"
#include"SceneLoading.h"
#include"System/Audio.h"
#include"Player.h"
//初期化
void SceneResult::Initialize()
{
	//スプライト初期化
	sprite = new Sprite("Data/Sprite/result.png");

	Score = new Sprite("Data/Sprite/number.png");

	BGMresult = Audio::Instance().LoadAudioSource("Data/Sound/BGM_main - .wav");

	for (int i = 0; i <= 3; i++)
	{
		if (highscore[i] < Player::Instance().HighScore())
		{
			highscore[i] = Player::Instance().HighScore();
			break;
		}
	}
}

//終了化
void SceneResult::Finalize()
{
	//スプライト終了化
	if (sprite != nullptr)
	{
		delete sprite;
		sprite = nullptr;
	}
	if (BGMresult != nullptr)
	{
		delete BGMresult;
		BGMresult = nullptr;
	}
}

//更新処理
void SceneResult::Update(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	//何かボタンを押したらゲームシーンへの切り替え
	const GamePadButton anyButton = GamePad::BTN_X;
	if (gamePad.GetButtonDown() & anyButton)
	{
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
	}
	BGMresult->Play(false);
}

//描画処理
void SceneResult::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	RenderState* renderState = graphics.GetRenderState();

	//描画準備
	RenderContext rc;
	rc.deviceContext = dc;
	rc.renderState = graphics.GetRenderState();

	//2Dスプライト描画
	{
		//タイトル描画
		float screenWidth = static_cast<float>(graphics.GetScreenWidth());
		float screeHeight = static_cast<float>(graphics.GetScreenHeight());
		sprite->Render(rc,
			0, 0, 0, screenWidth, screeHeight,
			0,
			1, 1, 1, 1);

		for (int m = 0; m < 3; m++)
		{

			int i = 1;
			int j = 0;
			int k = 0;
			while (i - 1 <= highscore[m])
			{
				k = highscore[m] / i;
				k = k % 10;
				Score->Render(rc,
					900 - (j * 100), 170 + m * 150, 0, 100, 50,
					(k * 100), 0, 100, 100,
					0,
					1, 1, 1, 1);
				j++;
				i *= 10;
			}
		}
			int score = Player::Instance().HighScore();
			{
				int i = 1;
				int j = 0;
				int k = 0;
				while (i - 1 <= score)
				{
					k = score / i;
					k = k % 10;
					Score->Render(rc,
						900 - (j * 100), 660, 0, 100, 50,
						(k * 100), 0, 100, 100,
						0,
						1, 1, 1, 1);
					j++;
					i *= 10;
				}
			}
		
	}
}

//GUI描画
void SceneResult::DrawGUI()
{

}