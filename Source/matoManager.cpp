#include "matoManager.h"
#include "Collision.h"

//�G�l�~�[�S�폜
void matoManager::Clear()
{
	for (mato* enemy : enemies)
	{
		delete enemy;
	}
	enemies.clear();
}

//�G�l�~�[�폜
void matoManager::Remove(mato* enemy)
{
	//�j�����X�g�ɒǉ�
	removes.insert(enemy);
}

//�G�l�~�[�o�^
void matoManager::Register(mato* enemy)
{
	enemies.emplace_back(enemy);
}

//�X�V����
void matoManager::Update(float elapsedTime)
{
	for (mato* enemy : enemies)
	{
		enemy->Update(elapsedTime);
	}
	//�j������
	//��enemies�͈̔�for������erase()����ƕs����������Ă��܂����߁A
	// �X�V�������I�������ɔj�����X�g�ɐς܂ꂽ�I�u�W�F�N�g���폜����B
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
	//�j�����X�g���N���A
	removes.clear();

	//�G���m�̏Փˏ���
	CollisionEnemyVsEnemies();
}

void matoManager::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	for (mato* enemy : enemies)
	{
		enemy->Render(rc, renderer);
	}
}

//�f�o�b�O�v���~�e�B�u�`��
void matoManager::RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)
{
	for (mato* enemy : enemies)
	{
		enemy->RenderDebugPrimitive(rc, renderer);
	}
}

//�G�l�~�[���m�̏Փˏ���
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