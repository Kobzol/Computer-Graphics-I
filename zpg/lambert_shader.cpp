#include "stdafx.h"

Vector3 LambertShader::calculateColor(Scene& scene, Ray& ray, const Vector3& eye, OmniLight& light)
{
	Triangle& triangle = scene.getTriangle(ray);
	Vector3 normal = triangle.normal(ray);

	return (this->calcDiffuseColor(scene, normal, ray, light) * this->calcShadow(scene, ray.getIntersectPoint(), light)) * (1.0f - triangle.surface()->get_material()->reflectivity);
}
