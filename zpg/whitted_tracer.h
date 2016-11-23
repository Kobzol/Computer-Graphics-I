#pragma once

class WhittedTracer : public Tracer
{
public:
	WhittedTracer(int recursionLimit, Shader& shader);
	virtual Vector3 trace(Scene& scene, Camera &camera, float x, float y, OmniLight& light, Cubemap& cubemap) override;

private:
	Vector3 trace(Scene& scene, Ray ray, OmniLight& light, Cubemap& cubemap, int currentDepth, int depth);

	Shader& shader;
	int recursionLimit;
};