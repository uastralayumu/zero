#pragma once

#include "system/ModelRenderer.h"

//ステージ
class Stage
{
public:
	Stage();
	~Stage();

	//更新処理
	void Update(float elapsedTime);

	//描画処理
	void Render(const RenderContext& rc, ModelRenderer* rendere);

private:
	Model* model = nullptr;
};