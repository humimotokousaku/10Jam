#include "IPart.h"

uint32_t IPart::sSerialNumber = 0;

void IPart::ColliderUpdate()
{
	this->SetOBBCenterPos(GetWorldPosition());
	this->SetOBBDirect(0);
}
