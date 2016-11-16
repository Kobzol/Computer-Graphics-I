#pragma once

#include <random>

class PathTracer : public Tracer
{
public:
	PathTracer(int samplesPerPixel, Shader& shader);
	virtual Vector3 trace(Scene& scene, Ray ray, OmniLight& light, Cubemap& cubemap, int depth) override;

private:
	Vector3 trace(Scene& scene, Ray ray, OmniLight& light, Cubemap& cubemap, int depth, int maxDepth);

	float brdf(const Vector3& eye, const Vector3& light);
	Vector3 generateRandomVec(const Vector3& normal);

	Shader& shader;
	std::default_random_engine generator;
	std::uniform_real_distribution<double> distribution;
};
