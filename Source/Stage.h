#pragma once

#include "system/ModelRenderer.h"

//�X�e�[�W
class Stage
{
public:
	Stage();
	~Stage();

	//�X�V����
	void Update(float elapsedTime);

	//�`�揈��
	void Render(const RenderContext& rc, ModelRenderer* rendere);

private:
	Model* model = nullptr;
};