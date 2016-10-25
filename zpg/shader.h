#pragma once

class Shader
{
public:
	virtual Vector3 calculateColor(Scene& scene, Ray& ray, const Vector3& eye, OmniLight& light) = 0;
	virtual Vector3 calculateColor(Scene& scene, Ray& ray, const Vector3& eye, OmniLight& light, Vector3 specular) = 0;

	float calcDiffuse(const Vector3& normal, const Ray& ray, const OmniLight& light);
	float calcSpecular(const Vector3& normal, const Ray& ray, const OmniLight& light, const Vector3& eye, float exponent = 1.0f);
};