#include "stdafx.h"

enum class CubemapDir : int
{
	POS_X = 0,
	NEG_X = 1,
	POS_Y = 2,
	NEG_Y = 3,
	POS_Z = 4,
	NEG_Z = 5
};

Cubemap::Cubemap(const std::string& path, std::string extension)
{
	for (size_t i = 0; i < 6; i++)
	{
		std::string texturePath = path + "_" + std::to_string(i) + extension;
		this->textures[i] = std::unique_ptr<Texture>(LoadTexture(texturePath.c_str()));
	}
}

Color4 Cubemap::get_texel(Vector3& direction)
{
	int largestIndex = direction.LargestComponent(true);
	int cubeIndex = largestIndex * 2;

	if (direction.data[largestIndex] < 0)
	{
		cubeIndex += 1;
	}

	float u, v;
	float scale = 1.0f / abs(direction.data[largestIndex]);
	switch (static_cast<CubemapDir>(cubeIndex))
	{
		case CubemapDir::POS_X:
		case CubemapDir::NEG_X:
			u = direction.y * -1 * this->negative(cubeIndex);
			v = direction.z;
			break;
		case CubemapDir::POS_Y:
		case CubemapDir::NEG_Y:
			u = direction.x * this->negative(cubeIndex);
			v = direction.z;
			break;
		case CubemapDir::POS_Z:
		case CubemapDir::NEG_Z:
			u = direction.x;
			v = direction.y * -1 * this->negative(cubeIndex);
			break;
	}

	u = this->normalize(u * scale);
	v = this->normalize(v * scale);

	return this->textures[cubeIndex]->get_texel(u, v);
}
