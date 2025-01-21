#pragma once
#pragma once

#include<vector>
#include<set>
#include"mato.h"

//エネミーマネージャー
class matoManager
{
private:
	matoManager() {};
	~matoManager() {};
public:
	//唯一のインスタンス取得
	static matoManager& Instance()
	{
		static matoManager instance;
		return instance;
	}

	//エネミー全削除
	void Clear();

	//エネミー削除
	void Remove(mato* enemy);

	//エネミー登録
	void Register(mato* enemy);

	//更新処理
	void Update(float elapsedTime);

	//描画処理
	void Render(const RenderContext& rc, ModelRenderer* renderer);

	//デバッグプリミティブ描画
	void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer);

	//エネミー数取得
	int GetEnemyCount() const { return static_cast<int>(enemies.size()); }

	//エネミー取得
	mato* GetEnemy(int index) { return enemies.at(index); }

private:
	//エネミー同士の衝突処理
	void CollisionEnemyVsEnemies();

	std::vector<mato*> enemies;
	std::set<mato*> removes;

};