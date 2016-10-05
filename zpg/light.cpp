#include "stdafx.h"

Light::Light(Vector3 position): position(position)
{

}

Vector3 Light::getPosition()
{
	return this->position;
}
