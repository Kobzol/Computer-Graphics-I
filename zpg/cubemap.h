#pragma once

#include <memory>

class Cubemap
{
public:
	Cubemap(const std::string& path, std::string extension = ".jpg");

	Color4 get_texel(Vector3& direction);

private:
	float normalize(float value)
	{
		return (value + 1) / 2.0f;
	}
	int negative(int index)
	{
		return (index % 2 == 0) ? 1 : -1;
	}

	std::unique_ptr<Texture> textures[6];
};