#pragma once

#include"Stage.h"
#include"Player.h"
#include"CameraController.h"
#include"Scene.h"
#include "System/Sprite.h"
#include"System/AudioSource.h"
#include"object.h"

// ゲームシーン
class SceneGame:public Scene
{
public:
	SceneGame() {};
	~SceneGame() override {};

	// 初期化
	void Initialize() override;

	// 終了化
	void Finalize() override;

	// 更新処理
	void Update(float elapsedTime) override;

	// 描画処理
	void Render() override;

	// GUI描画
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
