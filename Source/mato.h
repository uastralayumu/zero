#pragma once
#pragma once


#include"System/ModelRenderer.h"
#include"Character.h"

//�G�l�~�[
class mato : public Character
{
public:
	mato() {};
	~mato() override {};

	//�X�V����
	virtual void Update(float elapsedTime) = 0;

	//�j��
	void Destroy();

	//�`�揈��
	virtual void Render(const RenderContext& rc, ModelRenderer* rendrer) = 0;
};