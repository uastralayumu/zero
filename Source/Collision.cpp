#include "Collision.h"

//���Ƌ��̌�������
bool Collision::IntersectSphereVsSphere(
	const DirectX::XMFLOAT3& positionA,
	float radiusA,
	const DirectX::XMFLOAT3& positionB,
	float radiusB,
	DirectX::XMFLOAT3& outPositionB)
{
	//A��B�̒P�ʃx�N�g�����Z�o
	DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);
	DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);
	DirectX::XMVECTOR Vec       = DirectX::XMVectorSubtract(PositionB, PositionA);
	DirectX::XMVECTOR LengthSq  = DirectX::XMVector3LengthSq(Vec);
	float lengthSq;
	DirectX::XMStoreFloat(&lengthSq, LengthSq);

	//��������
	float range = radiusA + radiusB;
	if (lengthSq > range * range)
	{
		return false;
	}

	//A��B�������o��
	Vec = DirectX::XMVector3Normalize(Vec);
	Vec = DirectX::XMVectorScale(Vec, range);
	PositionB = DirectX::XMVectorAdd(PositionA, Vec);
	DirectX::XMStoreFloat3(&outPositionB,PositionB);

	return true;
}

//�~���Ɖ~���̌�������
bool Collision::IntersectCylinderVsCylinder(
	const DirectX::XMFLOAT3& positionA,
	float radiusA,
	float heightA,
	const DirectX::XMFLOAT3& positionB,
	float radiusB,
	float heightB,
	DirectX::XMFLOAT3& outPositionB)
{
	//A�̑�����B�̓�����Ȃ瓖�����Ă��Ȃ�
	if (positionA.y > positionB.y + heightB )
	{
		return false;
	}

	//A�̓���B�̑�����艺�Ȃ瓖�����Ă��Ȃ�
	if (positionA.y + heightA < positionB.y)
	{
		return false;
	}

	//XZ���ʂł͈̔̓`�F�b�N
	float vx = positionB.x - positionA.x;
	float vz = positionB.z - positionA.z;
	float range = radiusA + radiusB;
	float distXZ = sqrtf(vx * vx + vz * vz);
	if (distXZ > range)
	{
		return false;
	}

	//�P�ʃx�N�g��
	vx /= distXZ;
	vz /= distXZ;

	//A��B�������o��
 	outPositionB.x = (vx * range) + positionA.x;
	outPositionB.y = positionB.y;
	outPositionB.z = (vz * range) + positionA.z;

	return true;
}

bool Collision::IntersectSphereVsCylinder(
	const DirectX::XMFLOAT3& spherePosition,
	float sphereRadius,
	const DirectX::XMFLOAT3& cylinderPosition,
	float cylinderRadius,
	float cylinderHeight,
	DirectX::XMFLOAT3& outCylinderPosition
)
{
	//A�̑�����B�̓�����Ȃ瓖�����Ă��Ȃ�
	if (spherePosition.y > cylinderPosition.y + cylinderHeight)
	{
		return false;
	}

	//A�̓���B�̑�����艺�Ȃ瓖�����Ă��Ȃ�
	if (spherePosition.y + sphereRadius < cylinderPosition.y)
	{
		return false;
	}

	//XZ���ʂł͈̔̓`�F�b�N
	float vx = cylinderPosition.x - spherePosition.x;
	float vz = cylinderPosition.z - spherePosition.z;
	float range = sphereRadius + cylinderRadius;
	float distXZ = sqrtf(vx * vx + vz * vz);
	if (distXZ > range)
	{
		return false;
	}

	//�P�ʃx�N�g��
	vx /= distXZ;
	vz /= distXZ;

	//A��B�������o��
	outCylinderPosition.x = (vx * range) + spherePosition.x;
	outCylinderPosition.y = cylinderPosition.y;
	outCylinderPosition.z = (vz * range) + spherePosition.z;

	return true;
}