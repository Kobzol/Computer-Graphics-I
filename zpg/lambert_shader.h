#pragma once

#include "stdafx.h"

class LambertShader : public Shader
{
public:
	LambertShader(Vector3 color);

	virtual Vector3 calculateColor(Scene& scene, Ray& ray, OmniLight& light) override;

private:
	Vector3 color;
};