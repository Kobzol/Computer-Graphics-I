#pragma once

#include "stdafx.h"

class LambertShader : public Shader
{
public:
	virtual Vector3 calculateColor(Scene& scene, Ray& ray, const Vector3& eye, OmniLight& light) override;
};