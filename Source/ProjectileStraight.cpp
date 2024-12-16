#include "ProjectileStraight.h"

//�R���X�g���N�^
ProjectileStraight::ProjectileStraight(ProjectileManager* manager)
	: Projectile(manager)
{
	model = new Model("Data/Model/Sword/Sword.mdl");

	//�\���T�C�Y�𒲐�
	scale.x = scale.y = scale.z = 3.0f;
}

//�f�X�g���N�^
ProjectileStraight::~ProjectileStraight()
{
	delete model;
}

//�X�V����
void ProjectileStraight::Update(float elapsedTime)
{
	//��������
	lifeTimer -= 0.01f;
	if (lifeTimer < 0.0f)
	{
		//�������폜
		Destroy();
	}
	//�ړ� 
	float speed = this->speed * elapsedTime;
	position.x += direction.x * speed;
	position.y += direction.y * speed;
	position.z += direction.z * speed;

	//�I�u�W�F�N�g�s����X�V
	UpdateTransform();

	//���f���s��X�V
	model->UpdateTransform();
}

//�`�揈��
void ProjectileStraight::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	renderer->Render(rc, transform, model, ShaderId::Lambert);
}

//����
void ProjectileStraight::Launch(const DirectX::XMFLOAT3& direction,
	const DirectX::XMFLOAT3& position)
{
	this->direction = direction;
	this->position = position;
}