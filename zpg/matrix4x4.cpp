#include "stdafx.h"

Matrix4x4::Matrix4x4()
{
	for ( int r = 0; r < 4; ++r )
	{
		for ( int c = 0; c < 4; ++c )
		{
			data_[c + r * 4] = static_cast<float>( ( r == c )? 1 : 0 );
		}
	}
}

Matrix4x4::Matrix4x4( const float m00, const float m01, const float m02, const float m03, 
					 const float m10, const float m11, const float m12, const float m13, 
					 const float m20, const float m21, const float m22, const float m23,
					 const float m30, const float m31, const float m32, const float m33 )
{
	m00_ = m00;
	m01_ = m01;
	m02_ = m02;
	m03_ = m03;

	m10_ = m10;
	m11_ = m11;
	m12_ = m12;
	m13_ = m13;

	m20_ = m20;
	m21_ = m21;
	m22_ = m22;
	m23_ = m23;

	m30_ = m30;
	m31_ = m31;
	m32_ = m32;
	m33_ = m33;
}

void Matrix4x4::Transpose()
{	
	float tmp = m01_;
	m01_ = m10_;
	m10_ = tmp;

	tmp = m02_;
	m02_ = m20_;
	m20_ = tmp;

	tmp = m03_;
	m03_ = m30_;
	m30_ = tmp;

	tmp = m12_;
	m12_ = m21_;
	m21_ = tmp;	

	tmp = m13_;
	m13_ = m31_;
	m31_ = tmp;

	tmp = m23_;
	m23_ = m32_;
	m32_ = tmp;
}

void Matrix4x4::EuclideanInverse()
{
	const float m03 = -m00_ * m03_ - m10_ * m13_ - m20_ * m23_;
	const float m13 = -m01_ * m03_ - m11_ * m13_ - m21_ * m23_;
	const float m23 = -m02_ * m03_ - m12_ * m13_ - m22_ * m23_;

	m03_ = m03;
	m13_ = m13;
	m23_ = m23;

	float tmp = m01_;
	m01_ = m10_;
	m10_ = tmp;

	tmp = m02_;
	m02_ = m20_;
	m20_ = tmp;

	tmp = m12_;
	m12_ = m21_;
	m21_ = tmp;	
}

void Matrix4x4::set( const int row, const int column, const float value )
{
	assert( row >= 0 && row < 4 && column >= 0 && column < 4 );

	data_[column + row * 4] = value;
}

float * Matrix4x4::data()
{
	return &data_[0];
}

Matrix4x4 operator*( const Matrix4x4 & a, const Matrix4x4 & b )
{
	return Matrix4x4( a.m00_ * b.m00_ + a.m01_ * b.m10_ + a.m02_ * b.m20_ + a.m03_ * b.m30_,
		a.m00_ * b.m01_ + a.m01_ * b.m11_ + a.m02_ * b.m21_ + a.m03_ * b.m31_,
		a.m00_ * b.m02_ + a.m01_ * b.m12_ + a.m02_ * b.m22_ + a.m03_ * b.m32_,
		a.m00_ * b.m03_ + a.m01_ * b.m13_ + a.m02_ * b.m23_ + a.m03_ * b.m33_,

		a.m10_ * b.m00_ + a.m11_ * b.m10_ + a.m12_ * b.m20_ + a.m13_ * b.m30_,
		a.m10_ * b.m01_ + a.m11_ * b.m11_ + a.m12_ * b.m21_ + a.m13_ * b.m31_,
		a.m10_ * b.m02_ + a.m11_ * b.m12_ + a.m12_ * b.m22_ + a.m13_ * b.m32_,
		a.m10_ * b.m03_ + a.m11_ * b.m13_ + a.m12_ * b.m23_ + a.m13_ * b.m33_,

		a.m20_ * b.m00_ + a.m21_ * b.m10_ + a.m22_ * b.m20_ + a.m23_ * b.m30_,
		a.m20_ * b.m01_ + a.m21_ * b.m11_ + a.m22_ * b.m21_ + a.m23_ * b.m31_,
		a.m20_ * b.m02_ + a.m21_ * b.m12_ + a.m22_ * b.m22_ + a.m23_ * b.m32_,
		a.m20_ * b.m03_ + a.m21_ * b.m13_ + a.m22_ * b.m23_ + a.m23_ * b.m33_,

		a.m30_ * b.m00_ + a.m31_ * b.m10_ + a.m32_ * b.m20_ + a.m33_ * b.m30_,
		a.m30_ * b.m01_ + a.m31_ * b.m11_ + a.m32_ * b.m21_ + a.m33_ * b.m31_,
		a.m30_ * b.m02_ + a.m31_ * b.m12_ + a.m32_ * b.m22_ + a.m33_ * b.m32_,
		a.m30_ * b.m03_ + a.m31_ * b.m13_ + a.m32_ * b.m23_ + a.m33_ * b.m33_ );
}

Vector3 operator*( const Matrix4x4 & a, const Vector3 & b )
{
	return Vector3( a.m00_ * b.x + a.m01_ * b.y + a.m02_ * b.z,
		a.m10_ * b.x + a.m11_ * b.y + a.m12_ * b.z,
		a.m20_ * b.x + a.m21_ * b.y + a.m22_ * b.z );	
}

Vector3 operator*( const Matrix4x4 & a, const Vector4 & b )
{	
	const float w = a.m30_ * b.x + a.m31_ * b.y + a.m32_ * b.z + a.m33_ * b.w;
	assert( w != 0 ); // FIX
	const float w_r = 1 / w;

	return Vector3( ( a.m00_ * b.x + a.m01_ * b.y + a.m02_ * b.z + a.m03_ * b.w ) * w_r,
		( a.m10_ * b.x + a.m11_ * b.y + a.m12_ * b.z + a.m13_ * b.w ) * w_r,
		( a.m20_ * b.x + a.m21_ * b.y + a.m22_ * b.z + a.m23_ * b.w ) * w_r );
}

void Matrix4x4::Print() const
{
	printf( "%0.6f, %0.6f, %0.6f, %0.6f\n", m00_, m01_, m02_, m03_ );
	printf( "%0.6f, %0.6f, %0.6f, %0.6f\n", m10_, m11_, m12_, m13_ );
	printf( "%0.6f, %0.6f, %0.6f, %0.6f\n", m20_, m21_, m22_, m23_ );
	printf( "%0.6f, %0.6f, %0.6f, %0.6f\n", m30_, m31_, m32_, m33_ );
}
