#include<imgui.h>
#include"System/Input.h"
#include "Player.h"
#include "System/Input.h"
#include "Camera.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "ProjectileStraight.h"
#include "ProjectileHoming.h"
#include "System/Graphics.h"
#include "objectManager.h"

//������
void Player::Initialize()
{	
	//���f�����傫���̂ŃX�P�[�����O
	scale.x = scale.y = scale.z = 0.01f;

	//�q�b�g�G�t�F�N�g�ǂݍ���
	hitEffect = new Effect("Data/Effect/Hit.efk");

	//�q�b�gSE�ǂݍ���
	hitSE = Audio::Instance().LoadAudioSource("Data/Sound/Hit.wav");

	danganCount = 0;
	position = { 0,0,0 };
	angle = { 0,0,0 };
	score = 0;
	combo = 0;
}

//�I����
void Player::Finalize()
{
	delete hitSE;
	delete hitEffect;
	delete model;
}

//�X�V����
void Player::Update(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	//�ړ����͏���
	InputMove(elapsedTime);
	//�I�u�W�F�N�g�s����X�V
	UpdateTransform();
	//�e�ۓ��͏���
	InputProjectile();
	//���͏����X�V
	UpdateVelocity(elapsedTime);
	//�e�ۍX�V����
	projectileManager.Update(elapsedTime);
	//�e�ۂƓG�̏Փ˔���
	CollisitionProjectilesVsEnemies();

	if (gamePad.GetButton() & GamePad::BTN_X)
	{
		move = 0.03f;
	}
	else move = 0.05f;

	if (position.z <= -363)
	{
		move = 0;
		position.z = -363;
	}

	position.z -= move;
}

//�`�揈��
void Player::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	//�e�ە`�揈��
	projectileManager.Render(rc, renderer);
}

//�f�o�b�O�pGUI�`��
void Player::DrawDebugGUI()
{
	ImVec2 pos = ImGui::GetMainViewport()->GetWorkPos();
	ImGui::SetNextWindowPos(ImVec2(pos.x + 10, pos.y + 10), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("Player,nullptr,ImGuiWindowFlags_None"))
	{
		//�g�����X�t�H�[��
		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			//�ʒu
			ImGui::InputFloat3("Positon", &position.x);
			//��]
			DirectX::XMFLOAT3 a;
			a.x = DirectX::XMConvertToDegrees(angle.x);
			a.y = DirectX::XMConvertToDegrees(angle.y);
			a.z = DirectX::XMConvertToDegrees(angle.z);
			ImGui::InputFloat3("Angle", &a.x);
			angle.x = DirectX::XMConvertToRadians(a.x);
			angle.y = DirectX::XMConvertToRadians(a.y);
			angle.z = DirectX::XMConvertToRadians(a.z);
			//�X�P�[��
			ImGui::InputFloat3("Scale", &scale.x);
		}
	}
	ImGui::End();
}

void Player::OnLanding()
{
	jumpCount = 0;
}

DirectX::XMFLOAT3 Player::GetMoveVec() const
{
	//���͏����擾
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisLX();
	float ay = gamePad.GetAxisLY();

	//�J���������ƃX�e�B�b�N�̓��͒l�ɂ���Đi�s�������v�Z����
	Camera& camera = Camera::Instance();
	const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
	const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

	//�ړ��x�N�g����XZ���ʂɐ����ȃx�N�g���ɂȂ�悤�ɂ���

	//�J�����E�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
	float cameraRightX = cameraRight.x;
	float cameraRightZ = cameraRight.z;
	float cameraRightLenght = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
	if (cameraRightLenght > 0.0f)
	{
		//�P�ʃx�N�g����
		cameraRightX /= cameraRightLenght;
		cameraRightZ /= cameraRightLenght;
	}

	//�J�����O���s�x�N�g����XZ�P�ʃx�N�g���ɕϊ�
	float cameraFrontX = cameraFront.x;
	float cameraFrontZ = cameraFront.z;
	float cameraFrontLenght = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
	if (cameraFrontLenght > 0.0f)
	{
		//�P�ʃx�N�g����
		cameraFrontX /= cameraFrontLenght;
		cameraFrontZ /= cameraFrontLenght;
	}
	//�X�e�B�b�N�̐������͒l���J�����E�����ɔ��f���A
	//�X�e�B�b�N�̐������͒l���J�����O�����ɔ��f���A
	//�i�s�x�N�g�����v�Z����
	DirectX::XMFLOAT3 vec;
	vec.x = (cameraRightX * ax) + (cameraFrontX * ax);
	vec.z = (cameraRightZ * ay) + (cameraFrontZ * ay);
	//Y�������Ɉړ����Ȃ�
	vec.y = 0.0f;
	return vec;
}

//�ړ����͏���
void Player::InputMove(float elapsedTime)
{
	//�i�s�x�N�g���擾
	DirectX::XMFLOAT3 moveVec = GetMoveVec();

	//�ړ�����
	Move(elapsedTime, moveVec.x, moveVec.z, moveSpeed);

	//���񏈗�
	Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);
}

