#include "System/Graphics.h"
#include "SceneGame.h"
#include "SceneManager.h"
#include"SceneLoading.h"
#include"SceneTitle.h"
#include"Camera.h"
#include"EnemyManager.h"
#include"EnemySlime.h"
#include"Player.h"
#include"EffectManager.h"
#include"objectManager.h"
#include"objectEnemy.h"
#include"SceneResult.h"
#include"System/Audio.h"

// ������
void SceneGame::Initialize()
{
	//�X�e�[�W������
	stage = new Stage();

	sprite = new Sprite("Data/Sprite/aim.png");

	dangan = new Sprite("Data/Sprite/UI_tama.png");

	distance = new Sprite("Data/Sprite/UI_position.png");

	presentlocation = new Sprite("Data/Sprite/UI_point.png");

	BGMmain = Audio::Instance().LoadAudioSource("Data/Sound/BGM_main - .wav");

	//�v���C���[������
	Player::Instance().Initialize();

	//�J���������ݒ�
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 10, -10),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 0)
	);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f,
		1000.0f
	);
	//�J�����R���g���[���[�����ݒ�
	cameraController = new CameraController();

	//�G�l�~�[������
	EnemyManager& enemyManager = EnemyManager::Instance();
	/*for (int i = 0; i < 2; ++i)
	{
		EnemySlime* slime = new EnemySlime();
		slime->SetPosition(DirectX::XMFLOAT3(i * 2.0f, 0, 5));
		slime->SetTerritory(slime->GetPosition(), 10.0f);
		enemyManager.Register(slime);
	}*/
	/*objectManager& objectManager = objectManager::Instance();*/
	for (int i = 0; i < 2; ++i)
	{
		objectEnemy* object = new objectEnemy();
		object->SetPosition(DirectX::XMFLOAT3(i * 5.0f, 0, 10));
		object->SetTerritory(object->GetPosition(), 10.0f);
		enemyManager.Register(object);
	}
}

// �I����
void SceneGame::Finalize()
{
	//�G�l�~�[�I����
	EnemyManager::Instance().Clear();
	//�G�l�~�[�I����
	/*objectManager::Instance().Clear();*/
	//�J�����R���g���[���[�I����
	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}
	//�X�e�[�W�I����
	if (stage != nullptr)
	{
		delete stage;
		stage = nullptr;
	}
	////�I�u�W�F�N�g�I����
	//if (object != nullptr)
	//{
	//	delete object;
	//	object = nullptr;
	//}

	if (sprite != nullptr)
	{
		delete sprite;
		sprite = nullptr;
	}

	if (dangan != nullptr)
	{
		delete dangan;
		dangan = nullptr;
	}

	if (distance != nullptr)
	{
		delete distance;
		distance = nullptr;
	}

	if (presentlocation != nullptr)
	{
		delete presentlocation;
		presentlocation = nullptr;
	}

	if (BGMmain != nullptr)
	{
		delete BGMmain;
		BGMmain = nullptr;
	}

	//�v���C���[�I����
	Player::Instance().Finalize();

}

// �X�V����
void SceneGame::Update(float elapsedTime)
{
	//�J�����R���g���[���[�X�V����
	DirectX::XMFLOAT3 target = Player::Instance().GetPosition();
	target.y += 1.7f;
	target.z += 10.5f;
	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);
	//�X�e�[�W�X�V����
	stage->Update(elapsedTime);
	////�I�u�W�F�N�g
	//object->Update(elapsedTime);
	//�v���C���[�X�V����
	Player::Instance().Update(elapsedTime);
	//�G�l�~�[�X�V����
	EnemyManager::Instance().Update(elapsedTime);
	////�I�u�W�F�N�g�X�V����
	//objectManager::Instance().Update(elapsedTime);
	//�G�t�F�N�g�X�V����
	EffectManager::Instance().Update(elapsedTime);

	BGMmain->Play(false);

	//�S�[���ɂ�����Q�[���V�[���̐؂�ւ�
	if (Player::Instance().PlayerPositionZ() < -362)
	{
		SceneManager::Instance().ChangeScene(new SceneResult);
	}
}

// �`�揈��
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ShapeRenderer* shapeRenderer = graphics.GetShapeRenderer();
	ModelRenderer* modelRenderer = graphics.GetModelRenderer();

	//�`�揈��
	RenderContext rc;
	rc.deviceContext = dc;
	rc.renderState = graphics.GetRenderState();
	rc.lightDirection = { 0.0f,-1.0f,0.0f };

	//�J�����p�����[�^�ݒ�
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();
	
	// 3D���f���`��
	{
		//�X�e�[�W�`��
		stage->Render(rc, modelRenderer);
		////�I�u�W�F�N�g
		
		//�v���C���[�`��
		Player::Instance().Render(rc, modelRenderer);
		//�G�l�~�[�`��
		EnemyManager::Instance().Render(rc, modelRenderer);
		//�I�u�W�F�N�g�`��
		/*objectManager::Instance().Render(rc, modelRenderer);*/

		//�G�t�F�N�g�`��
		EffectManager::Instance().Render(rc.view, rc.projection);
	}

	// 3D�f�o�b�O�`��
	{
		////�v���C���[�f�o�b�O�v���~�e�B�u�`��
		//Player::Instance().RenderDebugPrimitive(rc, shapeRenderer);

		////�G�l�~�[�f�o�b�O�v���~�e�B�u�`��
		//EnemyManager::Instance().RenderDebugPrimitive(rc, shapeRenderer);
		////�I�u�W�F�N�g�f�o�b�O�v���~�e�B�u�`��
		//objectManager::Instance().RenderDebugPrimitive(rc, shapeRenderer);
	}

	// 2D�X�v���C�g�`��
	{
		float screenWindth = 220.0f;
		float screenHeight = 220.0f;
		float mouseposx = Player::Instance().Mouseposx();
		float mouseposy = Player::Instance().Mouseposy();
		float danganCount = Player::Instance().DanganCount();
		int maxdanganCount = Player::Instance().MaxDanganCount();
		float playerpositionz = Player::Instance().PlayerPositionZ();
		playerpositionz = ((playerpositionz / -363) * 1910) - 32;
		distance->Render(rc,
			0, 0, 0, 1910,1080,
			0,
			1, 1, 1, 0.5f);
		presentlocation->Render(rc,
			playerpositionz, 50, 0, 64, 32,
			0,
			1, 1, 1, 1);
		sprite->Render(rc,
			mouseposx - screenWindth * 0.5f, mouseposy - screenHeight * 0.5f, 0, screenWindth, screenHeight,
			0,
			1, 1, 1, 1);
		for (int i = 0; i < maxdanganCount - danganCount; i++)
		{
			dangan->Render(rc,
				1840, 100 + (i * 50), 0, 60, 60,
				0,
				1, 1, 1, 1);
		}
	}
}

// GUI�`��
void SceneGame::DrawGUI()
{
	//�v���C���[�f�o�b�O�`��
	Player::Instance().DrawDebugGUI();
}
