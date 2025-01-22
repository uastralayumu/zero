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
#include"matoManager.h"
#include"matoEnemy.h"
#include"SceneResult.h"
#include"System/Audio.h"


// 初期化
void SceneGame::Initialize()
{
	//ステージ初期化
	stage = new Stage();

	sprite = new Sprite("Data/Sprite/aim.png");

	dangan = new Sprite("Data/Sprite/UI_tama.png");

	distance = new Sprite("Data/Sprite/UI_position.png");

	presentlocation = new Sprite("Data/Sprite/UI_point.png");

	BGMmain = Audio::Instance().LoadAudioSource("Data/Sound/BGM_main - .wav");

	//プレイヤー初期化
	Player::Instance().Initialize();

	//カメラ初期設定
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
	//カメラコントローラー初期設定
	cameraController = new CameraController();

	//エネミー初期化
	EnemyManager& enemyManager = EnemyManager::Instance();
	/*for (int i = 0; i < 2; ++i)
	{
		EnemySlime* slime = new EnemySlime();
		slime->SetPosition(DirectX::XMFLOAT3(i * 2.0f, 0, 5));
		slime->SetTerritory(slime->GetPosition(), 10.0f);
		enemyManager.Register(slime);
	}*/
	/*objectManager& objectManager = objectManager::Instance();*/


	DirectX::XMFLOAT3 p[16] = {
		{ -3, 5, -50 }
		/*{ 0, 2, -15 },
		{ 0, 1, -50 },
		{ 0, 0, 30 },
		{ 0, 3, 25 },
		{ 0, 1, 10 },
		{ 0, 2, 10 },
		{ 0, 5, 10 },*/
	};
	for (int i = 0; i < 1; ++i)
	{
		objectEnemy* object = new objectEnemy();

    //object->SetPosition(DirectX::XMFLOAT3(i * 2.0f, 5, 10));
    object->SetPosition(p[i]);

		object->SetTerritory(object->GetPosition(), 10.0f);
		enemyManager.Register(object);
	}
	
	DirectX::XMFLOAT3 m[6] = {
		{ 1, 2, -5 },
		{ 2, 1, -15 },
		{ -3, 3, -40 },
		{ 5, 3.5, -30 },
		{ 6, 1.5, -20 },
		{ 3, 1, -30 },
	};
	for (int i = 0; i < 6; ++i)
	{
		matoEnemy* mato = new matoEnemy();
		mato->SetPosition(m[i]);
		mato->SetTerritory(mato->GetPosition(), 10.0f);
		enemyManager.Register(mato);
	}
}

// 終了化
void SceneGame::Finalize()
{
	//エネミー終了化
	EnemyManager::Instance().Clear();
	//エネミー終了化
	objectManager::Instance().Clear();
	//的終了化
	matoManager::Instance().Clear();

	//カメラコントローラー終了化
	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}
	//ステージ終了化
	if (stage != nullptr)
	{
		delete stage;
		stage = nullptr;
	}
	////オブジェクト終了化
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

	//プレイヤー終了化
	Player::Instance().Finalize();

}

// 更新処理
void SceneGame::Update(float elapsedTime)
{
	//カメラコントローラー更新処理
	DirectX::XMFLOAT3 target = Player::Instance().GetPosition();
	target.y += 1.7f;
	target.z += 10.5f;
	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);
	//ステージ更新処理
	stage->Update(elapsedTime);
	////オブジェクト
	//object->Update(elapsedTime);
	//プレイヤー更新処理
	Player::Instance().Update(elapsedTime);
	//エネミー更新処理
	EnemyManager::Instance().Update(elapsedTime);
	////オブジェクト更新処理
	//objectManager::Instance().Update(elapsedTime);
	//エフェクト更新処理
	EffectManager::Instance().Update(elapsedTime);

  //的更新処理
	matoManager::Instance().Update(elapsedTime);
  
	BGMmain->Play(false);

	//ゴールについたらゲームシーンの切り替え
	if (Player::Instance().PlayerPositionZ() < -362)
	{
		SceneManager::Instance().ChangeScene(new SceneResult);
	}
}

// 描画処理
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ShapeRenderer* shapeRenderer = graphics.GetShapeRenderer();
	ModelRenderer* modelRenderer = graphics.GetModelRenderer();

	//描画処理
	RenderContext rc;
	rc.deviceContext = dc;
	rc.renderState = graphics.GetRenderState();
	rc.lightDirection = { 0.0f,-1.0f,0.0f };

	//カメラパラメータ設定
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();
	
	// 3Dモデル描画
	{
		//ステージ描画
		stage->Render(rc, modelRenderer);
		////オブジェクト
		
		//プレイヤー描画
		Player::Instance().Render(rc, modelRenderer);
		//エネミー描画
		EnemyManager::Instance().Render(rc, modelRenderer);
		//オブジェクト描画
		/*objectManager::Instance().Render(rc, modelRenderer);*/

		//エフェクト描画
		EffectManager::Instance().Render(rc.view, rc.projection);
		//的描画
		matoManager::Instance().Render(rc, modelRenderer);
	}

	// 3Dデバッグ描画l
	{
		////プレイヤーデバッグプリミティブ描画
		//Player::Instance().RenderDebugPrimitive(rc, shapeRenderer);
		////エネミーデバッグプリミティブ描画
		//EnemyManager::Instance().RenderDebugPrimitive(rc, shapeRenderer);
		////オブジェクトデバッグプリミティブ描画
		//objectManager::Instance().RenderDebugPrimitive(rc, shapeRenderer);
		////的デバッグプリミティブ描画
		//matoManager::Instance().RenderDebugPrimitive(rc, shapeRenderer);
	}

	// 2Dスプライト描画
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

// GUI描画
void SceneGame::DrawGUI()
{
	//プレイヤーデバッグ描画
	Player::Instance().DrawDebugGUI();
}
