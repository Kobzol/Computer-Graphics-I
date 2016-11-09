#include "stdafx.h"

float switchIor(float ior)
{
	if (abs(ior - 1.0f) < 0.1f) return 1.5f;
	else return 1.0f;
}

float calcRS(float n1, float n2, float cosi, float cost)
{
	float val = (n1 * cosi - n2 * cost) / (n1 * cosi + n2 * cost);
	return val * val;
}
float calcRP(float n1, float n2, float cosi, float cost)
{
	float val = (n1 * cost - n2 * cosi) / (n1 * cost + n2 * cosi);
	return val * val;
}

float schlick(float n1, float n2, Vector3 rayDir, Vector3 normal)
{
	float r0 = (n1 - n2) / (n1 + n2);
	r0 *= r0;

	rayDir.Normalize();
	normal.Normalize();

	float cosi = rayDir.DotProduct(normal);

	if (cosi < 0)
	{
		cosi = rayDir.DotProduct(-normal);
	}

	return r0 + (1 - r0) * std::pow((1 - cosi), 5);
}

// switch direction
float calcR(float n1, float n2, Vector3 normal, Vector3 ray, Vector3 refracted)
{
	return schlick(n1, n2, ray, normal);
	ray.Normalize();
	normal.Normalize();
	refracted.Normalize();

	float cosi = (ray).DotProduct(normal);
	if (cosi < 0)
	{
		cosi = (ray).DotProduct(-normal);
	}

	float cost = (-normal).DotProduct(refracted);
	if (cost < 0)
	{
		cost = (normal).DotProduct(refracted);
	}

	return 0.5f * (calcRS(n1, n2, cosi, cost) + calcRP(n1, n2, cosi, cost));
}

Vector3 trace(Scene& scene, Ray ray, OmniLight& light, LambertShader& shader, Cubemap& cubemap, int depth, int maxDepth)
{
	Vector3 rayDir = Vector3(ray.dir);
	rayDir.Normalize();

	if (depth >= maxDepth)
	{
		Color4 envColor = cubemap.get_texel(rayDir);
		return Vector3(envColor.r, envColor.g, envColor.b);
	}

	rtcIntersect(scene.handle(), ray);

	if (!ray.hasCollided())
	{
		Color4 envColor = cubemap.get_texel(rayDir);
		return Vector3(envColor.r, envColor.g, envColor.b);
	}
	
	Triangle& triangle = scene.getTriangle(ray);
	Vector3 normal = triangle.normal(ray);
	Vector3 point = ray.getIntersectPoint();

	Ray reflectedRay(point, rayDir.reflect(normal), 0.01f);
	reflectedRay.refractionIndex = ray.refractionIndex;

	Material* mat = triangle.surface()->get_material();
	Vector3 eye = point - rayDir;

	std::string matName = mat->get_name();
	Vector3 specular = shader.calcSpecular(normal, ray, light, eye, 4.0f) * mat->specular;

	if (matName == "green_plastic_transparent" && depth < maxDepth)
	{
		float n1 = ray.refractionIndex;
		float n2 = switchIor(ray.refractionIndex);
		Vector3 refractedDir = rayDir.refract(normal, n1, n2);
		Ray refractedRay(point, refractedDir, 0.01f);
		refractedRay.refractionIndex = n2;

		float coefReflect = calcR(n1, n2, normal, rayDir, refractedDir);
		float coefRefract = 1.0f - coefReflect;
		
		return	trace(scene, reflectedRay, light, shader, cubemap, depth + 1, maxDepth) * coefReflect * specular +
				trace(scene, refractedRay, light, shader, cubemap, depth + 1, maxDepth) * coefRefract * mat->diffuse;
	}
	else
	{
		Vector3 result = light.ambient + shader.calculateColor(scene, ray, eye, light);

		if (depth < maxDepth)
		{
			result += trace(scene, reflectedRay, light, shader, cubemap, depth + 1, maxDepth) * mat->reflectivity * specular;
		}

		return result;
	}
}

void Renderer::render(Scene& scene, Camera& camera, Shader& shader, Cubemap& cubemap, cv::Mat& buffer)
{
	OmniLight light = OmniLight(camera.view_from());

#pragma omp parallel for
	for (int y = 0; y < camera.height(); y++)
	{
		for (int x = 0; x < camera.width(); x++)
		{
			Ray ray = camera.GenerateRay(x, y);
			Vector3 color = trace(scene, ray, light, (LambertShader&)shader, cubemap, 0, 4);
			buffer.at<pixel_t>(y, x) = pixel_t(color.z, color.y, color.x);
		}
	}
}