//�W�����v���͏���
void Player::InputJump()
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	if (gamePad.GetButtonDown() & GamePad::BTN_A)
	{
		if (jumpCount < jumpLimit)
		{
			jumpCount++;
			Jump(jumpSpeed);
		}
		
	}
}


//�v���C���[�ƃG�l�~�[�Ƃ̏Փˏ���
void Player::CollisionPlayerVsEnemies()
{
	EnemyManager& enemyManager = EnemyManager::Instance();

	//���ׂĂ̓G�Ƒ�������ŏՓˏ���
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);

		//�Փˏ���
		DirectX::XMFLOAT3 outPosition;
		if (Collision::IntersectCylinderVsCylinder(
			position,radius,height,
			enemy->GetPosition(),
			enemy->GetRadius() ,
			enemy->GetHeight(),
			outPosition))
		{
			//�����o����̈ʒu�ݒ�
			DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
			DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
			DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P, E);
			DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
			DirectX::XMFLOAT3 normal;
			DirectX::XMStoreFloat3(&normal, N);
			//�ォ�瓥��Â����ꍇ�͏��W�����v����
			if(normal.y > 0.8f)
			{
				Jump(jumpSpeed * 0.5f);
			}
			else
			{
				enemy->SetPosition(outPosition);
			}
		}
	}

}

//�f�o�b�O�v���~�e�B�u�`��
void Player::RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)
{
	//���N���X�̊֐��Ăяo��
	Character::RenderDebugPrimitive(rc, renderer);

	//�e�ۃf�o�b�O�v���~�e�B�u�`��
	projectileManager.RenderDebugPrimitive(rc, renderer);
}

//�e�ۓ��͏���
void Player::InputProjectile()
{
	Mouse& mouse = Input::Instance().GetMouse();
	mouseposx = mouse.GetPositionX();
	mouseposy = mouse.GetPositionY();	
	Graphics& graphics = Graphics::Instance();
	float screenWidth = static_cast<float>(graphics.GetScreenWidth());
	float screeHeight = static_cast<float>(graphics.GetScreenHeight());
	float mousedirx = ((mouseposx * 2 / screenWidth) - 1) * 0.65f;
	float mousediry = ((mouseposy * 2 / screeHeight) - 1) * 0.35f;
	Camera& camera = Camera::Instance();
	DirectX::XMFLOAT3 camerapos = camera.GetEye();
	DirectX::XMFLOAT3 cameradir = camera.GetFront();

	//���i�e�۔���
	if (mouse.GetButtonDown() & Mouse::BTN_LEFT && danganCount < maxdanganCount)
	{
		//�O����
		DirectX::XMFLOAT3 dir;
		dir.x = sinf(cameradir.x) + mousedirx;
		dir.y = sinf(cameradir.y) - mousediry;
		dir.z = sinf(cameradir.z);
		//���ˈʒu�i�v���C���[�̍�������j
		DirectX::XMFLOAT3 pos;
		pos.x = camerapos.x;
		pos.y = camerapos.y + 0.01;
		pos.z = camerapos.z;
		//����
		ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
		projectile->Launch(dir, pos);
		danganCount++;
	}
	
}



void  Player::CollisitionProjectilesVsEnemies()
{
	EnemyManager& enemyManager = EnemyManager::Instance();

	//���ׂĂ̒e�ۂƂ��ׂĂ̓G�𑍓�����ŏՓˏ���
	int projectileCount = projectileManager.GetProjectileCoust();
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0; i < projectileCount; ++i)
	{
		Projectile* projectile = projectileManager.GetProjectile(i);
		for (int j = 0; j < enemyCount; ++j)
		{
			Enemy* enemy = enemyManager.GetEnemy(j);
			//�Փˏ���
			DirectX::XMFLOAT3 outPositon;
			if (Collision::IntersectSphereVsCylinder(
				projectile->GetPosition(),
				projectile->GetRaidus(),
				enemy->GetPosition(),
				enemy->GetRadius() *2,
				enemy->GetHeight(),
				outPositon))
			{
				//�_���[�W��^����
				if (enemy->ApplyDamage(1, 0.5f))
				{
					
					//�q�b�g�G�t�F�N�g�Đ�
					{
						DirectX::XMFLOAT3 e = enemy->GetPosition();
						e.y += enemy->GetHeight() * 0.5f;
						hitEffect->Play(e);
					}

					//�q�b�gSE�Đ�
					{
						hitSE->Play(false);
					}

					{
						score += enemy->GetScore();


						if (enemy->GetCombo())
						{
							combo += 1;
						}
						else
						{
							combo = 0;
						}

						if (combo >= 2)
						{
							score += 30;
						}

					}
					//�e�۔j��
					projectile->Destroy();
				}
			}
		}
	}

}
