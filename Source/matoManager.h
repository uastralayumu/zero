#pragma once
#pragma once

#include<vector>
#include<set>
#include"mato.h"

//�G�l�~�[�}�l�[�W���[
class matoManager
{
private:
	matoManager() {};
	~matoManager() {};
public:
	//�B��̃C���X�^���X�擾
	static matoManager& Instance()
	{
		static matoManager instance;
		return instance;
	}

	//�G�l�~�[�S�폜
	void Clear();

	//�G�l�~�[�폜
	void Remove(mato* enemy);

	//�G�l�~�[�o�^
	void Register(mato* enemy);

	//�X�V����
	void Update(float elapsedTime);

	//�`�揈��
	void Render(const RenderContext& rc, ModelRenderer* renderer);

	//�f�o�b�O�v���~�e�B�u�`��
	void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer);

	//�G�l�~�[���擾
	int GetEnemyCount() const { return static_cast<int>(enemies.size()); }

	//�G�l�~�[�擾
	mato* GetEnemy(int index) { return enemies.at(index); }

private:
	//�G�l�~�[���m�̏Փˏ���
	void CollisionEnemyVsEnemies();

	std::vector<mato*> enemies;
	std::set<mato*> removes;

};