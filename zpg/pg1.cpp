#include "stdafx.h"
#include "scene.h"
#include "lambert_shader.h"

void rtc_error_function( const RTCError code, const char * str )
{
	printf( "ERROR in Embree: %s\n", str );
	exit( 1 );
}

RTCError check_rtc_or_die( RTCDevice & device )
{
	const RTCError error = rtcDeviceGetError( device );

	if ( error != RTC_NO_ERROR )
	{
		printf( "ERROR in Embree: " );

		switch ( error )
		{
		case RTC_UNKNOWN_ERROR:
			printf( "An unknown error has occurred." );
			break;

		case RTC_INVALID_ARGUMENT:
			printf( "An invalid argument was specified." );
			break;

		case RTC_INVALID_OPERATION:
			printf( "The operation is not allowed for the specified object." );
			break;

		case RTC_OUT_OF_MEMORY:
			printf( "There is not enough memory left to complete the operation." );
			break;

		case RTC_UNSUPPORTED_CPU:
			printf( "The CPU is not supported as it does not support SSE2." );
			break;

		case RTC_CANCELLED:
			printf( "The operation got cancelled by an Memory Monitor Callback or Progress Monitor Callback function." );
			break;
		}

		fflush( stdout );
		exit( 1 );
	}

	return error;
}

void filter_intersection( void * user_ptr, Ray & ray )
{
	/*  All hit information inside the ray is valid.
		The filter function can reject a hit by setting the geomID member of the ray to
	    RTC_INVALID_GEOMETRY_ID, otherwise the hit is accepted.The filter function is not
		allowed to modify the ray input data (org, dir, tnear, tfar), but can modify
		the hit data of the ray( u, v, Ng, geomID, primID ). */

	Surface * surface = reinterpret_cast<Surface *>( user_ptr );	
	printf( "intersection of: %s, ", surface->get_name().c_str() );
	const Vector3 p = ray.eval( ray.tfar );
	printf( "at: %0.3f (%0.3f, %0.3f, %0.3f)\n", ray.tfar, p.x, p.y, p.z );
	
	ray.geomID = RTC_INVALID_GEOMETRY_ID; // reject hit
}

void filter_occlusion( void * user_ptr, Ray & ray )
{
	/*  All hit information inside the ray is valid.
	The filter function can reject a hit by setting the geomID member of the ray to
	RTC_INVALID_GEOMETRY_ID, otherwise the hit is accepted.The filter function is not
	allowed to modify the ray input data (org, dir, tnear, tfar), but can modify
	the hit data of the ray( u, v, Ng, geomID, primID ). */

	Surface * surface = reinterpret_cast<Surface *>( user_ptr );	
	printf( "occlusion of: %s, ", surface->get_name().c_str() );
	const Vector3 p = ray.eval( ray.tfar );
	printf( "at: %0.3f (%0.3f, %0.3f, %0.3f)\n", ray.tfar, p.x, p.y, p.z );

	ray.geomID = RTC_INVALID_GEOMETRY_ID; // reject hit
}

int test(Scene& scene, std::vector<Surface*>& surfaces)
{
	// --- test rtcIntersect -----
	Ray rtc_ray = Ray(Vector3(-1.0f, 0.1f, 0.2f), Vector3(2.0f, 0.0f, 0.0f), 0.0f);	// pocatek, smer, near
	
	rtcIntersect(scene.handle(), rtc_ray ); // find the closest hit of a ray segment with the scene
	// pri filtrovani funkce rtcIntersect jsou pruseciky prochazeny od nejblizsiho k nejvzdalenejsimu
	// u funkce rtcOccluded jsou nalezene pruseciky vraceny v libovolnem poradi

	if ( rtc_ray.geomID != RTC_INVALID_GEOMETRY_ID )
	{
		Surface * surface = surfaces[rtc_ray.geomID];	// objekt
		Triangle & triangle = surface->get_triangle( rtc_ray.primID );	// trojúhelník

		// získání souřadnic průsečíku, normál, texturovacích souřadnic atd.
		Vector3 p = rtc_ray.eval( rtc_ray.tfar );	// souřadnice průsečíku, ve světovém souřadném systému, tfar = vzdálenost od originu (t)
		
		Vector3 geometry_normal = -Vector3( rtc_ray.Ng ); // Ng je nenormalizovaná normála zasaženého trojúhelníka vypočtená nesouhlasně s pravidlem pravé ruky o závitu
		geometry_normal.Normalize(); // normála zasaženého trojúhelníka vypočtená souhlasně s pravidlem pravé ruky o závitu
		
		const Vector3 normal = triangle.normal( rtc_ray.u, rtc_ray.v );
		const Vector2 texture_coord = triangle.texture_coord( rtc_ray.u, rtc_ray.v );

		p.Print();
	}

	// --- test rtcOccluded -----
	rtc_ray = Ray( Vector3( -1.0f, 0.1f, 0.2f ), Vector3( 1.0f, 0.0f, 0.0f ) );
	rtcOccluded( scene.handle(), rtc_ray ); // determining if any hit between a ray segment and the scene exists
	// po volání rtcOccluded je nastavena pouze hodnota geomID, ostatni jsou nezměněny
	if ( rtc_ray.geomID == 0 )
	{
		// neco jsme nekde na zadaném intervalu (tnear, tfar) trefili, ale nevime co ani kde
	}

	return 0;
}

