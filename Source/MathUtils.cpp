#include <stdlib.h>
#include"MathUtils.h"

//�w��͈͂̃����_���l���v�Z����
float MathUtils::RandomRange(float min, float max)
{
	//0.0�`1.0�̊Ԃ܂ł̃����_���l
	float value = static_cast<float>(rand()) / RAND_MAX;

	//min�`max�܂ł̃����_���l�ɕϊ�
	return min + (max - min) * value;
}