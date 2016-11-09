#include "stdafx.h"

Vector3 LambertShader::calculateColor(Scene& scene, Ray& ray, const Vector3& eye, OmniLight& light)
{
	Triangle& triangle = scene.getTriangle(ray);
	Vector3 normal = triangle.normal(ray);
	float diff = this->calcDiffuse(normal, ray, light);

	return ((diff * triangle.getTextureOrMatDiff(ray)) * this->calcShadow(scene, ray.getIntersectPoint(), light)) * (1.0f - triangle.surface()->get_material()->reflectivity);
}
