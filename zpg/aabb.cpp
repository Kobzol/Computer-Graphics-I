#include "stdafx.h"

AABB::AABB()
{
	bounds_[0] = Vector3( REAL_MAX, REAL_MAX, REAL_MAX );
	bounds_[1] = Vector3( REAL_MIN, REAL_MIN, REAL_MIN );
}

AABB::AABB( const Vector3 & p0, const Vector3 & p1 )
{
	bounds_[0] = p0;
	bounds_[1] = p1;
}

void AABB::Merge( const AABB & aabb )
{
	bounds_[0].x = MIN( bounds_[0].x, aabb.bounds_[0].x );
	bounds_[0].y = MIN( bounds_[0].y, aabb.bounds_[0].y );
	bounds_[0].z = MIN( bounds_[0].z, aabb.bounds_[0].z );

	bounds_[1].x = MAX( bounds_[1].x, aabb.bounds_[1].x );
	bounds_[1].y = MAX( bounds_[1].y, aabb.bounds_[1].y );
	bounds_[1].z = MAX( bounds_[1].z, aabb.bounds_[1].z );
}

void AABB::Merge( const Vector3 & p )
{
	Merge( AABB( p, p ) );
}

char AABB::dominant_axis() const
{
	return ( bounds_[1] - bounds_[0] ).LargestComponent();
}

Vector3 AABB::center() const
{
	return ( bounds_[0] + bounds_[1] ) * static_cast<TYPE_REAL>( 0.5 );
}

TYPE_REAL AABB::surface_area() const
{
	const Vector3 d = bounds_[1] - bounds_[0];

	return 2 * ( d.x * d.y + d.x * d.z + d.y * d.z );
}

Vector3 AABB::lower_bound() const
{
	return bounds_[0];
}

Vector3 AABB::upper_bound() const
{
	return bounds_[1];
}
