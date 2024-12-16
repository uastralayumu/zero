#pragma once

#include"System/ModelRenderer.h"
#include"Character.h"
#include "ProjectileManager.h"
#include "Effect.h"
#include"System/Audio.h"

//�v���C���[
class Player : public Character
{
private:
	Player() {};
	~Player() override {};
public:
	//�C���X�^���X�擾
	static Player& Instance()
	{
		static Player instance;
		return instance;
	}

	//������
	void Initialize();

	//�I����
	void Finalize();

	//�X�V����
	void Update(float elapsedTime);
	//�`�揈��
	void Render(const RenderContext& rc, ModelRenderer* renderer);
	//�f�o�b�O�pGUI�`��
	void DrawDebugGUI();
	//�f�o�b�O�v���~�e�B�u�`��
	void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer);

protected:
	//���n�������ɌĂ΂��
	void OnLanding() override;

private:
	//�X�e�B�b�N���͒l����ړ��x�N�g�����擾
	DirectX::XMFLOAT3 GetMoveVec() const;

	//�ړ����͏���
	void InputMove(float elapsedTime);

	//�W�����v���͏���
	void InputJump();

	//�v���C���[�ƃG�l�~�[�Ƃ̏Փˏ���
	void CollisionPlayerVsEnemies();

	//�e�ۓ��͏���
	void InputProjectile();

	//�e�ۂƓG�̏Փˏ���
	void CollisitionProjectilesVsEnemies();

private:
	Model* model = nullptr;
	float moveSpeed = 5.0f;
	float turnSpeed = DirectX::XMConvertToRadians(720);
	float jumpSpeed = 12.0f;
	int jumpCount = 0;
	int jumpLimit = 2;
	ProjectileManager projectileManager;
	Effect*  hitEffect = nullptr;
	AudioSource* hitSE = nullptr;
};