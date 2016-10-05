#include "stdafx.h"

float Vector2::L2Norm()
{
	return sqrt( SQR( x ) + SQR( y ) );
}

float Vector2::SqrL2Norm()
{
	return SQR( x ) + SQR( y );
}

void Vector2::Normalize()
{
	const float norm = SqrL2Norm();

	if ( norm != 0 )
	{
		const float rn = 1 / sqrt( norm );

		x *= rn;
		y *= rn;			
	}
}

float Vector2::CrossProduct( const Vector2 & v )
{
	return x * v.y - y * v.x;
}

Vector2 Vector2::CrossProduct()
{
	return Vector2( y, -x );
}

float Vector2::DotProduct( const Vector2 & v )
{
	return x * v.x + y * v.y;
}

Vector2 Vector2::Rotate( const float phi )
{
	const float cos_phi = cos( phi );
	const float sin_phi = sin( phi );

	return Vector2( x * cos_phi + y * sin_phi,
		-x * sin_phi + y * cos_phi );
}

char Vector2::LargestComponent( const bool absolute_value )
{
	const Vector2 d = ( absolute_value )? Vector2( abs( x ), abs( y ) ) : *this;

	if ( d.x > d.y )
	{
		return 0 ;
	}
	else
	{
		return 1;
	}

	return -1;
}

/*void Print()
{
printf( "(%0.3f, %0.3f)\n", x, y );
}*/

// --- operátory ------

Vector2 operator-( const Vector2 & v )
{
	return Vector2( -v.x, -v.y );
}

Vector2 operator+( const Vector2 & u, const Vector2 & v )
{
	return Vector2( u.x + v.x, u.y + v.y );
}

Vector2 operator-( const Vector2 & u, const Vector2 & v )
{
	return Vector2( u.x - v.x, u.y - v.y );
}

Vector2 operator*( const Vector2 & v, const float a )
{
	return Vector2( a * v.x, a * v.y ); 		
}

Vector2 operator*( const float a, const Vector2 & v )
{
	return Vector2( a * v.x, a * v.y ); 		
}

Vector2 operator*( const Vector2 & u, const Vector2 & v )
{
	return Vector2( u.x * v.x, u.y * v.y );
}

Vector2 operator/( const Vector2 & v, const float a )
{
	return v * ( 1 / a );
}

void operator+=( Vector2 & u, const Vector2 & v )
{
	u.x += v.x;
	u.y += v.y;	
}

void operator-=( Vector2 & u, const Vector2 & v )
{
	u.x -= v.x;
	u.y -= v.y;
}

void operator*=( Vector2 & v, const float a )
{
	v.x *= a;
	v.y *= a;
}

void operator/=( Vector2 & v, const float a )
{
	const float r = 1 / a;

	v.x *= r;
	v.y *= r;	
}
