#include "stdafx.h"

Shader::~Shader()
{

}

float Shader::calcDiffuse(const Vector3& normal, const Ray& ray, const  OmniLight& light)
{
	Vector3 toLight = light.position - ray.getIntersectPoint();
	toLight.Normalize();
	float diff = normal.DotProduct(toLight);
	return clamp(diff, 0.0f, 1.0f);
}
float Shader::calcSpecular(const Vector3& normal, const Ray& ray, const OmniLight& light, const Vector3& eye, float exponent)
{
	Vector3 position = ray.getIntersectPoint();
	Vector3 toLight = light.position - position;
	toLight.Normalize();

	Vector3 toEye = eye - position;
	toEye.Normalize();

	Vector3 half = toLight + toEye;
	half.Normalize();

	return pow(clamp(normal.DotProduct(half), 0.0f, 1.0f), exponent);
}
float Shader::calcShadow(Scene& scene, const Vector3& point, const OmniLight& light)
{
	Vector3 toLight = light.position - point;
	Ray rayToLight(point, toLight, 0.01f, toLight.L2Norm());
	rtcOccluded(scene.handle(), rayToLight);

	if (rayToLight.hasCollided())
	{
		return 0.0f;
	}
	else return 1.0f;
}
