#pragma once

#define ALIGNMENT 16
#define ALIGN __declspec( align( ALIGNMENT ) )

#include "targetver.h"

#define NOMINMAX
#define _CRT_SECURE_NO_WARNINGS

#include <assert.h>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <float.h>
#include <vector>
#include <set>
#include <random>
#include <functional>

// visual leak detector 2.5
//#include <vld.h>

// opencv
#include <opencv2/opencv.hpp>

// omp
#include <omp.h>

// embree
#include <embree2/rtcore.h>
#include <embree2/rtcore_ray.h>

#define REAL_MAX static_cast<float>( FLT_MAX )
#define REAL_MIN static_cast<float>( -FLT_MAX )
#define EPSILON static_cast<float>( 1e-5 )

#define DEG2RAD( x ) ( ( x ) * static_cast<float>( M_PI / 180 ) )
#define RAD2DEG( x ) ( ( x ) * static_cast<float>( 180 / M_PI ) )
#define SQR( x ) ( ( x ) * ( x ) )

#ifndef MIN
#define MIN( a, b ) ( ( a < b )? ( a ) : ( b ) )
#endif

#ifndef MAX
#define MAX( a, b ) ( ( a > b )? ( a ) : ( b ) )
#endif

typedef cv::Vec3f pixel_t;

#include "utils.h"

#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "matrix4x4.h"
#include "quaternion.h"
#include "color4.h"

#include "omnilight.h"
#include "texture.h"
#include "material.h"

#include "vertex.h"
#include "triangle.h"
#include "surface.h"
#include "ray.h"
#include "scene.h"

#include "objloader.h"

#include "camera.h"
#include "shader.h"
#include "lambert_shader.h"
#include "renderer.h"


