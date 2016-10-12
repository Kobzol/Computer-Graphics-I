#pragma once

class Shader
{
public:
	virtual Vector3 calculateColor(Scene& scene, Ray& ray, OmniLight& light) = 0;

protected:
	Vector3 getIntersectPosition(Ray& ray);
	Vector3 getNormal(Scene& scene, Ray& ray);
	Vector3 getNormal(Triangle& triangle, Ray& ray);

	Triangle& getTriangle(Scene& scene, Ray& ray);

	float calcDiffuse(const Vector3& normal, Ray& ray, OmniLight& light);
	float calcSpecular(const Vector3& normal, Ray& ray, OmniLight& light, const Vector3& eye, float exponent = 1.0f);

	Vector3 getTextureOrMatDiff(Scene& scene, Ray& ray);
};