#pragma once

class WhittedTracer : public Tracer
{
public:
	WhittedTracer(Shader& shader);
	virtual Vector3 trace(Scene& scene, Ray ray, OmniLight& light, Cubemap& cubemap, int depth) override;

private:
	Vector3 trace(Scene& scene, Ray ray, OmniLight& light, Cubemap& cubemap, int currentDepth, int depth);

	Shader& shader;
};