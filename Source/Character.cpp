#include"Character.h"


//行列更新処理
void Character::UpdateTransform()
{
	//スケール行列を作成
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x,scale.y,scale.z);
	//回転行列を作成
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x,angle.y,angle.z);
	//位置行列を作成
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x,position.y,position.z);
	//3つの行列を組み合わせ、ワールド行列を作成
	DirectX::XMMATRIX W = S * R * T;
	//計算したワールド行列を取り出す
	DirectX::XMStoreFloat4x4(&transform, W);
}

void Character::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
	//速力に力を加える
	velocity.x += impulse.x;
	velocity.y += impulse.y;
	velocity.z += impulse.z;
}

//デバッグプリミティブ描画
void Character::RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)
{
	//衝突判定用のデバッグ円柱を描画
	renderer->RenderCylinder(rc, position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
}

//ダメージを与える
bool Character::ApplyDamage(int damage,float invincibleTime)
{
	//ダメージが0の場合は健康状態を変更する必要がない
	if (damage == 0)return false;

	//死亡している場合は健康状態を変更しない
	if (health <= 0)return false;

	//無敵時間中はダメージを与えない
	if (invincibleTimer > 0)return false;

	//無敵時間設定
	invincibleTimer = 0.5f;

	//ダメージ処理
	health -= damage;

	//死亡通知
	if (health <= 0)
	{
		OnDead();
	}
	//ダメージ通知
	else
	{
		OnDamaged();
	}

	//健康状態が変更した場合はtrueを返す
	return true;
}

//移動処理
void Character::Move(float elapsedTime, float vx, float vz, float speed)
{

	//移動方向ベクトルを設定
	/*moveVecX = vx;
	moveVecZ = vz;*/
	position.z -= move;

	//最大速度設定
	maxMoveSpeed = speed;
}

//旋回処理
void Character::Turn(float elapsedTime, float vx, float vz, float speed)
{
	speed *= elapsedTime;

	//進行ベクトルがゼロベクトルの場合は処理する必要なし
	float lenght = sqrtf(vx * vx + vz * vz);
	if (lenght < 0.01f)return;

	//進行ベクトルを単位ベクトル化
	vx /= lenght;
	vz /= lenght;

	//自身の加点力前方向を求める
	float frontX = sinf(angle.y);
	float frontZ = cosf(angle.y);

	//回転角を求めるため、2つの単位ベクトルの内積を計算する
	float dot = (frontX * vx) + (frontZ * vz);

	//内積値は-1.0〜1.0で表現されており、2の単位ベクトルの内積を計算する
	//小さいほど1.0に近づくという性質を利用して回転速度を調節する
	float rot = 1.0f - dot;
	if (rot > speed) rot = speed;

	//左右判定を行うために２つの単位ベクトルの外積を計算する
	float cross = (frontZ * vx) - (frontX * vz);

	//2Dの外積値が正の場合か負の場合かによって左右判定が行える
	//左右判定を行うことによって左右回転を選択する
	if (cross < 0.0f)
	{
		angle.y -= rot;
	}
	else
	{
		angle.y += rot;
	}
}

//ジャンプ処理
void Character::Jump(float speed)
{
	//上方向の力を設定
	velocity.y = speed;
}

//速力処理更新
void Character::UpdateVelocity(float elapsedTime)
{
	//垂直速力更新処理
	UpdateVerticalVelocity(elapsedTime);

	//水平速力更新処理
	UpdateHorizontalVelocity(elapsedTime);

	//垂直移動更新処理
	UpdateVerticalMove(elapsedTime);

	//水平移動更新処理
	UpdateHorizontalMove(elapsedTime);
}

//垂直速力更新処理
void Character::UpdateVerticalVelocity(float elapsedTime)
{
	//重力処理
	velocity.y += gravity * elapsedTime;
}

//垂直移動更新処理
void Character::UpdateVerticalMove(float elapsedTime)
{
	//移動処理
	position.y += velocity.y * elapsedTime;

	//地面判定
	if (position.y < 0.0f)
	{
		position.y = 0.0f;
		velocity.y = 0.0f;

		//着地した
		if (!isGround)
		{
			OnLanding();
		}
		isGround = true;
	}
	else
	{
		isGround = false;
	}
}

//水平速力更新処理
void Character::UpdateHorizontalVelocity(float elapsedTime)
{
	//XZ平面の速力を減速する
	float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
		if (length > 0.0f)
		{
			//摩擦力
			float friction = this->friction * elapsedTime;

			//空中にいるときは摩擦力を減らす
			if (!isGround)friction *= airControl;
			
			//摩擦による横方向の減速処理
			if (length > friction)
			{
				float vx = velocity.x / length;
				float vz = velocity.z / length;

				velocity.x -= vx * friction;
				velocity.z -= vz * friction;
			}
			//横方向の速力が摩擦力以下になったので速力を無効化
			else
			{
				velocity.x = 0.0f;
				velocity.z = 0.0f;
			}

		}

		//XZ平面の速力を加速する
		if (length <= maxMoveSpeed)
		{
			//移動ベクトルがゼロベクトルでないなら加速する
			float moveVecLength = sqrtf(moveVecX * moveVecX + moveVecZ * moveVecZ);
			if (moveVecLength > 0.0f)
			{
				//加速力
				float acceleration = this->accleration * elapsedTime;
				//空中にいるときは摩擦力を減らす
				if (!isGround) acceleration *= airControl;

				//移動ベクトルによる加速処理
				velocity.x += moveVecX * acceleration;
				velocity.z += moveVecZ * acceleration;
				
				//最大速度制限
				float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
				if (length > maxMoveSpeed)
				{
					float vx = velocity.x / length;
					float vz = velocity.z / length;

					velocity.x = vx * maxMoveSpeed;
					velocity.z = vz * maxMoveSpeed;
				}
			}
		}

		//移動ベクトルをリセット
		moveVecX = 0.0f;
		moveVecZ = 0.0f;
}

//水平移動更新処理
void Character::UpdateHorizontalMove(float elapsedTime)
{
	//移動処理
	position.x += velocity.x * elapsedTime;
	position.z += velocity.z * elapsedTime;
}


//無敵時間更新
void Character::UpdateInvincibleTimer(float elapsedTime)
{
	if (invincibleTimer > 0.0f)
	{
		invincibleTimer -= elapsedTime;
	}
}