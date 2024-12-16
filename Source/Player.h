#pragma once

#include"System/ModelRenderer.h"
#include"Character.h"
#include "ProjectileManager.h"
#include "Effect.h"
#include"System/Audio.h"

//プレイヤー
class Player : public Character
{
private:
	Player() {};
	~Player() override {};
public:
	//インスタンス取得
	static Player& Instance()
	{
		static Player instance;
		return instance;
	}

	//初期化
	void Initialize();

	//終了化
	void Finalize();

	//更新処理
	void Update(float elapsedTime);
	//描画処理
	void Render(const RenderContext& rc, ModelRenderer* renderer);
	//デバッグ用GUI描画
	void DrawDebugGUI();
	//デバッグプリミティブ描画
	void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer);

protected:
	//着地した時に呼ばれる
	void OnLanding() override;

private:
	//スティック入力値から移動ベクトルを取得
	DirectX::XMFLOAT3 GetMoveVec() const;

	//移動入力処理
	void InputMove(float elapsedTime);

	//ジャンプ入力処理
	void InputJump();

	//プライヤーとエネミーとの衝突処理
	void CollisionPlayerVsEnemies();

	//弾丸入力処理
	void InputProjectile();

	//弾丸と敵の衝突処理
	void CollisitionProjectilesVsEnemies();

private:
	Model* model = nullptr;
	float moveSpeed = 5.0f;
	float turnSpeed = DirectX::XMConvertToRadians(720);
	float jumpSpeed = 12.0f;
	int jumpCount = 0;
	int jumpLimit = 2;
	ProjectileManager projectileManager;
	Effect*  hitEffect = nullptr;
	AudioSource* hitSE = nullptr;
};