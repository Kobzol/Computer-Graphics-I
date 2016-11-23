#include "stdafx.h"
#include <math.h>

PathTracer::PathTracer(int recursionLimit, Shader& shader, int samplesPerPixel): recursionLimit(recursionLimit), shader(shader), samplesPerPixel(samplesPerPixel)
{

}

Vector3 PathTracer::trace(Scene& scene, Camera &camera, float x, float y, OmniLight& light, Cubemap& cubemap)
{
	Vector3 color(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < this->samplesPerPixel; i++)
	{
		float jittedX = x +this->distribution(this->generator);
		float jittedY = y +this->distribution(this->generator);
		color += this->trace(scene, camera.GenerateRay(jittedX, jittedY), light, cubemap, 0, this->recursionLimit);
	}
	return color / (float) this->samplesPerPixel;
}

Vector3 PathTracer::trace(Scene& scene, Ray ray, OmniLight& light, Cubemap& cubemap, int depth, int maxDepth)
{
	Vector3 rayDir = Vector3(ray.dir);
	rayDir.Normalize();

	rtcIntersect(scene.handle(), ray);
	if (!ray.hasCollided())
	{
		Color4 envColor = cubemap.get_texel(rayDir);
		return Vector3(envColor.r, envColor.g, envColor.b);
	}

	Triangle& triangle = scene.getTriangle(ray);
	Vector3 normal = triangle.normal(ray);

	if (-rayDir.DotProduct(normal) < 0.0f)
	{
		normal = -normal;
	}

	Vector3 point = ray.getIntersectPoint();
	Material* mat = triangle.surface()->get_material();
	Vector3 eye = point - rayDir;
	Vector3 vec = this->generateRandomHemi(normal);
	Vector3 color(0.0f, 0.0f, 0.0f);
	
	if (depth < maxDepth)
	{
		Ray randomRay(point, vec, 0.01f);
		color = Vector3(1.0f, 1.0f, 1.0f) * this->trace(scene, randomRay, light, cubemap, depth + 1, maxDepth) * normal.DotProduct(vec) * this->brdf(eye, vec) * 2 * M_PI;
	}

	return color;
}

float PathTracer::brdf(const Vector3& eye, const Vector3& light)
{
	return 0.25f / M_PI;	// albedo
}
Vector3 PathTracer::generateRandomHemi(const Vector3& normal)
{
	double r1 = this->distribution(this->generator);
	double r2 = this->distribution(this->generator);

	double x = std::cos(2 * M_PI * r1) * std::sqrt(1 - r2 * r2);
	double y = std::sin(2 * M_PI * r1) * std::sqrt(1 - r2 * r2);
	double z = r2;

	Vector3 vec(x, y, z);
	vec.Normalize();

	if (normal.DotProduct(vec) < 0.0f)
	{
		vec = -vec;
	}

	return vec;
}
Vector3 PathTracer::generateRandomVec(const Vector3& normal)
{
	double r1 = this->distribution(this->generator);
	double r2 = this->distribution(this->generator);

	double x = 2 * std::cos(2 * M_PI * r1) * std::sqrt(r2 * (1 - r2));
	double y = 2 * std::sin(2 * M_PI * r1) * std::sqrt(r2 * (1 - r2));
	double z = 1 - 2 * r2;

	Vector3 vec(x, y, z);
	vec.Normalize();
	
	if (normal.DotProduct(vec) < 0.0f)
	{
		vec = -vec;
	}

	return vec;
}
