#pragma once

#include"Stage.h"
#include"Player.h"
#include"CameraController.h"
#include"Scene.h"

// �Q�[���V�[��
class SceneGame:public Scene
{
public:
	SceneGame() {};
	~SceneGame() override {};

	// ������
	void Initialize() override;

	// �I����
	void Finalize() override;

	// �X�V����
	void Update(float elapsedTime) override;

	// �`�揈��
	void Render() override;

	// GUI�`��
	void DrawGUI();
private:
	Stage* stage = nullptr;
	CameraController* cameraController = nullptr;
};
