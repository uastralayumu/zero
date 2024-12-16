#include"System/Input.h"
#include"CameraController.h"
#include"Camera.h"

//更新処理
void CameraController::Update(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisRX();
	float ay = gamePad.GetAxisRY();
	//カメラの回転速度
	float speed = rollSpeed * elapsedTime;

	//スティックの入力値に合わせてX軸とY軸を回転
	angle.x += ay * speed;
	angle.y += ax * speed;

	//X軸のカメラ回転を制限
	if (maxAngleX < angle.x)
	{
		angle.x = maxAngleX;
	}
	if (minAngleX > angle.x)
	{
		angle.x = minAngleX;
	}
	//Y軸の回転値を-3.14～3.14に収まるようにする
	if (angle.y < -DirectX::XM_PI)
	{
		angle.y += DirectX::XM_2PI;
	}
	if (angle.y > DirectX::XM_PI)
	{
		angle.y -= DirectX::XM_2PI;
	}
	//カメラ回転値を回転行列に変換
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	
	DirectX::XMVECTOR Front = Transform.r[2];
	DirectX::XMFLOAT3 front;
	DirectX::XMStoreFloat3(&front, Front);

	DirectX::XMFLOAT3 eye;
	eye.x = target.x - front.x * range;
	eye.y = target.y - front.y * range;
	eye.z = target.z - front.z * range;

	//カメラの視点と注視点を設定
	Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}