#pragma once

#include <DirectXMath.h>
#include"System/ShapeRenderer.h"

//キャラクター
class Character
{
public:
	Character(){}
	virtual ~Character (){}

	//位置取得
	const DirectX::XMFLOAT3& GetPosition() const { return position; }

	//位置設定
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

	//回転取得
	const DirectX::XMFLOAT3& GetAngle() const { return angle; }

	//回転設定
	void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }

	//スケール取得
	const DirectX::XMFLOAT3& GetScale() const { return scale; }

	//スケール設定
	void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

	//行列更新処理
	void UpdateTransform();

	//衝撃を与える
	void AddImpulse(const DirectX::XMFLOAT3& impulse);

	//デバッグプリミティブ描画
	virtual void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer);

	//高さ取得
	float GetHeight() const { return height; }

	//半径取得
	float GetRadius() const { return radius; }

	bool IsGround() const { return isGround; }

	//ダメージを与える
	bool ApplyDamage(int damage, float invincibleTime);

private:
	//垂直速力更新処理
	void UpdateVerticalVelocity(float elapsedTime);

	//垂直移動更新処理
	void UpdateVerticalMove(float elapsedTime);

	//水平速力更新処理
	void UpdateHorizontalVelocity(float elasedTime);

	//水平移動更新処理
	void UpdateHorizontalMove(float elapsedTime);

protected:
	//移動処理
	void Move(float elapsedTime, float vx, float vz, float speed);

	
	//旋回処理
	void Turn(float elapsedTime, float vx, float vz, float speed);
	//ジャンプ処理
	void Jump(float speed);

	//速力処理更新
	void UpdateVelocity(float elapsedTime);

	//無敵時間更新
	void UpdateInvincibleTimer(float elapsedTime);

	virtual void OnLanding(){}

	//ダメージを受けた時に呼ばれる
	virtual void OnDamaged() {}

	//死亡したときに呼ばれる
	virtual void OnDead() {}

protected:
	DirectX::XMFLOAT3      position = { 0,0,0 };
	DirectX::XMFLOAT3      angle = { 0,0,0 };
	DirectX::XMFLOAT3      scale = { 1,1,1 };
	DirectX::XMFLOAT4X4    transform = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};

	float radius = 0.5f;
	float gravity = 0.0f;
	float height = 2.0f;
	float invincibleTimer;
	float friction = 15.0f;
	int health = 1;
	float accleration = 50.0f;
	float maxMoveSpeed = 5.0f;
	float moveVecX = 0.0f;
	float moveVecZ = 0.0f;
	float airControl = 0.3f;
	DirectX::XMFLOAT3 velocity = { 0,0,0 };
	bool isGround = false;
	float move = 0.03f;

};