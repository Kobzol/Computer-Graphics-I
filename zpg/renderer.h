#pragma once

class Renderer
{
public:
	void render(Scene& scene, Camera& camera, Tracer& tracer, Cubemap& cubemap, cv::Mat& buffer);
};