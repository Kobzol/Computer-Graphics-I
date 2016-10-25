#include "stdafx.h"

Vector3 LambertShader::calculateColor(Scene& scene, Ray& ray, const Vector3& eye, OmniLight& light)
{
	return this->calculateColor(scene, ray, eye, light, scene.getTriangle(ray).getTextureOrMatSpec(ray));
}
Vector3 LambertShader::calculateColor(Scene& scene, Ray& ray, const Vector3& eye, OmniLight& light, Vector3 specVec)
{
	Triangle& triangle = scene.getTriangle(ray);
	Vector3 normal = triangle.normal(ray);
	float diff = this->calcDiffuse(normal, ray, light);
	float spec = this->calcSpecular(normal, ray, light, eye, 8.0f);

	return (diff * triangle.getTextureOrMatDiff(ray)) + (spec * specVec) + light.ambient;
}
