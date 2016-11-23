#pragma once

class Tracer
{
public:
	virtual ~Tracer();

	virtual Vector3 trace(Scene& scene, Camera &camera, float x, float y, OmniLight& light, Cubemap& cubemap) = 0;
};
