#include "stdafx.h"

void Vector4::Print()
{
	printf( "(%0.3f, %0.3f, %0.3f, %0.3f)\n", x, y, z, w );
}

// --- operátory ------

Vector4 operator-( const Vector4 & v )
{
	return Vector4( -v.x, -v.y, -v.z, -v.w );
}

Vector4 operator+( const Vector4 & u, const Vector4 & v )
{
	return Vector4( u.x + v.x, u.y + v.y, u.z + v.z, u.w + v.w );
}

Vector4 operator-( const Vector4 & u, const Vector4 & v )
{
	return Vector4( u.x - v.x, u.y - v.y, u.z - v.z, u.w - v.w );
}

Vector4 operator*( const Vector4 & v, const float a )
{
	return Vector4( a * v.x, a * v.y, a * v.z, a * v.w );
}

Vector4 operator*( const float a, const Vector4 & v )
{
	return Vector4( a * v.x, a * v.y, a * v.z, a * v.w ); 		
}

Vector4 operator*( const Vector4 & u, const Vector4 & v )
{
	return Vector4( u.x * v.x, u.y * v.y, u.z * v.z, u.w * v.w );
}

Vector4 operator/( const Vector4 & v, const float a )
{
	return v * ( 1 / a );
}

void operator+=( Vector4 & u, const Vector4 & v )
{
	u.x += v.x;
	u.y += v.y;
	u.z += v.z;
	u.w += v.w;
}

void operator-=( Vector4 & u, const Vector4 & v )
{
	u.x -= v.x;
	u.y -= v.y;
	u.z -= v.z;
	u.w -= v.w;
}

void operator*=( Vector4 & v, const float a )
{
	v.x *= a;
	v.y *= a;
	v.z *= a;
	v.w *= a;
}

void operator/=( Vector4 & v, const float a )
{
	const float r = 1 / a;

	v.x *= r;
	v.y *= r;
	v.z *= r;
	v.w *= r;
}
