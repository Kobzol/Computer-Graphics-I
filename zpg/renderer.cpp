#include "stdafx.h"
#include "tracer.h"

void Renderer::render(Scene& scene, Camera& camera, Tracer& tracer, Cubemap& cubemap, cv::Mat& buffer)
{
	OmniLight light = OmniLight(camera.view_from());

#pragma omp parallel for
	for (int y = 0; y < camera.height(); y++)
	{
		for (int x = 0; x < camera.width(); x++)
		{
			Ray ray = camera.GenerateRay(x, y);
			
			Vector3 color(0.0f, 0.0f, 0.0f);
			for (int i = 0; i < tracer.getSamplesPerPixel(); i++)
			{
				color += tracer.trace(scene, ray, light, cubemap, 8);
			}
			buffer.at<pixel_t>(y, x) = pixel_t(color.z, color.y, color.x);
		}
	}
}
