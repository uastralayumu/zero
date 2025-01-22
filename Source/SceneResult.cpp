#include "System/Graphics.h"
#include"SceneResult.h"
#include "SceneTitle.h"
#include"System/Input.h"
#include"SceneGame.h"
#include"SceneManager.h"
#include"SceneLoading.h"
#include"System/Audio.h"
#include"Player.h"
//������
void SceneResult::Initialize()
{
	//�X�v���C�g������
	sprite = new Sprite("Data/Sprite/result.png");

	BGMresult = Audio::Instance().LoadAudioSource("Data/Sound/BGM_main - .wav");

	for (int i = 0; i <= 3; i++)
	{
		if (highscore[i] < Player::Instance().HighScore())
		{
			highscore[i] = Player::Instance().HighScore();
		}
	}
}

//�I����
void SceneResult::Finalize()
{
	//�X�v���C�g�I����
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

//�X�V����
void SceneResult::Update(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	//�����{�^������������Q�[���V�[���ւ̐؂�ւ�
	const GamePadButton anyButton = GamePad::BTN_X;
	if (gamePad.GetButtonDown() & anyButton)
	{
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
	}
	BGMresult->Play(false);
}

//�`�揈��
void SceneResult::Render()
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
void SceneResult::DrawGUI()
{

}