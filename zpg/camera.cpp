#include "stdafx.h"

const char Camera::scanline_hilbert_4x4[] = { 0,0, 1,0, 1,1, 0,1, 0,2, 0,3, 1,3, 1,2, 2,2, 2,3, 3,3, 3,2, 3,1, 2,1, 2,0, 3,0 };

Camera::Camera( const int width, const int height, const Vector3 & view_from,
			   const Vector3 & viet_at, const float fov_y )
{
	width_ = width;
	height_ = height;				

	view_from_ = view_from;
	view_at_ = viet_at;

	fov_y_ = fov_y;
	d_ = 1.0f;

	Update();
}

float Camera::aspect_ratio() const
{
	return width_ / static_cast<float>( height_ );
}

float Camera::fov_y() const
{
	return fov_y_;
}

int Camera::width() const
{
	return width_;
}

void Camera::set_width(const int width)
{
	width_ = width;
	Update();
}

int Camera::height() const
{
	return height_;
}

void Camera::set_height(const int height)
{
	height_ = height;
	Update();
}

Vector3 Camera::view_from() const
{
	return view_from_;		
}	

Vector3 Camera::view_at() const
{
	return view_at_;		
}

float Camera::orthogonal_depth( const Vector3 & p ) const
{
	Vector3 axis_z_ = view_from_ - view_at_;
	axis_z_.Normalize();

	return ( p - view_from_ ).DotProduct( -axis_z_ );
}

Vector3 Camera::ws2es( const Vector3 & p ) const
{
	return view_ * Vector4(p); // pøechod bodu ze svìtového souøadného systému (world space) do kamerového (eye space)
}

Vector3 Camera::normal_ws2es( const Vector3 & p ) const
{
	Vector3 normal_es = view_normal_ * Vector4( p );
	normal_es.Normalize();

	return normal_es; // pøechod normály ze svìtového souøadného systému (world space) do kamerového (eye space)
}

void Camera::BuildViewMatrix()
{		
	Vector3 axis_z_ = view_from_ - view_at_;
	axis_z_.Normalize();
	const Vector3 up = Vector3( 0, 0, 1 );
	Vector3 axis_x_ = up.CrossProduct( axis_z_ );
	axis_x_.Normalize(); // renormalizace je tady nutná
	Vector3 axis_y_ = axis_z_.CrossProduct( axis_x_ );
	axis_y_.Normalize(); // renormalizace je tady nutná

	view_.set( 0, 0, axis_x_.x );
	view_.set( 0, 1, axis_y_.x );
	view_.set( 0, 2, axis_z_.x );
	view_.set( 0, 3, view_from_.x );

	view_.set( 1, 0, axis_x_.y );
	view_.set( 1, 1, axis_y_.y );
	view_.set( 1, 2, axis_z_.y );
	view_.set( 1, 3, view_from_.y );

	view_.set( 2, 0, axis_x_.z );
	view_.set( 2, 1, axis_y_.z );
	view_.set( 2, 2, axis_z_.z );
	view_.set( 2, 3, view_from_.z );

	view_t_ = view_;	

	view_normal_ = view_;	
	view_normal_.Transpose();

	view_.EuclideanInverse();	
}

void Camera::Update()
{
	BuildViewMatrix();

	pixel_size_ = 2 * d_ * tanf( fov_y_ * 0.5f ) / height_;
}

Ray Camera::GenerateRay( const float sx, const float sy ) const
{	
	const float x = pixel_size_ * ( sx - 0.5f * ( width_ - 1 ) );
	const float y = pixel_size_ * ( -sy + 0.5f * ( height_ - 1 ) );

	Vector3 direction = Vector3( x, y, -d_ ); // smìr nového paprsku v kamerovém prostoru
	direction.Normalize();

	direction = view_t_ * direction; // pøechod do svìtového souøadného systému
	direction.Normalize();

	return Ray( view_from_, direction, 0 );
}

void Camera::Save( const char * file_name )
{
	FILE * file = fopen( file_name, "a" );

	if ( file != NULL )
	{
		fprintf( file, "--- Camera parameters ---\n" );
		fprintf( file, "width=%d px, height=%d px, aspect=%0.3f, fov_y=%0.3f deg\n",
			width(), height(), aspect_ratio(), fov_y() );
		fprintf( file, "from=(%0.6f, %0.6f, %0.6f), at=(%0.6f, %0.6f, %0.6f)\n",
			view_from_.x, view_from_.y, view_from_.z, view_at_.x, view_at_.y, view_at_.z );

		fclose( file );
		file = NULL;
	}
}

void Camera::Save(cv::FileStorage & fs)
{
	fs << "camera_width" << width_;
	fs << "camera_height" << height_;
	fs << "fov_y" << fov_y_;
	fs << "pixel_size" << pixel_size_;
	fs << "view_from" << (cv::Mat_<float>(3, 1) << view_from_.x, view_from_.y, view_from_.z);
	fs << "view_at" << (cv::Mat_<float>(3, 1) << view_at_.x, view_at_.y, view_at_.z);
	fs << "view_matrix" << (cv::Mat_<float>(4, 4) <<
		view_.data()[0], view_.data()[1], view_.data()[2], view_.data()[3],
		view_.data()[4], view_.data()[5], view_.data()[6], view_.data()[7],
		view_.data()[8], view_.data()[9], view_.data()[10], view_.data()[11],
		view_.data()[12], view_.data()[13], view_.data()[14], view_.data()[15]);

	fs << "view_t_matrix" << (cv::Mat_<float>(4, 4) <<
		view_t_.data()[0], view_t_.data()[1], view_t_.data()[2], view_t_.data()[3],
		view_t_.data()[4], view_t_.data()[5], view_t_.data()[6], view_t_.data()[7],
		view_t_.data()[8], view_t_.data()[9], view_t_.data()[10], view_t_.data()[11],
		view_t_.data()[12], view_t_.data()[13], view_t_.data()[14], view_t_.data()[15]);
}

void Camera::Print()
{
	printf( "width=%d px, height=%d px, fov_y=%0.3f deg, pixel_size=%0.6f m/px\n", width_, height_, RAD2DEG( fov_y_ ), pixel_size_ );

	printf( "view_from = " );
	view_from_.Print();

	printf( "view_at = " );
	view_at_.Print();	

	printf( "view_matrix = (world-space -> eye-space)\n" );
	view_.Print();

	printf( "view_t_matrix = (eye-space -> world-space)\n" );
	view_t_.Print();
}
