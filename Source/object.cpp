#include "object.h"
#include "objectManager.h"

//�j��
void object::Destroy()
{
	objectManager::Instance().Remove(this);
}