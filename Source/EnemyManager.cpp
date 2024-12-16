#include "EnemyManager.h"
#include "Collision.h"

//エネミー全削除
void EnemyManager::Clear()
{
	for (Enemy* enemy : enemies)
	{
		delete enemy;
	}
	enemies.clear();
}

//エネミー削除
void EnemyManager::Remove(Enemy* enemy)
{
	//破棄リストに追加
	removes.insert(enemy);
}

//エネミー登録
void EnemyManager::Register(Enemy* enemy)
{
	enemies.emplace_back(enemy);
}

//更新処理
void EnemyManager::Update(float elapsedTime)
{
	for (Enemy* enemy : enemies)
	{
		enemy->Update(elapsedTime);
	}
	//破棄処理
	//※enemiesの範囲for文中でerase()すると不具合が発生してしまうため、
	// 更新処理が終わった後に破棄リストに積まれたオブジェクトを削除する。
	for (Enemy* enemy : removes)
	{
		std::vector<Enemy*>::iterator it = std::find(enemies.begin(),
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

void EnemyManager::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	for (Enemy* enemy : enemies)
	{
		enemy->Render(rc, renderer);
	}
}

//デバッグプリミティブ描画
void EnemyManager::RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)
{
	for (Enemy* enemy : enemies)
	{
		enemy->RenderDebugPrimitive(rc, renderer);
	}
}

//エネミー同士の衝突処理
void EnemyManager::CollisionEnemyVsEnemies()
{
	size_t enemyCount = enemies.size();
	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemy1 = enemies.at(i);
		for (int j = i + 1;j <  enemyCount; ++j)
		{	
			Enemy* enemy2 = enemies.at(j);
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