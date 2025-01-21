#include "matoManager.h"
#include "Collision.h"

//エネミー全削除
void matoManager::Clear()
{
	for (mato* enemy : enemies)
	{
		delete enemy;
	}
	enemies.clear();
}

//エネミー削除
void matoManager::Remove(mato* enemy)
{
	//破棄リストに追加
	removes.insert(enemy);
}

//エネミー登録
void matoManager::Register(mato* enemy)
{
	enemies.emplace_back(enemy);
}

//更新処理
void matoManager::Update(float elapsedTime)
{
	for (mato* enemy : enemies)
	{
		enemy->Update(elapsedTime);
	}
	//破棄処理
	//※enemiesの範囲for文中でerase()すると不具合が発生してしまうため、
	// 更新処理が終わった後に破棄リストに積まれたオブジェクトを削除する。
	for (mato* enemy : removes)
	{
		std::vector<mato*>::iterator it = std::find(enemies.begin(),
			enemies.end(), enemy);
		if (it != enemies.end())
		{
			enemies.erase(it);
		}
		delete enemy;
	}
	//破棄リストをクリア
	removes.clear();

	//敵同士の衝突処理
	CollisionEnemyVsEnemies();
}

void matoManager::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	for (mato* enemy : enemies)
	{
		enemy->Render(rc, renderer);
	}
}

//デバッグプリミティブ描画
void matoManager::RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)
{
	for (mato* enemy : enemies)
	{
		enemy->RenderDebugPrimitive(rc, renderer);
	}
}

//エネミー同士の衝突処理
void matoManager::CollisionEnemyVsEnemies()
{
	size_t enemyCount = enemies.size();
	for (int i = 0; i < enemyCount; ++i)
	{
		mato* enemy1 = enemies.at(i);
		for (int j = i + 1; j < enemyCount; ++j)
		{
			mato* enemy2 = enemies.at(j);
			DirectX::XMFLOAT3 outPosition;
			if (Collision::IntersectCylinderVsCylinder(
				enemy1->GetPosition(),
				enemy1->GetRadius(),
				enemy1->GetHeight(),
				enemy2->GetPosition(),
				enemy2->GetRadius(),
				enemy2->GetHeight(),
				outPosition))
			{
				enemy2->SetPosition(outPosition);
			}
		}
	}
}