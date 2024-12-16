#include"System/Input.h"
#include"CameraController.h"
#include"Camera.h"

//�X�V����
void CameraController::Update(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisRX();
	float ay = gamePad.GetAxisRY();
	//�J�����̉�]���x
	float speed = rollSpeed * elapsedTime;

	//�X�e�B�b�N�̓��͒l�ɍ��킹��X����Y������]
	angle.x += ay * speed;
	angle.y += ax * speed;

	//X���̃J������]�𐧌�
	if (maxAngleX < angle.x)
	{
		angle.x = maxAngleX;
	}
	if (minAngleX > angle.x)
	{
		angle.x = minAngleX;
	}
	//Y���̉�]�l��-3.14�`3.14�Ɏ��܂�悤�ɂ���
	if (angle.y < -DirectX::XM_PI)
	{
		angle.y += DirectX::XM_2PI;
	}
	if (angle.y > DirectX::XM_PI)
	{
		angle.y -= DirectX::XM_2PI;
	}
	//�J������]�l����]�s��ɕϊ�
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	
	DirectX::XMVECTOR Front = Transform.r[2];
	DirectX::XMFLOAT3 front;
	DirectX::XMStoreFloat3(&front, Front);

	DirectX::XMFLOAT3 eye;
	eye.x = target.x - front.x * range;
	eye.y = target.y - front.y * range;
	eye.z = target.z - front.z * range;

	//�J�����̎��_�ƒ����_��ݒ�
	Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}