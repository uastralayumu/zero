#include "System/Graphics.h"
#include"SceneTitle.h"
#include"System/Input.h"
#include"SceneGame.h"
#include"SceneManager.h"
#include"SceneLoading.h"

//������
void SceneTitle::Initialize()
{
	//�X�v���C�g������
	sprite = new Sprite("Data/Sprite/Title.png");
}

//�I����
void SceneTitle::Finalize()
{
	//�X�v���C�g�I����
	if (sprite != nullptr)
	{
		delete sprite;
		sprite = nullptr;
	}
}

//�X�V����
void SceneTitle::Update(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	//�����{�^������������Q�[���V�[���ւ̐؂�ւ�
	const GamePadButton anyButton =
		GamePad::BTN_A
		| GamePad::BTN_B
		| GamePad::BTN_X
		| GamePad::BTN_Y
		;
	if (gamePad.GetButtonDown() & anyButton)
	{
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
	}
}

//�`�揈��
void SceneTitle::Render()
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
void SceneTitle::DrawGUI()
{

}