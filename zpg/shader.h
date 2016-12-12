#pragma once

class Shader
{
public:
	virtual ~Shader();

	virtual Vector3 calculateColor(Scene& scene, Ray& ray, const Vector3& eye, OmniLight& light) = 0;

	virtual Vector3 calcDiffuseColor(Scene& scene, const Vector3& normal, const Ray& ray, const OmniLight& light);
	virtual float calcDiffuse(const Vector3& normal, const Ray& ray, const OmniLight& light);
	virtual float calcSpecular(const Vector3& normal, const Ray& ray, const OmniLight& light, const Vector3& eye, float exponent = 1.0f);
	virtual float calcShadow(Scene& scene, const Vector3& point, const OmniLight& light);
};