#include "System/Graphics.h"
#include"SceneTitle.h"
#include"System/Input.h"
#include"SceneGame.h"
#include"SceneManager.h"
#include"SceneLoading.h"
#include "SceneTutorial.h"
#include"SceneTutorial.h"

//������
void SceneTutorial::Initialize()
{
	//�X�v���C�g������
	sprite = new Sprite("Data/Sprite/tutorial_1.png");
}

//�I����
void SceneTutorial::Finalize()
{
	//�X�v���C�g�I����
	if (sprite != nullptr)
	{
		delete sprite;
		sprite = nullptr;
	}
}

//�X�V����
void SceneTutorial::Update(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	//�����{�^������������Q�[���V�[���ւ̐؂�ւ�
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

//�`�揈��
void SceneTutorial::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	RenderState* renderState = graphics.GetRenderState();

	//�`�揀��
	RenderContext rc;
	rc.deviceContext = dc;
	rc.renderState = graphics.GetRenderState();

	//2D�X�v���C�g�`��
	{
		//�^�C�g���`��
		float screenWidth = static_cast<float>(graphics.GetScreenWidth());
		float screeHeight = static_cast<float>(graphics.GetScreenHeight());
		sprite->Render(rc,
			0, 0, 0, screenWidth, screeHeight,
			0,
			1, 1, 1, 1);
	}
}

//GUI�`��
void SceneTutorial::DrawGUI()
{

}