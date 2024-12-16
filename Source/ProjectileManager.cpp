#include "ProjectileManager.h"

//�R���X�g���N�^
ProjectileManager::ProjectileManager()
{
}

//�f�X�g���N�^
ProjectileManager::~ProjectileManager()
{
	Clear();
}

//�e�ۍ폜
void ProjectileManager::Remove(Projectile* projectile)
{
	//�j�����X�g�ɒǉ�
	removes.insert(projectile);
}

//�X�V����
void ProjectileManager::Update(float elapsedTime)
{
	for (Projectile* projectile : projectiles)
	{
		projectile->Update(elapsedTime);
	}

	//�j������
	//��projectiles�͈̔�for������erase()����ƕs����������Ă��܂����߁A
	//  �X�V�������I�������ɔj�����X�g�ɐς܂ꂽ�I�u�W�F�N�g���폜����B
	for (Projectile* projectile : removes)
	{
		//std::vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜���Ȃ���΂Ȃ�Ȃ�
		std::vector<Projectile*>::iterator it = std::find(projectiles.begin(),
			projectiles.end(), projectile);
		if (it != projectiles.end())
		{
			projectiles.erase(it);
		}
		//�e�ۂ̔j������
		delete projectile;
	}
	//�j�����X�g���N���A
	removes.clear();
}

//�`�揈��
void ProjectileManager::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	for (Projectile* projectile : projectiles)
	{
		projectile->Render(rc,renderer);
	}
}

//�f�o�b�O�v���~�e�B�u�`��
void ProjectileManager::RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)
{
	for (Projectile* projectile : projectiles)
	{
		projectile->RenderDebugPrimitive(rc, renderer);
	}
}

//�e�ۓo�^
void ProjectileManager::Register(Projectile* projectile)
{
	projectiles.emplace_back(projectile);
}

//�e�ۑS�폜
void ProjectileManager::Clear()
{
	for(Projectile* projectile :projectiles) 
	{
		delete projectile;
	}
	projectiles.clear();
}