/*
Osy - Z nahoru, Y dopředu, X doprava
*/

void render(Scene& scene, const Vector3& position, const Vector3& target, Cubemap& cubemap, int wait = 0)
{
	Camera camera = Camera(
		640,
		480,
		position,
		target,
		DEG2RAD(42.185f)
	);

	Renderer renderer;
	cv::Mat img(camera.height(), camera.width(), CV_32FC3);
	LambertShader shader;
	PathTracer tracer(12, shader, 200);
	//WhittedTracer tracer(10, shader);

	renderer.render(scene, camera, tracer, cubemap, img);

	cv::imshow("Render", img);

	cv::Mat saved;
	img.convertTo(saved, CV_8UC3, 255.0f);
	cv::imwrite("render.jpg", saved);
	
	cv::waitKey(wait);
}

int main( int argc, char * argv[] )
{
	printf( "PG1, (c)2011-2016 Tomas Fabian\n\n" );

	_MM_SET_FLUSH_ZERO_MODE( _MM_FLUSH_ZERO_ON ); // Flush to Zero, Denormals are Zero mode of the MXCSR
	_MM_SET_DENORMALS_ZERO_MODE( _MM_DENORMALS_ZERO_ON );
	RTCDevice device = rtcNewDevice( NULL ); // musíme vytvořit alespoň jedno Embree zařízení		
	check_rtc_or_die( device ); // ověření úspěšného vytvoření Embree zařízení
	rtcDeviceSetErrorFunction( device, rtc_error_function ); // registrace call-back funkce pro zachytávání chyb v Embree

	std::vector<Surface*> surfaces;
	std::vector<Material*> materials;

	// načtení geometrie
	if (LoadOBJ("../../data/6887/6887_allied_avenger.obj", Vector3( 0.5f, 0.5f, 0.5f ), surfaces, materials) < 0)
	{
		return -1;
	}

	// vytvoření scény v rámci Embree
	Scene scene(device);
	// RTC_INTERSECT1 = enables the rtcIntersect and rtcOccluded functions

	// nakopírování všech modelů do bufferů Embree
	for (Surface* surface : surfaces)
	{
		scene.addSurface(surface);
	}
	for (Material* material : materials)
	{
		scene.addMaterial(material);
	}

	scene.commit();

	Cubemap map("../../data/tex/tex", ".jpg");

	Vector3 position(-140.0f, -175.0f, 110.0f);
	Vector3 target(0.0f, 0.0f, 35.0f);
	render(scene, position, target, map, 0);

	/*while (true)
	{
		char input;
		std::cin >> input;

		if (input == 'y') target.x--;
		else if (input == 'x') target.x++;
		else if (input == 'c') target.y--;
		else if (input == 'v') target.y++;
		else if (input == 'b') target.z--;
		else if (input == 'n') target.z++;

		render(scene, target, position, map, 1);
	}*/

	// TODO *** ray tracing ****
	//test(scene, surfaces);

	SafeDeleteVectorItems<Material*>(materials);
	SafeDeleteVectorItems<Surface*>(surfaces);

	rtcDeleteDevice(device); // Embree zařízení musíme také uvolnit před ukončením aplikace

	return 0;
}
