#include "stdafx.h"
#include "tracer.h"

void Renderer::render(Scene& scene, Camera& camera, Tracer& tracer, Cubemap& cubemap, cv::Mat& buffer)
{
	OmniLight light = OmniLight(camera.view_from());

#ifndef _DEBUG
	#pragma omp parallel for schedule(dynamic, 5) shared(camera, tracer, scene, light, cubemap, buffer)
#endif
	for (int y = 0; y < camera.height(); y++)
	{
		for (int x = 0; x < camera.width(); x++)
		{
			Vector3 color = tracer.trace(scene, camera, x, y, light, cubemap);
			buffer.at<pixel_t>(y, x) = pixel_t(color.z, color.y, color.x);
		}
	}
}
