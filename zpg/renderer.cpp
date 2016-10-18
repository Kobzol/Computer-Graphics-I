#include "stdafx.h"

float transformNormal(float v)
{
	return (v / 2.0f) + 0.5f;
}

void Renderer::render(Scene& scene, Camera& camera, Shader& shader, Cubemap& cubemap, cv::Mat& buffer)
{
	OmniLight light = OmniLight(camera.view_from());

	for (int y = 0; y < camera.height(); y++)
	{
		for (int x = 0; x < camera.width(); x++)
		{
			Ray ray = camera.GenerateRay(x, y);
			rtcIntersect(scene.handle(), ray);

			pixel_t color = pixel_t(0.0f, 0.0f, 0.0f);

			if (ray.geomID != RTC_INVALID_GEOMETRY_ID)
			{
				color = shader.calculateColor(scene, ray, light).ToOpenCV();
			}
			else
			{
				Color4 envColor = cubemap.get_texel(Vector3(ray.dir[0], ray.dir[1], ray.dir[2]));
				color = pixel_t(envColor.r, envColor.g, envColor.b);
			}

			buffer.at<pixel_t>(y, x) = pixel_t(color[2], color[1], color[0]);
		}
	}
}
