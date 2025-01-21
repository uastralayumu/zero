#include "System/Graphics.h"
#include"SceneTitle.h"
#include"System/Input.h"
#include"SceneGame.h"
#include"SceneManager.h"
#include"SceneLoading.h"
#include "SceneTutorial.h"
#include"SceneTutorial.h"

//初期化
void SceneTutorial::Initialize()
{
	//スプライト初期化
	sprite = new Sprite("Data/Sprite/tutorial_1.png");
}

//終了化
void SceneTutorial::Finalize()
{
	//スプライト終了化
	if (sprite != nullptr)
	{
		delete sprite;
		sprite = nullptr;
	}
}

//更新処理
void SceneTutorial::Update(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	//何かボタンを押したらゲームシーンへの切り替え
	const GamePadButton anyButton = GamePad::BTN_X;
	if (gamePad.GetButtonDown() & anyButton)
	{
		tutorial++;
	}

	switch (tutorial)
	{
	case 1:	sprite = new Sprite("Data/Sprite/tutorial_2.png");
		break;
	case 2:	sprite = new Sprite("Data/Sprite/tutorial_3.png");
		break;
	case 3:	sprite = new Sprite("Data/Sprite/tutorial_4.png");
		break;
	case 4:	sprite = new Sprite("Data/Sprite/tutorial_5.png");
		break;
	case 5:	SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
		break;
	}
}

//描画処理
void SceneTutorial::Render()
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
	}
}

//GUI描画
void SceneTutorial::DrawGUI()
{

}