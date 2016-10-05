#include "stdafx.h"

OmniLight::OmniLight( const Vector3 & position, const Vector3 & ambient,
		const Vector3 & diffuse, const Vector3 & specular )
{
	this->position = position;

	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;

	occlusion = 0.0f;
}
