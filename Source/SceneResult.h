#pragma once

#include "System/Sprite.h"
#include "Scene.h"
#include"System/AudioSource.h"

class SceneResult :public Scene
{
public:
	SceneResult() {}
	~SceneResult() override {}

	//‰Šú‰»
	void Initialize() override;

	//I—¹‰»
	void Finalize() override;

	//XVˆ—
	void  Update(float elapsedTime) override;

	//•`‰æˆ—
	void Render() override;

	//GUI•`‰æ
	void DrawGUI() override;

private:
	Sprite* sprite = nullptr;
	AudioSource* BGMresult = nullptr;
};