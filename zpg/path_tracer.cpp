#include "stdafx.h"
#include <math.h>

PathTracer::PathTracer(int recursionLimit, Shader& shader, int samplesPerPixel): recursionLimit(recursionLimit), shader(shader), samplesPerPixel(samplesPerPixel)
{

}

Vector3 PathTracer::trace(Scene& scene, Camera &camera, float x, float y, OmniLight& light, Cubemap& cubemap)
{
	float width = std::sqrt(this->samplesPerPixel);
	float moveOffset = 1.0f / width;

	Vector3 color(0.0f, 0.0f, 0.0f);
	for (float i = -0.5f; i < 0.5f; i += moveOffset)
	{
		for (float j = -0.5f; j < 0.5f; j += moveOffset)
		{
			color += this->trace(scene, camera.GenerateRay(x + i, y + j), light, cubemap, 0, this->recursionLimit);
		}
	}

	return color / static_cast<float>(this->samplesPerPixel);
}

static float switchIor(float ior)
{
	return 2.5f - ior;	// 1.0 -> 1.5, 1.5 -> 1.0
}

static float calcRS(float n1, float n2, float cosi, float cost)
{
	float val = (n1 * cosi - n2 * cost) / (n1 * cosi + n2 * cost);
	return val * val;
}
static float calcRP(float n1, float n2, float cosi, float cost)
{
	float val = (n1 * cost - n2 * cosi) / (n1 * cost + n2 * cosi);
	return val * val;
}

static float schlick(float n1, float n2, Vector3 rayDir, Vector3 normal)
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
static float calcR(float n1, float n2, Vector3 normal, Vector3 ray, Vector3 refracted)
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

inline Vector3 orthogonal(const Vector3& v)
{
	return (abs(v.x) > abs(v.z)) ? Vector3(-v.y, v.x, 0.0f) : Vector3(0.0f, -v.z, v.y);
}
inline Vector3 transformVector(const Vector3& normal, const Vector3& direction)
{
	// normal je osa z
	Vector3 o1 = orthogonal(normal); // o1 je pomocna osa x
	o1.Normalize();
	Vector3 o2 = o1.CrossProduct(normal); // o2 je pomocna osa y
	o2.Normalize();

	Vector3 direction_ws = Vector3(
		o1.x * direction.x + o2.x * direction.y + normal.x * direction.z,
		o1.y * direction.x + o2.y * direction.y + normal.y * direction.z,
		o1.z * direction.x + o2.z * direction.y + normal.z * direction.z
	); // direction je vstupni vektor, ktery chcete "posadit" do ws
	direction_ws.Normalize();

	return direction_ws;
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
	Vector3 specular = this->shader.calcSpecular(normal, ray, light, eye, 3.0f) * mat->specular * mat->reflectivity;

	if (depth < maxDepth)
	{
		if (mat->get_name() == "green_plastic_transparent")	// glass
		{
			float n1 = ray.refractionIndex;
			float n2 = switchIor(ray.refractionIndex);

			Vector3 refractedDir = rayDir.reverseRefract(normal, n1, n2);

			Ray refractedRay(point, refractedDir, 0.01f);
			refractedRay.refractionIndex = n2;

			float coefReflect = calcR(n1, n2, normal, rayDir, refractedDir);
			float coefRefract = 1.0f - coefReflect;

			if (refractedDir.IsZero())
			{
				coefRefract = 0.0f;
				coefReflect = 1.0f;
			}

			if (this->distribution(this->generator) <= coefReflect) // reflect
			{
				Ray reflectedRay = Ray(point, rayDir.reflect(normal), 0.01f);
				reflectedRay.refractionIndex = ray.refractionIndex;

				return this->trace(scene, reflectedRay, light, cubemap, depth + 1, maxDepth) * mat->diffuse;
			}
			else // refract
			{
				return this->trace(scene, refractedRay, light, cubemap, depth + 1, maxDepth) * mat->diffuse;
			}
		}
		else if (this->distribution(this->generator) < mat->reflectivity) // reflect
		{
			Vector3 reflected = rayDir.reflect(normal);
			Ray reflectedRay(point, reflected, 0.01f);
			reflectedRay.refractionIndex = ray.refractionIndex;

			return this->trace(scene, reflectedRay, light, cubemap, depth + 1, maxDepth) * specular;
		}
		else // diffuse
		{
			Vector3 vec = this->generateRandomAngle(normal);
			//Vector3 vec = this->generateRandomVec(normal);
			float cosTheta = normal.DotProduct(vec);
			float pdf = cosTheta / M_PI;

			Ray randomRay(point, vec, 0.01f);
			randomRay.refractionIndex = ray.refractionIndex;

			Vector3 color = this->trace(scene, randomRay, light, cubemap, depth + 1, maxDepth);
			color = color * this->shader.calcDiffuseColor(scene, normal, ray, light) * cosTheta;

			return color / pdf;
		}
	}

	return Vector3(0.0f, 0.0f, 0.0f);
}

// vzorec 35 z compendia
// zrcadlo - vygenerovat jenom paprsek wi (vec otoceny o normalu, s PDF = 1)
// prepocet souradneho systemu (funkce pro kolmy vektor)
// if (n*s) > psi - kontrola n * s
float PathTracer::brdf(const Vector3& eye, const Vector3& light, const Vector3& normal)
{
	return 0.4f / M_PI;	// albedo
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
Vector3 PathTracer::generateRandomAngle(const Vector3& normal)
{
	double r1 = this->distribution(this->generator);
	double r2 = this->distribution(this->generator);

	double x = std::cos(2 * M_PI * r1) * std::sqrt(1 - r2);
	double y = std::sin(2 * M_PI * r1) * std::sqrt(1 - r2);
	double z = std::sqrt(r2);

	Vector3 vec(x, y, z);
	vec.Normalize();

	if (normal.DotProduct(vec) < 0.0f)
	{
		vec = -vec;
	}

	return transformVector(normal, vec);
}
