#include "stdafx.h"
#include "scene.h"

Scene::Scene(RTCDevice& device)
{
	this->scene = rtcDeviceNewScene(device, RTC_SCENE_STATIC | RTC_SCENE_HIGH_QUALITY, RTC_INTERSECT1);
}

Scene::~Scene()
{
	rtcDeleteScene(this->scene);
}

static void filter_intersection(void* user_ptr, Ray& ray)
{
	if (ray.tfar < 0.01f)
	{
		ray.geomID = RTC_INVALID_GEOMETRY_ID;
	}
}

void Scene::addSurface(Surface* surface)
{
	this->surfaces.push_back(surface);
	unsigned geom_id = rtcNewTriangleMesh(scene, RTC_GEOMETRY_STATIC, surface->no_triangles(), surface->no_vertices());

	rtcSetUserData(scene, geom_id, &surface);
	rtcSetIntersectionFilterFunction(scene, geom_id, reinterpret_cast< RTCFilterFunc >(&filter_intersection));

	// kopírování samotných vertexů trojúhelníků
	embree_structs::Vertex* vertices = static_cast<embree_structs::Vertex*>(rtcMapBuffer(scene, geom_id, RTC_VERTEX_BUFFER));

	for (int t = 0; t < surface->no_triangles(); t++)
	{
		for (int v = 0; v < 3; v++)
		{
			embree_structs::Vertex& vertex = vertices[t * 3 + v];
			Vector3 position = surface->get_triangle(t).vertex(v).position;

			vertex.x = position.x;
			vertex.y = position.y;
			vertex.z = position.z;
		}
	}

	rtcUnmapBuffer(scene, geom_id, RTC_VERTEX_BUFFER);

	// vytváření indexů vrcholů pro jednotlivé trojúhelníky
	embree_structs::Triangle* triangles = static_cast<embree_structs::Triangle*>(rtcMapBuffer(scene, geom_id, RTC_INDEX_BUFFER));

	for (int t = 0, v = 0; t < surface->no_triangles(); ++t)
	{
		embree_structs::Triangle& triangle = triangles[t];

		triangle.v0 = v++;
		triangle.v1 = v++;
		triangle.v2 = v++;
	}

	rtcUnmapBuffer(scene, geom_id, RTC_INDEX_BUFFER);
}
void Scene::addMaterial(Material* material)
{
	this->materials.push_back(material);
}

Surface* Scene::getSurface(int index)
{
	return this->surfaces[index];
}

Material* Scene::getMaterial(int index)
{
	return this->materials[index];
}

Triangle& Scene::getTriangle(const Ray& ray)
{
	Surface* surface = this->getSurface(ray.geomID);
	return surface->get_triangle(ray.primID);
}

void Scene::commit()
{
	rtcCommit(this->scene);
}

RTCScene& Scene::handle()
{
	return this->scene;
}
