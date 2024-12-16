#include"System/Graphics.h"
#include"System/Input.h"
#include"SceneLoading.h"
#include"SceneManager.h"

//������
void SceneLoading::Initialize()
{
	//�X�v���C�g������
	sprite = new Sprite("Data/Sprite/LoadingIcon.png");

	//�X���b�h�J�n
	thread = new std::thread(LoadingThread,this);
}

//�I����
void SceneLoading::Finalize()
{
	//�X�v���C�g�I����
	if (sprite != nullptr)
	{
		delete sprite;
		sprite = nullptr;
	}
	
	//�X���b�h�I����
	if (thread != nullptr)
	{
		thread->join();
		delete thread;
		thread = nullptr;
	}
}

//�X�V����
void SceneLoading::Update(float elapsedTime)
{
	constexpr float speed = 180;
	angle += speed * elapsedTime;

	//���̃V�[���̏���������������V�[����؂�ւ���
	if (nextScene->IsReady())
	{
		SceneManager::Instance().ChangeScene(nextScene);
		nextScene = nullptr;
	}
}

//�`�揈��
void SceneLoading::Render()
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
		//��ʉE���Ƀ��[�f�B���O�A�C�R����`��
		float screeWidth = static_cast<float>(graphics.GetScreenWidth());
		float screenHeight = static_cast<float>(graphics.GetScreenHeight());
		float spriteWidth = 256;
		float spriteHeight = 256;
		float positionX = screeWidth - spriteWidth;
		float positionY = screenHeight - spriteHeight;

		sprite->Render(rc,
			positionX, positionY, 0, spriteWidth, spriteHeight,
			angle,
			1, 1, 1, 1);
	}
}

//GUI�`��
void SceneLoading::DrawGUI()
{

}

//���[�f�B���O�X���b�h
void SceneLoading::LoadingThread(SceneLoading* scene)
{
	//COM�֘A�̏������ŃX���b�h���ɌĂԕK�v������
	CoInitialize(nullptr);

	//���̃V�[���̏��������s��
	scene->nextScene->Initialize();

	//�X���b�h���I���O��COM�֘A�̏I����
	CoUninitialize();

	//���̃V�[���̏��������ݒ�
	scene->nextScene->SetReady();
}