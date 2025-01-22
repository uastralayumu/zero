#pragma once

#include"System/ModelRenderer.h"
#include"Character.h"

//エネミー
class Enemy : public Character
{
public:
	Enemy() {};
	~Enemy() override {};

	//更新処理
	virtual void Update(float elapsedTime) = 0;

	//破棄
	void Destroy();

	//描画処理
	virtual void Render(const RenderContext& rc, ModelRenderer* rendrer) = 0;

	//スコア
	virtual int GetScore() = 0;

	//コンボ
	virtual bool GetCombo() = 0;
};