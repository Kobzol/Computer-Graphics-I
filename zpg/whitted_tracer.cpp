#include "stdafx.h"

WhittedTracer::WhittedTracer(int recursionLimit, Shader& shader): recursionLimit(recursionLimit), shader(shader)
{

}

Vector3 WhittedTracer::trace(Scene& scene, Camera &camera, float x, float y, OmniLight& light, Cubemap& cubemap)
{
	const int count = 5;
	float positions[count][2] = {
		{ -0.5f, -0.5f },
		{ 0.5f, -0.5f },
		{ 0.0f, 0.0f },
		{ 0.5f, 0.5f },
		{ -0.5f, 0.5f }
	};

	Vector3 color(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < count; i++)
	{
		color += this->trace(scene, camera.GenerateRay(x + positions[i][0], y + positions[i][1]), light, cubemap, 0, this->recursionLimit);
	}

	return color / static_cast<float>(count);
}

float switchIor(float ior)
{
	return 2.5f - ior;	// 1.0 -> 1.5, 1.5 -> 1.0
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

Vector3 WhittedTracer::trace(Scene& scene, Ray ray, OmniLight& light, Cubemap& cubemap, int depth, int maxDepth)
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
	Vector3 normal = triangle.normal(ray);	// turn the normal immediately
	Vector3 point = ray.getIntersectPoint();

	if (normal.DotProduct(-rayDir) < 0)
	{
		normal = -normal;
	}

	Ray reflectedRay(point, rayDir.reflect(normal), 0.01f);
	reflectedRay.refractionIndex = ray.refractionIndex;

	Material* mat = triangle.surface()->get_material();
	Vector3 eye = point - rayDir;

	std::string matName = mat->get_name();
	Vector3 specular = this->shader.calcSpecular(normal, ray, light, eye, 4.0f) * mat->specular;

	// totální odraz, kvazi nahodna cisla, total compendium book
	// fr lambertovsky povrch - albedo / pi (albedo <0, 1>)
	// path tracing - vzdycky jenom jeden paprsek, supersampling
	// kdyz dojde rekurze, vratit 0
	// kdyz vyletime ze sceny, vratit 1 (globalni svetlo) nebo cube mapu
	if (matName == "green_plastic_transparent" && depth < maxDepth)
	{
		float n1 = ray.refractionIndex;
		float n2 = switchIor(ray.refractionIndex);

		Vector3 refractedDir = rayDir.reverseRefract(normal, n1, n2);

		Ray refractedRay(point, refractedDir, 0.01f);
		refractedRay.refractionIndex = n2;

		Vector3 color(0.0f, 0.0f, 0.0f);
		float coefReflect = 1.0f;

		if (!refractedDir.IsZero())
		{
			coefReflect = calcR(n1, n2, normal, rayDir, refractedDir);
			float coefRefract = 1.0f - coefReflect;
			color += this->trace(scene, refractedRay, light, cubemap, depth + 1, maxDepth) * coefRefract * mat->diffuse;
		}

		color += this->trace(scene, reflectedRay, light, cubemap, depth + 1, maxDepth) * coefReflect * mat->diffuse;
		
		return color;
	}
	else
	{
		Vector3 result = light.ambient + this->shader.calculateColor(scene, ray, eye, light);

		if (depth < maxDepth)
		{
			result += this->trace(scene, reflectedRay, light, cubemap, depth + 1, maxDepth) * mat->reflectivity * specular;
		}

		return result;
	}
}
