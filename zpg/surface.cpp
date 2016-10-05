#include "stdafx.h"

Surface * BuildSurface( const std::string & name, std::vector<Vertex> & face_vertices )
{
	const int no_vertices = static_cast< int >( face_vertices.size() );

	assert( ( no_vertices > 0 ) && ( no_vertices % 3 == 0 ) );

	const int no_triangles = no_vertices / 3;

	Surface * surface = new Surface( name, no_triangles );

	// kopírování dat
	for ( int i = 0; i < no_triangles; ++i )
	{		
		surface->get_triangles()[i] = Triangle( face_vertices[i * 3],
			face_vertices[i * 3 + 1], face_vertices[i * 3 + 2], surface );
	}

	return surface;
}

Surface::Surface()
{
	n_ = 0;
	triangles_ = NULL;
}

Surface::Surface( const std::string & name, const int n )
{
	assert( n > 0 );

	name_ = name;

	n_ = n;
	triangles_ = new Triangle[n_];
}

Surface::~Surface()
{
	SAFE_DELETE_ARRAY( triangles_ );
	n_ = 0;
}

Triangle & Surface::get_triangle( const int i )
{
	return triangles_[i];
}

Triangle * Surface::get_triangles()
{
	return triangles_;
}

std::string Surface::get_name()
{
	return name_;
}

int Surface::no_triangles()
{
	return n_;
}

int Surface::no_vertices()
{
	return 3 * n_;
}

Matrix4x4 * Surface::transformation()
{
	return &transformation_;
}

void Surface::set_material( Material * material )
{
	material_ = material;
}

Material * Surface::get_material() const
{
	return material_;
}
