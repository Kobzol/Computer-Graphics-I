#pragma once

class Tracer
{
public:
	Tracer(int samplesPerPixel = 1);
	virtual ~Tracer();

	virtual Vector3 trace(Scene& scene, Ray ray, OmniLight& light, Cubemap& cubemap, int depth) = 0;
	virtual int getSamplesPerPixel() const;

private:
	int samplesPerPixel;
};
