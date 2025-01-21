#include "Stage.h"

//コンストラクタ
Stage::Stage()
{
	//ステージモデルを読み込み
	model = new Model("Data/Model/Stage/stage.mdl");
}

Stage::~Stage()
{
	//ステージモデルを破壊
	delete model;
}

//更新処理
void Stage::Update(float elapsedTime)
{
	//今は特にやることはない
}

//描画処理
void Stage::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	DirectX::XMFLOAT4X4 transform;
	DirectX::XMStoreFloat4x4(&transform, DirectX::XMMatrixIdentity());
	
	//レンダラにモデルを描画してもらう
	renderer->Render(rc, transform, model, ShaderId::Lambert);
}