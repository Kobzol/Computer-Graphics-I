#pragma once

#include <random>

class PathTracer : public Tracer
{
public:
	PathTracer(int recursionLimit, Shader& shader, int samplesPerPixel = 1);
	virtual Vector3 trace(Scene& scene, Camera &camera, float x, float y, OmniLight& light, Cubemap& cubemap) override;

private:
	Vector3 trace(Scene& scene, Ray ray, OmniLight& light, Cubemap& cubemap, int depth, int maxDepth);

	float brdf(const Vector3& eye, const Vector3& light, const Vector3& normal);
	Vector3 generateRandomVec(const Vector3& normal);
	Vector3 generateRandomHemi(const Vector3& normal);
	Vector3 generateRandomAngle(const Vector3& normal);

	int recursionLimit;
	Shader& shader;
	int samplesPerPixel;

	std::default_random_engine generator;
	std::uniform_real_distribution<double> distribution;
};
