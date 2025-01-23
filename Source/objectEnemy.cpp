#include "objectEnemy.h"
#include "MathUtils.h"
#include "Player.h"
#include "ProjectileStraight.h"

//コンストラクタ
objectEnemy::objectEnemy()
{
	model = new Model("Data/Model/object/debris.mdl");

	//モデルが大きいのでスケーリング
	scale.x = scale.y = scale.z = 0.50f;

	//幅、高さ設定
	radius = 1.5f;
	height = 2.5f;

	//徘徊ステートへ遷移
	SetWanderState();
}

//デストラクタ
objectEnemy::~objectEnemy()
{
	delete model;
}

//更新処理
void objectEnemy::Update(float elapsedTime)
{
	////ステート毎の更新処理
	//switch (state)
	//{
	//case State::Wander:
	//	UpdateWanderState(elapsedTime);
	//	break;
	//case State::Idle:
	//	UpdateIdleState(elapsedTime);
	//	break;
	//case State::Attack:
	//	UpdateAttackState(elapsedTime);
	//	break;
	//}
	//速力処理更新
	UpdateVelocity(elapsedTime);

	////弾丸更新処理
	//projectileManager.Update(elapsedTime);

	//無敵時間更新
	UpdateInvincibleTimer(elapsedTime);

	//オブジェクト行列を更新
	//UpdateTransform();

	//スケール行列を作成
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	//回転行列を作成
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	//位置行列を作成
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y+1.6, position.z);

	//3つの行列を組み合わせ、ワールド行列を作成
	DirectX::XMMATRIX W = S * R * T;
	//計算したワールド行列を取り出す
	DirectX::XMStoreFloat4x4(&transform, W);

	//モデル行列更新
	model->UpdateTransform();
}

//描画処理
void objectEnemy::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	renderer->Render(rc, transform, model, ShaderId::Lambert);
}

//デバッグプリミティブ描画
void objectEnemy::RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)
{
	//基底クラスのデバッグプリミティブ描画
	Enemy::RenderDebugPrimitive(rc, renderer);

	//縄張り範囲をデバッグ円柱描画
	renderer->RenderCylinder(rc, territoryOrigin, territoryRange, 1.0f, DirectX::XMFLOAT4(0, -1, 0, 1));

	//ターゲット位置をデバッグ球描画
	renderer->RenderSphere(rc, targetPosition, 1.0f, DirectX::XMFLOAT4(1, 1, 0, 1));

	//索敵範囲をデバッグ円柱描画
	renderer->RenderCylinder(rc, position, searchRange, 1.0f, DirectX::XMFLOAT4(1, 0, 0, 1));
}

//縄張り設定
void objectEnemy::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
	territoryOrigin = origin;
	territoryRange = range;
}

//ターゲット位置をランダム設定
void objectEnemy::SetRandomTargetPosition()
{
	float theta = MathUtils::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
	float range = MathUtils::RandomRange(0.0f, territoryRange);
	targetPosition.x = territoryOrigin.x + sinf(theta) * range;
	targetPosition.y = territoryOrigin.y;
	targetPosition.z = territoryOrigin.z + cosf(theta) * range;
}

//目標地点へ移動
void objectEnemy::MoveToTarget(float elapsedTime, float moveSpeedRate, float turnSpeedRate)
{
	//ターゲット方向への進行ベクトルを算出
	float vx = targetPosition.x - position.x;
	float vz = targetPosition.z - position.z;
	float dist = sqrtf(vx * vx + vz * vz);
	vx /= dist;
	vz /= dist;

	//移動処理
	//Move(elapsedTime, vx, vz, moveSpeed * moveSpeedRate);
	/*Turn(elapsedTime, vx, vz, turnSpeed * turnSpeedRate);*/
}

//徘徊ステートへ遷移
void objectEnemy::SetWanderState()
{
	state = State::Wander;


	//目標地点設定
	SetRandomTargetPosition();

}

//徘徊ステート更新処理
void objectEnemy::UpdateWanderState(float elapsedTime)
{
	//目標地点までXZ平面での距離判定
	float vx = targetPosition.x - position.x;
	float vz = targetPosition.z - position.z;
	float distSq = vx * vx + vz * vz;
	if (distSq < radius * radius)
	{
		//待機ステートへ遷移
		SetIdleState();
	}

	//目標地点へ移動
	MoveToTarget(elapsedTime, 1.0f, 1.0f);

	//プレイヤー索敵
	if (SearchPlayer())
	{
		//見つかったら攻撃ステートへ遷移
		SetAttackState();
	}
}

//死亡した時に呼ばれる
void objectEnemy::OnDead()
{
	//自信を破棄
	Destroy();
}

//プレイヤー索敵
bool objectEnemy::SearchPlayer()
{
	//プレイヤーとの高低差を考慮して3Dでの距離判定をする
	const DirectX::XMFLOAT3& playerPosition = Player::Instance().GetPosition();
	float vx = playerPosition.x - position.x;
	float vy = playerPosition.y - position.y;
	float vz = playerPosition.z - position.z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);
	if (dist < searchRange)
	{
		float distXZ = sqrtf(vx * vx + vz * vz);
		//単位ベクトル化
		vx /= distXZ;
		vz /= distXZ;
		//前方ベクトル
		float frontX = sinf(angle.y);
		float frontZ = cosf(angle.y);
		//2つのベクトルの内積値で前後判定
		float dot = (frontX * vx) + (frontZ * vz);
		if (dot > 0.0f)
		{
			return true;
		}
	}
	return false;
}


//待機ステートへ遷移
void objectEnemy::SetIdleState()
{
	state = State::Idle;

	//タイマーをランダム設定
	stateTimer = MathUtils::RandomRange(3.0f, 5.0f);

}

//待機ステート更新処理
void objectEnemy::UpdateIdleState(float elapsedTime)
{
	//タイマー処理
	stateTimer -= elapsedTime;

	if (stateTimer < 0.0f)
	{
		//徘徊ステートへ遷移
		SetWanderState();
	}

	//プレイヤー索敵
	if (SearchPlayer())
	{
		//見つかったら攻撃ステート遷移
		SetAttackState();
	}
}

//攻撃ステート遷移
void objectEnemy::SetAttackState()
{
	state = State::Attack;

	stateTimer = 0.0f;
}

//追跡ステート更新処理
void objectEnemy::UpdateAttackState(float elapsedTime)
{
	//目標地点をプレイヤー位置に設定
	targetPosition = Player::Instance().GetPosition();

	//目標地点へ移動
	MoveToTarget(elapsedTime, 0.0f, 1.0f);

	//タイマー処理
	stateTimer -= elapsedTime;
	if (stateTimer < 0.0f)
	{
		//前方向
		DirectX::XMFLOAT3 dir;
		dir.x = sinf(angle.y);
		dir.y = 0.0f;
		dir.z = cosf(angle.y);
		//発射位置(プレイヤーの腰当たり)
		DirectX::XMFLOAT3 pos;
		pos.x = position.x;
		pos.y = position.y + height * 0.5f;
		pos.z = position.z;
		//発射
		ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
		projectile->Launch(dir, pos);

		stateTimer = 2.0f;
	}

	//プレイヤーを見失ったら
	if (!SearchPlayer())
	{
		//待機ステートへ遷移
		SetIdleState();
	}
}