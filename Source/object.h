#pragma once


#include"System/ModelRenderer.h"
#include"Character.h"

//�G�l�~�[
class object : public Character
{
public:
	object() {};
	~object() override {};

	//�X�V����
	virtual void Update(float elapsedTime) = 0;

	//�j��
	void Destroy();

	//�`�揈��
	virtual void Render(const RenderContext& rc, ModelRenderer* rendrer) = 0;
};