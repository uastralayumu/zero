#include<imgui.h>
#include"System/Input.h"
#include "Player.h"
#include "System/Input.h"
#include "Camera.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "ProjectileStraight.h"
#include "ProjectileHoming.h"
#include "System/Graphics.h"
#include "objectManager.h"

//初期化
void Player::Initialize()
{	
	//モデルが大きいのでスケーリング
	scale.x = scale.y = scale.z = 0.01f;

	//ヒットエフェクト読み込み
	hitEffect = new Effect("Data/Effect/Hit.efk");

	//ヒットSE読み込み
	hitSE = Audio::Instance().LoadAudioSource("Data/Sound/Hit.wav");

	danganCount = 0;
	position = { 0,0,0 };
	angle = { 0,0,0 };
	score = 0;
	combo = 0;
}

//終了化
void Player::Finalize()
{
	delete hitSE;
	delete hitEffect;
	delete model;
}

//更新処理
void Player::Update(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	//移動入力処理
	InputMove(elapsedTime);
	//オブジェクト行列を更新
	UpdateTransform();
	//弾丸入力処理
	InputProjectile();
	//速力処理更新
	UpdateVelocity(elapsedTime);
	//弾丸更新処理
	projectileManager.Update(elapsedTime);
	//弾丸と敵の衝突判定
	CollisitionProjectilesVsEnemies();

	if (danganCount != maxdanganCount)
	{
		if (gamePad.GetButton() & GamePad::BTN_X)
		{
			move = 0.03f;
			suscore += elapsedTime;
		}
		else move = 0.05f;

		if (suscore > 1)
		{
			suscore -= 1;
			score -= 1;
		}

		if (position.z <= -363)
		{
			move = 0;
			position.z = -363;
		}
	}
	else
	{
		move = 0.3f;
	}
	position.z -= move;
}

//描画処理
void Player::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	//弾丸描画処理
	projectileManager.Render(rc, renderer);
}

//デバッグ用GUI描画
void Player::DrawDebugGUI()
{
	ImVec2 pos = ImGui::GetMainViewport()->GetWorkPos();
	ImGui::SetNextWindowPos(ImVec2(pos.x + 10, pos.y + 10), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("Player,nullptr,ImGuiWindowFlags_None"))
	{
		//トランスフォーム
		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			//位置
			ImGui::InputFloat3("Positon", &position.x);
			//回転
			DirectX::XMFLOAT3 a;
			a.x = DirectX::XMConvertToDegrees(angle.x);
			a.y = DirectX::XMConvertToDegrees(angle.y);
			a.z = DirectX::XMConvertToDegrees(angle.z);
			ImGui::InputFloat3("Angle", &a.x);
			angle.x = DirectX::XMConvertToRadians(a.x);
			angle.y = DirectX::XMConvertToRadians(a.y);
			angle.z = DirectX::XMConvertToRadians(a.z);
			//スケール
			ImGui::InputFloat3("Scale", &scale.x);
		}
	}
	ImGui::End();
}

void Player::OnLanding()
{
	jumpCount = 0;
}

DirectX::XMFLOAT3 Player::GetMoveVec() const
{
	//入力情報を取得
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisLX();
	float ay = gamePad.GetAxisLY();

	//カメラ方向とスティックの入力値によって進行方向を計算する
	Camera& camera = Camera::Instance();
	const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
	const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

	//移動ベクトルはXZ平面に水平なベクトルになるようにする

	//カメラ右方向ベクトルをXZ単位ベクトルに変換
	float cameraRightX = cameraRight.x;
	float cameraRightZ = cameraRight.z;
	float cameraRightLenght = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
	if (cameraRightLenght > 0.0f)
	{
		//単位ベクトル化
		cameraRightX /= cameraRightLenght;
		cameraRightZ /= cameraRightLenght;
	}

	//カメラ前歩行ベクトルをXZ単位ベクトルに変換
	float cameraFrontX = cameraFront.x;
	float cameraFrontZ = cameraFront.z;
	float cameraFrontLenght = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
	if (cameraFrontLenght > 0.0f)
	{
		//単位ベクトル化
		cameraFrontX /= cameraFrontLenght;
		cameraFrontZ /= cameraFrontLenght;
	}
	//スティックの水平入力値をカメラ右方向に反映し、
	//スティックの垂直入力値をカメラ前方向に反映し、
	//進行ベクトルを計算する
	DirectX::XMFLOAT3 vec;
	vec.x = (cameraRightX * ax) + (cameraFrontX * ax);
	vec.z = (cameraRightZ * ay) + (cameraFrontZ * ay);
	//Y軸方向に移動しない
	vec.y = 0.0f;
	return vec;
}

