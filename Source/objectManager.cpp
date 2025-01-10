#include "objectManager.h"
#include "Collision.h"

//�G�l�~�[�S�폜
void objectManager::Clear()
{
	for (object* enemy : enemies)
	{
		delete enemy;
	}
	enemies.clear();
}

//�G�l�~�[�폜
void objectManager::Remove(object* enemy)
{
	//�j�����X�g�ɒǉ�
	removes.insert(enemy);
}

//�G�l�~�[�o�^
void objectManager::Register(object* enemy)
{
	enemies.emplace_back(enemy);
}

//�X�V����
void objectManager::Update(float elapsedTime)
{
	for (object* enemy : enemies)
	{
		enemy->Update(elapsedTime);
	}
	//�j������
	//��enemies�͈̔�for������erase()����ƕs����������Ă��܂����߁A
	// �X�V�������I�������ɔj�����X�g�ɐς܂ꂽ�I�u�W�F�N�g���폜����B
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
	//�j�����X�g���N���A
	removes.clear();

	//�G���m�̏Փˏ���
	CollisionEnemyVsEnemies();
}

void objectManager::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	for (object* enemy : enemies)
	{
		enemy->Render(rc, renderer);
	}
}

//�f�o�b�O�v���~�e�B�u�`��
void objectManager::RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)
{
	for (object* enemy : enemies)
	{
		enemy->RenderDebugPrimitive(rc, renderer);
	}
}

//�G�l�~�[���m�̏Փˏ���
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