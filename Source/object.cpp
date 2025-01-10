#include "object.h"
#include "objectManager.h"

//”jŠü
void object::Destroy()
{
	objectManager::Instance().Remove(this);
}