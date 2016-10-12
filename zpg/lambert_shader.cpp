#include "stdafx.h"

Vector3 LambertShader::calculateColor(Scene& scene, Ray& ray, OmniLight& light)
{
	Vector3 normal = this->getNormal(scene, ray);
	float diff = this->calcDiffuse(normal, ray, light);
	float spec = this->calcSpecular(normal, ray, light, light.position, 8.0f);
	Vector3 specColor = Vector3(1.0f, 1.0f, 1.0f);

	return (diff * this->getTextureOrMatDiff(scene, ray)) + (spec * specColor) + light.ambient;
}