//移動入力処理
void Player::InputMove(float elapsedTime)
{
	//進行ベクトル取得
	DirectX::XMFLOAT3 moveVec = GetMoveVec();

	//移動処理
	Move(elapsedTime, moveVec.x, moveVec.z, moveSpeed);

	//旋回処理
	Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);
}

//ジャンプ入力処理
void Player::InputJump()
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	if (gamePad.GetButtonDown() & GamePad::BTN_A)
	{
		if (jumpCount < jumpLimit)
		{
			jumpCount++;
			Jump(jumpSpeed);
		}
		
	}
}


//プレイヤーとエネミーとの衝突処理
void Player::CollisionPlayerVsEnemies()
{
	EnemyManager& enemyManager = EnemyManager::Instance();

	//すべての敵と総当たりで衝突処理
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);

		//衝突処理
		DirectX::XMFLOAT3 outPosition;
		if (Collision::IntersectCylinderVsCylinder(
			position,radius,height,
			enemy->GetPosition(),
			enemy->GetRadius() ,
			enemy->GetHeight(),
			outPosition))
		{
			//押し出し後の位置設定
			DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
			DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
			DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P, E);
			DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
			DirectX::XMFLOAT3 normal;
			DirectX::XMStoreFloat3(&normal, N);
			//上から踏んづけた場合は小ジャンプする
			if(normal.y > 0.8f)
			{
				Jump(jumpSpeed * 0.5f);
			}
			else
			{
				enemy->SetPosition(outPosition);
			}
		}
	}

}

//デバッグプリミティブ描画
void Player::RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)
{
	//基底クラスの関数呼び出し
	Character::RenderDebugPrimitive(rc, renderer);

	//弾丸デバッグプリミティブ描画
	projectileManager.RenderDebugPrimitive(rc, renderer);
}

//弾丸入力処理
void Player::InputProjectile()
{
	Mouse& mouse = Input::Instance().GetMouse();
	mouseposx = mouse.GetPositionX();
	mouseposy = mouse.GetPositionY();	
	Graphics& graphics = Graphics::Instance();
	float screenWidth = static_cast<float>(graphics.GetScreenWidth());
	float screeHeight = static_cast<float>(graphics.GetScreenHeight());
	float mousedirx = ((mouseposx * 2 / screenWidth) - 1) * 0.65f;
	float mousediry = ((mouseposy * 2 / screeHeight) - 1) * 0.35f;
	Camera& camera = Camera::Instance();
	DirectX::XMFLOAT3 camerapos = camera.GetEye();
	DirectX::XMFLOAT3 cameradir = camera.GetFront();

	//直進弾丸発射
	if (mouse.GetButtonDown() & Mouse::BTN_LEFT && danganCount < maxdanganCount)
	{
		//前方向
		DirectX::XMFLOAT3 dir;
		dir.x = sinf(cameradir.x) + mousedirx;
		dir.y = sinf(cameradir.y) - mousediry;
		dir.z = sinf(cameradir.z);
		//発射位置（プレイヤーの腰当たり）
		DirectX::XMFLOAT3 pos;
		pos.x = camerapos.x;
		pos.y = camerapos.y + 0.01;
		pos.z = camerapos.z;
		//発射
		ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
		projectile->Launch(dir, pos);
		danganCount++;
	}
	
}



void  Player::CollisitionProjectilesVsEnemies()
{
	EnemyManager& enemyManager = EnemyManager::Instance();

	//すべての弾丸とすべての敵を総当たりで衝突処理
	int projectileCount = projectileManager.GetProjectileCoust();
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0; i < projectileCount; ++i)
	{
		Projectile* projectile = projectileManager.GetProjectile(i);
		for (int j = 0; j < enemyCount; ++j)
		{
			Enemy* enemy = enemyManager.GetEnemy(j);
			//衝突処理
			DirectX::XMFLOAT3 outPositon;
			if (Collision::IntersectSphereVsCylinder(
				projectile->GetPosition(),
				projectile->GetRaidus(),
				enemy->GetPosition(),
				enemy->GetRadius() *2,
				enemy->GetHeight(),
				outPositon))
			{
				//ダメージを与える
				if (enemy->ApplyDamage(1, 0.5f))
				{
					
					//ヒットエフェクト再生
					{
						DirectX::XMFLOAT3 e = enemy->GetPosition();
						e.y += enemy->GetHeight() * 0.5f;
						hitEffect->Play(e);
					}

					//ヒットSE再生
					{
						hitSE->Play(false);
					}

					{
						score += enemy->GetScore();


						if (enemy->GetCombo())
						{
							combo += 1;
						}
						else
						{
							combo = 0;
						}

						if (combo >= 2)
						{
							score += 30;
						}

					}
					//弾丸破壊
					projectile->Destroy();
				}
			}
		}
	}

}
