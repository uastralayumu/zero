#pragma once

#include"Stage.h"
#include"Player.h"
#include"CameraController.h"
#include"Scene.h"
#include "System/Sprite.h"
#include"System/AudioSource.h"
#include"object.h"

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
	Sprite* sprite = nullptr;
	Sprite* dangan = nullptr;
	Sprite* distance = nullptr;
	Sprite* presentlocation = nullptr;
	Sprite* Score = nullptr;
	CameraController* cameraController = nullptr;
	object* object = nullptr;
	AudioSource* BGMmain = nullptr;
};
