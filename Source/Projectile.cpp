#include "Projectile.h"
#include "ProjectileManager.h"

//�R���X�g���N�^
Projectile::Projectile(ProjectileManager* manager)
	:manager(manager)
{
	manager->Register(this);
}

//�f�o�b�O�v���~�e�B�u�`��
void Projectile::RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)
{
	//�Փ˔���p�̃f�o�b�O����`��
	renderer->RenderSphere(rc, position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
}

//�s��X�V����
void Projectile::UpdateTransform()
{
	DirectX::XMVECTOR Front, Up, Right;

	//�O�x�N�g�����Z�o
	Front = DirectX::XMLoadFloat3(&direction);
	Front = DirectX::XMVector3Normalize(Front);

	//���̏�x�N�g�����Z�o
	Up = DirectX::XMVectorSet(0.001f, 1, 0, 0);
	Up = DirectX::XMVector3Normalize(Up);

	//�E�x�N�g�����Z�o
	Right = DirectX::XMVector3Cross(Front, Up);
	Right = DirectX::XMVector3Normalize(Right);

	//��x�N�g�����Z�o
	Up = DirectX::XMVectorSubtract(Front,Right);

	//�v�Z���ʂ����o��
	DirectX::XMFLOAT3 right, up, front;
	DirectX::XMStoreFloat3(&right, Right);
	DirectX::XMStoreFloat3(&up, Up);
	DirectX::XMStoreFloat3(&front, Front);

	//�Z�o�������x�N�g������s����쐬
	transform._11 = right.x * scale.x;
	transform._12 = right.y * scale.y;
	transform._13 = right.z * scale.z;
	transform._14 = 0.0f;
	transform._21 = up.x * scale.x;
	transform._22 = up.y * scale.y;
	transform._23 = up.z * scale.z;
	transform._24 = 0.0f;
	transform._31 = front.x * scale.x;
	transform._32 = front.y * scale.y;
	transform._33 = front.z * scale.z;
	transform._34 = 0.0f;
	transform._41 = position.x;
	transform._42 = position.y;
	transform._43 = position.z;
	transform._44 = 1.0f;

		//���˕���
		this->direction = front;
}

//�j��
void Projectile::Destroy()
{
	manager->Remove(this);
}