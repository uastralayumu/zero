#include "objectManager.h"
#include "Collision.h"

//エネミー全削除
void objectManager::Clear()
{
	for (object* enemy : enemies)
	{
		delete enemy;
	}
	enemies.clear();
}

//エネミー削除
void objectManager::Remove(object* enemy)
{
	//破棄リストに追加
	removes.insert(enemy);
}

//エネミー登録
void objectManager::Register(object* enemy)
{
	enemies.emplace_back(enemy);
}

//更新処理
void objectManager::Update(float elapsedTime)
{
	for (object* enemy : enemies)
	{
		enemy->Update(elapsedTime);
	}
	//破棄処理
	//※enemiesの範囲for文中でerase()すると不具合が発生してしまうため、
	// 更新処理が終わった後に破棄リストに積まれたオブジェクトを削除する。
	for (object* enemy : removes)
	{
		std::vector<object*>::iterator it = std::find(enemies.begin(),
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

void objectManager::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	for (object* enemy : enemies)
	{
		enemy->Render(rc, renderer);
	}
}

//デバッグプリミティブ描画
void objectManager::RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)
{
	for (object* enemy : enemies)
	{
		enemy->RenderDebugPrimitive(rc, renderer);
	}
}

//エネミー同士の衝突処理
void objectManager::CollisionEnemyVsEnemies()
{
	size_t enemyCount = enemies.size();
	for (int i = 0; i < enemyCount; ++i)
	{
		object* enemy1 = enemies.at(i);
		for (int j = i + 1; j < enemyCount; ++j)
		{
			object* enemy2 = enemies.at(j);
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