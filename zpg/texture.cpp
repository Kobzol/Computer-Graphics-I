#include "stdafx.h"

Texture::Texture()
{
	pixel_size_ = 0;
	row_size_ = 0;
	width_ = 0;
	height_ = 0;

	data_ = NULL;
}

Texture::Texture( cv::Mat & image )
{
	int depth = image.depth();
	pixel_size_ = image.channels() * 1;	
	width_ = image.cols;
	height_ = image.rows;
	row_size_ = width_ * pixel_size_; //image->widthStep;

	//assert( pixel_size_ == 4 );
	assert( width_ * pixel_size_ == row_size_ );

	data_ = new unsigned char[width_ * height_ * pixel_size_];
	//data_ = reinterpret_cast<unsigned char *>( image->imageData );

	for ( int y = 0; y < height_; ++y )
	{
		memcpy( data_ + ( height_ - 1 - y ) * width_ * pixel_size_,
			image.data + y * image.step, width_ * pixel_size_ );
	}	
}

Texture::~Texture()
{
	SAFE_DELETE_ARRAY( data_ );
}

int Texture::width() const
{
	return width_;
}

int Texture::height() const
{
	return height_;
}

unsigned char * Texture::get_data() const 
{
	return data_;
}

Color4 Texture::get_texel( const float u, const float v )
{
	// interpolace nejbližším sousedem
	/*const int x = MAX( 0, MIN( width_ - 1, cvRound( u * width_ ) ) );
	const int y = MAX( 0, MIN( height_ - 1, cvRound( v * height_ ) ) );
	unsigned char * pixel = &data_[x * pixel_size_ + y * row_size_];

	return Vector3( pixel[0], pixel[1], pixel[2] ) / static_cast<float>( 255 );*/

	// bilineární interpolace
	const float x = MAX( 0, MIN( width_ - 1, u * width_ ) );
	const float y = MAX( 0, MIN( height_ - 1, v * height_ ) );

	const int x0 = static_cast<int>( floor( x ) );
	const int y0 = static_cast<int>( floor( y ) );

	const int x1 = MIN( width_ - 1, x0 + 1 );
	const int y1 = MIN( height_ - 1, y0 + 1 );

	unsigned char * p1 = &data_[x0 * pixel_size_ + y0 * row_size_];
	unsigned char * p2 = &data_[x1 * pixel_size_ + y0 * row_size_];
	unsigned char * p3 = &data_[x1 * pixel_size_ + y1 * row_size_];
	unsigned char * p4 = &data_[x0 * pixel_size_ + y1 * row_size_];

	const float kx = x - x0;
	const float ky = y - y0;

	return ( Color4( p1[0], p1[1], p1[2], p1[3] ) * ( 1 - kx ) * ( 1 - ky ) +
		Color4( p2[0], p2[1], p2[2], p2[3] ) * kx * ( 1 - ky ) +				
		Color4( p3[0], p3[1], p3[2], p3[3] ) * kx * ky +
		Color4( p4[0], p4[1], p4[2], p4[3] ) * ( 1 - kx ) * ky ) *
		static_cast<float>( 1.0 / 255.0 );
}

Texture * LoadTexture( const char * file_name, const int flip, const bool single_channel )
{
	cv::Mat image_bgr = ( single_channel )?
		cv::imread( file_name, 0 ) :
		cv::imread( file_name, 1 );

	if ( image_bgr.empty() )
	{
		printf( "Texture %s not found.\n", file_name );

		return NULL;
	}

	if ( ( flip == 0 ) || ( flip == 1 ) )
	{
		cv::flip( image_bgr, image_bgr, flip ); // flip v/h
	}
	else
	{
		if ( ( flip == 2 ) || ( flip == 3 ) )
		{
			cv::transpose( image_bgr, image_bgr );
			cv::flip( image_bgr, image_bgr, flip - 2 ); // rotate +/- 90
		}
		else if ( flip == 4 )
		{
			cv::flip( image_bgr, image_bgr, 0 );
			cv::flip( image_bgr, image_bgr, 1 ); // rotate 180
		}
	}
	
	cv::Mat image_rgba = cv::Mat( image_bgr.size(), single_channel ? CV_8UC1 : CV_8UC4);

	if ( image_bgr.channels() == 4 )
	{
		// obrázek je už naèten i s alfa kanálem
		cv::cvtColor( image_bgr, image_rgba, cv::COLOR_BGRA2RGBA );
	}
	else
	{		
		if ( single_channel )
		{
			//cvCvtColor( image_bgr, image_rgba, CV_BGR2RGBA );			
			image_bgr.copyTo( image_rgba );
		}
		else
		{
		// alfa kanál chybí
			cv::cvtColor( image_bgr, image_rgba, cv::COLOR_BGR2RGBA );
		}
	}

	//cvReleaseImage( &image_bgr );
	//image_bgr = NULL;

	Texture * texture = new Texture( image_rgba );

	//cvReleaseImage( &image_rgba );
	//image_rgba = NULL;

	return texture;
}
