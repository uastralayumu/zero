#include"Camera.h"

//�w�����������
void Camera::SetLookAt(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& focus, const DirectX::XMFLOAT3& up)
{
	//���_�A�����_�A���������r���[�s����쐬
	DirectX::XMVECTOR Eye = DirectX::XMLoadFloat3(&eye);
	DirectX::XMVECTOR Focus = DirectX::XMLoadFloat3(&focus);
	DirectX::XMVECTOR Up = DirectX::XMLoadFloat3(&up);
	DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH (Eye,Focus,Up);
	DirectX::XMStoreFloat4x4(&view, View);

	//�J�����̕��������o��
	this->right.x = view._11;
	this->right.y = view._21;
	this->right.z = view._31;

	this->up.x = view._12;
	this->up.y = view._22;
	this->up.z = view._32;

	this->front.x = view._13;
	this->front.y = view._23;
	this->front.z = view._33;

	//���_�A�����_��ۑ�
	this->eye = eye;
	this->focue = focue;
}

//�p�[�X�y�N�e�B�u�ݒ�
void Camera::SetPerspectiveFov(float fovY, float aspect, float nearZ, float farZ)
{
	//��p�A��ʔ䗦�A�N���b�v��������v���W�F�N�V�����s����쐬
	DirectX::XMMATRIX Projection = DirectX::XMMatrixPerspectiveFovLH(fovY, aspect, nearZ, farZ);

	DirectX::XMStoreFloat4x4(&projection, Projection);
}

Camera::Camera()
{

}

Camera::~Camera()
{

}