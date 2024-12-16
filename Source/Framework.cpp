#include <memory>
#include <sstream>
#include <imgui.h>

#include "Framework.h"
#include "System/Input.h"
#include "System/Graphics.h"
#include "System/ImGuiRenderer.h"
#include "SceneGame.h"
#include"SceneTitle.h"
#include"SceneManager.h"
#include"EffectManager.h"
#include"System/Audio.h"

// ���������Ԋu�ݒ�
static const int syncInterval = 1;

// �R���X�g���N�^
Framework::Framework(HWND hWnd)
	: hWnd(hWnd)
{
	//�I�[�f�B�I������
	Audio::Instance().Initialize();

	hDC = GetDC(hWnd);

	// �C���v�b�g������
	Input::Instance().Initialize(hWnd);

	// �O���t�B�b�N�X������
	Graphics::Instance().Initialize(hWnd);

	// IMGUI������
	ImGuiRenderer::Initialize(hWnd, Graphics::Instance().GetDevice(), Graphics::Instance().GetDeviceContext());

	//�G�t�F�N�g�}�l�[�W���[������
	EffectManager::Instance().Initialize();

	// �V�[��������
	SceneManager::Instance().ChangeScene(new SceneTitle);
}

// �f�X�g���N�^
Framework::~Framework()
{
	// IMGUI�I����
	ImGuiRenderer::Finalize();

	ReleaseDC(hWnd, hDC);

	// �V�[���I����
	SceneManager::Instance().Clear();

	//�G�t�F�N�g�}�l�[�W���[�I����
	EffectManager::Instance().Finalize();

	//�I�[�f�B�I�I����
	Audio::Instance().Finalize();
}

// �X�V����
void Framework::Update(float elapsedTime)
{
	// �C���v�b�g�X�V����
	Input::Instance().Update();

	// IMGUI�t���[���J�n����	
	ImGuiRenderer::NewFrame();

	// �V�[���X�V����
	SceneManager::Instance().Update(elapsedTime);
}

// �`�揈��
void Framework::Render(float elapsedTime)
{
	//�ʃX���b�h���Ƀf�o�C�X�R���e�L�X�g���g���Ă����ꍇ��
	//�����A�N�Z�X���Ȃ��悤�ɔr�����䂷��
	std::lock_guard<std::mutex> lock(Graphics::Instance().GetMutex());

	ID3D11DeviceContext* dc = Graphics::Instance().GetDeviceContext();

	// ��ʃN���A
	Graphics::Instance().Clear(0, 0, 1, 1);

	// �����_�[�^�[�Q�b�g�ݒ�
	Graphics::Instance().SetRenderTargets();

	// �V�[���`�揈��
	SceneManager::Instance().Render();

	// �V�[��GUI�`�揈��
	SceneManager::Instance().DrawGUI();

#if 0
	// IMGUI�f���E�C���h�E�`��iIMGUI�@�\�e�X�g�p�j
	ImGui::ShowDemoWindow();
#endif
	// IMGUI�`��
	ImGuiRenderer::Render(dc);

	// ��ʕ\��
	Graphics::Instance().Present(syncInterval);
}

// �t���[�����[�g�v�Z
void Framework::CalculateFrameStats()
{
	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.  These stats 
	// are appended to the window caption bar.
	static int frames = 0;
	static float time_tlapsed = 0.0f;

	frames++;

	// Compute averages over one second period.
	if ((timer.TimeStamp() - time_tlapsed) >= 1.0f)
	{
		float fps = static_cast<float>(frames); // fps = frameCnt / 1
		float mspf = 1000.0f / fps;
		std::ostringstream outs;
		outs.precision(6);
		outs << "FPS : " << fps << " / " << "Frame Time : " << mspf << " (ms)";
		SetWindowTextA(hWnd, outs.str().c_str());

		// Reset for next average.
		frames = 0;
		time_tlapsed += 1.0f;
	}
}

// �A�v���P�[�V�������[�v
int Framework::Run()
{
	MSG msg = {};

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			timer.Tick();
			CalculateFrameStats();

			float elapsedTime = syncInterval == 0
				? timer.TimeInterval()
				: syncInterval / static_cast<float>(GetDeviceCaps(hDC, VREFRESH))
				;
			Update(elapsedTime);
			Render(elapsedTime);
		}
	}
	return static_cast<int>(msg.wParam);
}

// ���b�Z�[�W�n���h��
LRESULT CALLBACK Framework::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGuiRenderer::HandleMessage(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc;
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) PostMessage(hWnd, WM_CLOSE, 0, 0);
		break;
	case WM_ENTERSIZEMOVE:
		// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
		timer.Stop();
		break;
	case WM_EXITSIZEMOVE:
		// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
		// Here we reset everything based on the new window dimensions.
		timer.Start();
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}
