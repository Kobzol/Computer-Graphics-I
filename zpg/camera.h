#ifndef CAMERA_H_
#define CAMERA_H_

/*! \class Camera
\brief Základní dírková kamera.

\author Tomáš Fabián
\version 1.0
\date 2014
*/
class Camera
{
public:
	Camera( const int width, const int height, const Vector3 & view_from,
		const Vector3 & viet_at, const float fov_y );

	float aspect_ratio() const;	

	float fov_y() const;

	int width() const;

	void set_width(const int width);

	int height() const;

	void set_height(const int width);

	Vector3 view_from() const;

	Vector3 view_at() const;

	void BuildViewMatrix();

	void Update();

	Ray GenerateRay( const float sx, const float sy ) const;	

	float orthogonal_depth( const Vector3 & p ) const;
	Vector3 ws2es( const Vector3 & p ) const;
	Vector3 normal_ws2es( const Vector3 & p ) const;

	void Save( const char * file_name );
	void Save(cv::FileStorage & fs);

	void Print();

	static const char scanline_hilbert_4x4[];

protected:

private:
	int width_; /*!< Šíøka obrazu [px]. */
	int height_; /*!< Výška obrazu [px]. */

	Vector3 view_from_; /*!< Oko [m]. */
	Vector3 view_at_; /*!< Bod, na který se díváme [m]. */

	float fov_y_; /*!< Zorný úhel ve vertikální rovinì [rad]. */
	float d_; /*!< Vzdálenost promítací roviny od støedu promítání (oka) [m]. */
	float pixel_size_; /*!< Velikost 1 pixelu snímaèe [m/px]. */	

	Matrix4x4 view_t_; /*!< Matice pøechodu eye-space do world-space. */
	Matrix4x4 view_; /*!< Matice pøechodu z world-space do eye-space. */
	Matrix4x4 view_normal_; /*!< Matice pøechodu z world-space do eye-space pro transformaci normál. */

};

#endif
