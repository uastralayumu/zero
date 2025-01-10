#pragma once
#include<vector>
#include<set>
#include"object.h"

//エネミーマネージャー
class objectManager
{
private:
	objectManager() {};
	~objectManager() {};
public:
	//唯一のインスタンス取得
	static objectManager& Instance()
	{
		static objectManager instance;
		return instance;
	}

	//エネミー全削除
	void Clear();

	//エネミー削除
	void Remove(object* enemy);

	//エネミー登録
	void Register(object* enemy);

	//更新処理
	void Update(float elapsedTime);

	//描画処理
	void Render(const RenderContext& rc, ModelRenderer* renderer);

	//デバッグプリミティブ描画
	void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer);

	//エネミー数取得
	int GetEnemyCount() const { return static_cast<int>(enemies.size()); }

	//エネミー取得
	object* GetEnemy(int index) { return enemies.at(index); }

private:
	//エネミー同士の衝突処理
	void CollisionEnemyVsEnemies();

	std::vector<object*> enemies;
	std::set<object*> removes;

};