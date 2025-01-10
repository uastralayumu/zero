#pragma once
#include<vector>
#include<set>
#include"object.h"

//�G�l�~�[�}�l�[�W���[
class objectManager
{
private:
	objectManager() {};
	~objectManager() {};
public:
	//�B��̃C���X�^���X�擾
	static objectManager& Instance()
	{
		static objectManager instance;
		return instance;
	}

	//�G�l�~�[�S�폜
	void Clear();

	//�G�l�~�[�폜
	void Remove(object* enemy);

	//�G�l�~�[�o�^
	void Register(object* enemy);

	//�X�V����
	void Update(float elapsedTime);

	//�`�揈��
	void Render(const RenderContext& rc, ModelRenderer* renderer);

	//�f�o�b�O�v���~�e�B�u�`��
	void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer);

	//�G�l�~�[���擾
	int GetEnemyCount() const { return static_cast<int>(enemies.size()); }

	//�G�l�~�[�擾
	object* GetEnemy(int index) { return enemies.at(index); }

private:
	//�G�l�~�[���m�̏Փˏ���
	void CollisionEnemyVsEnemies();

	std::vector<object*> enemies;
	std::set<object*> removes;

};