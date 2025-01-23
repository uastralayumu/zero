#pragma once

#include "System/Sprite.h"
#include "Scene.h"
#include"System/AudioSource.h"

class SceneResult :public Scene
{
public:
	SceneResult() {}
	~SceneResult() override {}

	//������
	void Initialize() override;

	//�I����
	void Finalize() override;

	//�X�V����
	void  Update(float elapsedTime) override;

	//�`�揈��
	void Render() override;

	//GUI�`��
	void DrawGUI() override;

private:
	Sprite* sprite = nullptr;
	Sprite* Score = nullptr;
	AudioSource* BGMresult = nullptr;

	int higtscore = 0;
};