#include "Enemy.h"
#include "EnemyManager.h"

//�j��
void Enemy::Destroy()
{
	EnemyManager::Instance().Remove(this);
}