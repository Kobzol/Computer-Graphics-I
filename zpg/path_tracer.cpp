#include "stdafx.h"
#include <math.h>

PathTracer::PathTracer(int samplesPerPixel, Shader& shader): Tracer(samplesPerPixel), shader(shader)
{

}

Vector3 PathTracer::trace(Scene& scene, Ray ray, OmniLight& light, Cubemap& cubemap, int depth)
{
	return this->trace(scene, ray, light, cubemap, 0, depth);
}

Vector3 PathTracer::trace(Scene& scene, Ray ray, OmniLight& light, Cubemap& cubemap, int depth, int maxDepth)
{
	if (depth >= maxDepth)
	{
		return Vector3(0.0f, 0.0f, 0.0f);
	}

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
	Vector3 point = ray.getIntersectPoint();
	Material* mat = triangle.surface()->get_material();
	Vector3 eye = point - rayDir;
	Vector3 vec = this->generateRandomVec(normal);
	Vector3 color = this->brdf(eye, vec) * normal.DotProduct(vec) * this->shader.calcDiffuse(normal, ray, light) * mat->diffuse;
	color *= this->shader.calcShadow(scene, point, light);
	
	Ray randomRay(point, vec, 0.01f);
	color += this->trace(scene, randomRay, light, cubemap, depth + 1, depth);

	return color;
}

float PathTracer::brdf(const Vector3& eye, const Vector3& light)
{
	return 0.5f / M_PI;	// albedo
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
