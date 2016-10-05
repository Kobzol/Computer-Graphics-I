#include "stdafx.h"

LambertShader::LambertShader(Vector3 color): color(color)
{

}

Vector3 LambertShader::calculateColor(Scene& scene, Ray& ray, OmniLight& light)
{
	Vector3 normal = this->getNormal(scene, ray);
	float diff = this->calcDiffuse(normal, ray, light);
	float spec = this->calcSpecular(normal, ray, light, light.position, 8.0f);
	Vector3 specColor = Vector3(1.0f, 1.0f, 1.0f);

	Triangle& triangle = this->getTriangle(scene, ray);
	Surface* surface = triangle.surface();
	Material* mat = surface->get_material();
	
	/*Texture* texture = mat->get_texture(mat->kDiffuseMapSlot);
	Color4 color = texture->get_texel(ray.u, ray.v);
	Vector3 col = Vector3(color.r, color.g, color.b);*/

	return (diff * mat->diffuse) + (spec * specColor);
}
