#include "stdafx.h"
#include "shader.h"

Vector3 Shader::getIntersectPosition(Ray& ray)
{
	return ray.eval(ray.tfar);
}

Vector3 Shader::getNormal(Scene& scene, Ray& ray)
{
	Surface* surface = scene.getSurface(ray.geomID);
	Triangle& triangle = surface->get_triangle(ray.primID);
	return this->getNormal(triangle, ray);
}

Vector3 Shader::getNormal(Triangle& triangle, Ray& ray)
{
	Vector3 normal = triangle.normal(ray.u, ray.v);
	normal.Normalize();
	return normal;
}

Triangle& Shader::getTriangle(Scene& scene, Ray& ray)
{
	Surface* surface = scene.getSurface(ray.geomID);
	return surface->get_triangle(ray.primID);
}

float Shader::calcDiffuse(const Vector3& normal, Ray& ray, OmniLight& light)
{
	Vector3 toLight = light.position - this->getIntersectPosition(ray);
	toLight.Normalize();

	return normal.DotProduct(toLight);
}

float Shader::calcSpecular(const Vector3& normal, Ray& ray, OmniLight& light, const Vector3& eye, float exponent)
{
	Vector3 position = this->getIntersectPosition(ray);
	
	Vector3 toLight = light.position - position;
	toLight.Normalize();

	Vector3 toEye = eye - position;
	toEye.Normalize();

	Vector3 half = toLight + toEye;
	half.Normalize();

	return pow(normal.DotProduct(half), exponent);
}

Vector3 Shader::getTextureOrMatDiff(Scene & scene, Ray & ray)
{
	Triangle& triangle = this->getTriangle(scene, ray);
	Surface* surface = triangle.surface();
	Material* mat = surface->get_material();
	Texture* texture = mat->get_texture(mat->kDiffuseMapSlot);

	Vector3 color = mat->diffuse;
	if (texture != nullptr)
	{
		Vector2 texCoords = triangle.texture_coord(ray.u, ray.v);
		Color4 col = texture->get_texel(texCoords.x, texCoords.y);
		color = Vector3(col.r, col.g, col.b);
	}

	return color;
}
