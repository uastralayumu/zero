#include "ProjectileStraight.h"
#include"System/Input.h"
#include "Player.h"

//コンストラクタ
ProjectileStraight::ProjectileStraight(ProjectileManager* manager)
	: Projectile(manager)
{
	model = new Model("Data/Model/Sword/Sword.mdl");

	//表示サイズを調節
	scale.x = scale.y = scale.z = 3.0f;
}

//デストラクタ
ProjectileStraight::~ProjectileStraight()
{
	delete model;
}

//更新処理
void ProjectileStraight::Update(float elapsedTime)
{
	//寿命処理
	lifeTimer -= 0.01f;
	GamePad& gamePad = Input::Instance().GetGamePad();
	if (lifeTimer < 0.0f || gamePad.GetButtonDown() & GamePad::BTN_Y || position.y < 0.2f)
	{
		//自分を削除
		Destroy();
	}
	//移動 
	float speed = this->speed * elapsedTime;
	float move = Player::Instance().PlayerMover();
	speed = speed - (speed * move);
	position.x += direction.x * speed;
	position.y += direction.y * speed;
	position.z += direction.z * speed;

	//オブジェクト行列を更新
	UpdateTransform();

	//モデル行列更新
	model->UpdateTransform();
}

//描画処理
void ProjectileStraight::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	renderer->Render(rc, transform, model, ShaderId::Lambert);
}

//発射
void ProjectileStraight::Launch(const DirectX::XMFLOAT3& direction,
	const DirectX::XMFLOAT3& position)
{
	this->direction = direction;
	this->position = position;
}