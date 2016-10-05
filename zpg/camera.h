#ifndef CAMERA_H_
#define CAMERA_H_

/*! \class Camera
\brief Z�kladn� d�rkov� kamera.

\author Tom� Fabi�n
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
	int width_; /*!< ���ka obrazu [px]. */
	int height_; /*!< V��ka obrazu [px]. */

	Vector3 view_from_; /*!< Oko [m]. */
	Vector3 view_at_; /*!< Bod, na kter� se d�v�me [m]. */

	float fov_y_; /*!< Zorn� �hel ve vertik�ln� rovin� [rad]. */
	float d_; /*!< Vzd�lenost prom�tac� roviny od st�edu prom�t�n� (oka) [m]. */
	float pixel_size_; /*!< Velikost 1 pixelu sn�ma�e [m/px]. */	

	Matrix4x4 view_t_; /*!< Matice p�echodu eye-space do world-space. */
	Matrix4x4 view_; /*!< Matice p�echodu z world-space do eye-space. */
	Matrix4x4 view_normal_; /*!< Matice p�echodu z world-space do eye-space pro transformaci norm�l. */

};

#endif
