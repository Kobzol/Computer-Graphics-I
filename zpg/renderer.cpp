#include "stdafx.h"

static int SAME_HIT_COUNTER = 0;

Vector3 trace(Scene& scene, Ray ray, const Vector3& eye, OmniLight& light, LambertShader& shader, Cubemap& cubemap, int iteration)
{
	if (iteration <= 0)
	{
		return Vector3(0.0f, 0.0f, 0.0f);
	}

	Vector3 rayDir = Vector3(ray.dir);
	rayDir.Normalize();
	rtcIntersect(scene.handle(), ray);
	if (ray.geomID == RTC_INVALID_GEOMETRY_ID)
	{
		Color4 envColor = cubemap.get_texel(rayDir);
		return Vector3(envColor.r, envColor.g, envColor.b);
	}
	
	Triangle& triangle = scene.getTriangle(ray);
	Vector3 point = ray.getIntersectPoint();
	Ray reflectedRay(point, rayDir.reflect(triangle.normal(ray)), 0.01f);

	Vector3 toLight = light.position - point;
	Ray rayToLight(point, toLight, 0.01f, toLight.L2Norm());
	rtcOccluded(scene.handle(), rayToLight);

	Material* mat = triangle.surface()->get_material();
	Vector3 shadow = Vector3(1.0f, 1.0f, 1.0f);

	if (rayToLight.geomID != RTC_INVALID_GEOMETRY_ID)
	{
		shadow = Vector3(0.0f, 0.0f, 0.0f);
	}
	
	return shader.calculateColor(scene, ray, eye, light) * shadow
		+ trace(scene, reflectedRay, point, light, shader, cubemap, iteration - 1) * mat->reflectivity * mat->specular * 0.5f;
}

void Renderer::render(Scene& scene, Camera& camera, Shader& shader, Cubemap& cubemap, cv::Mat& buffer)
{
	OmniLight light = OmniLight(camera.view_from());  // OmniLight(Vector3(70.0f, -40.0f, 100.0f));

	for (int y = 0; y < camera.height(); y++)
	{
		for (int x = 0; x < camera.width(); x++)
		{
			Ray ray = camera.GenerateRay(x, y);
			Vector3 color = trace(scene, ray, camera.view_from(), light, (LambertShader&)shader, cubemap, 3);
			buffer.at<pixel_t>(y, x) = pixel_t(color.z, color.y, color.x);
		}
	}

	std::cout << SAME_HIT_COUNTER << std::endl;
}
