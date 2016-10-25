#pragma once

#include <embree2/rtcore.h>
#include "surface.h"

// struktury pro ukládání dat pro Embree
namespace embree_structs
{
	struct Vertex { float x, y, z, a; };
	typedef Vertex Normal;
	struct Triangle { int v0, v1, v2; };
};

class Scene
{
public:
	Scene(RTCDevice& device);
	~Scene();

	void addSurface(Surface* surface);
	void addMaterial(Material* material);

	Surface* getSurface(int index);
	Material* getMaterial(int index);

	Triangle& getTriangle(const Ray& ray);
		
	void commit();

	RTCScene& handle();

private:
	RTCScene scene;

	std::vector<Surface*> surfaces;
	std::vector<Material*> materials;
};