#ifndef VECTOR4_H_
#define VECTOR4_H_

/*! \struct Vector4
\brief Ètyørozmìrný (4D) vektor.

Implementace ètyøsložkového reálného vektoru podporující základní
matematické operace.

\note
Vektor se považuje za sloupcový, pøestože je v komentáøích pro jednoduchost
uvádìn jako øádkový.

\code{.cpp}
Vector4 v = Vector3( 2.0f, 4.5f, 7.8f, 1.0f );
v.Normalize();
\endcode

\author Tomáš Fabián
\version 0.9
\date 2007-2013
*/
struct /*ALIGN*/ Vector4
{
public:
	union	// anonymní unie
	{
		struct
		{
			float x; /*!< První složka vektoru. */
			float y; /*!< Druhá složka vektoru. */
			float z; /*!< Tøetí složka vektoru. */
			float w; /*!< Ètvrtá (homogení) složka vektoru. */
		};

		float data[4]; /*!< Pole složek vektoru. */
	};

	//! Výchozí konstruktor.
	/*!
	Inicializuje všechny složky vektoru na hodnotu nula,
	\f$\mathbf{v}=\mathbf{0}\f$.
	*/
	Vector4() : x( 0 ), y( 0 ), z( 0 ), w( 0 ) { }	

	//! Obecný konstruktor.
	/*!
	Inicializuje složky vektoru podle zadaných hodnot parametrù,
	\f$\mathbf{v}=(x,y,z,w)\f$.

	\param x první složka vektoru.
	\param y druhá složka vektoru.
	\param z tøetí složka vektoru.
	\param w ètvrtá (homogení) složka vektoru.
	*/
	Vector4( const float x, const float y, const float z, const float w ) : x( x ), y( y ), z( z ), w( w ) { }	

	Vector4( const Vector3 & v ) : x( v.x ), y( v.y ), z( v.z ), w( 1 ) { }

	void Print();

	// --- operátory ------

	friend Vector3 operator-( const Vector3 & v );

	friend Vector3 operator+( const Vector3 & u, const Vector3 & v );
	friend Vector3 operator-( const Vector3 & u, const Vector3 & v );

	friend Vector3 operator*( const Vector3 & v, const float a );	
	friend Vector3 operator*( const float a, const Vector3 & v );
	friend Vector3 operator*( const Vector3 & u, const Vector3 & v );

	friend Vector3 operator/( const Vector3 & v, const float a );

	friend void operator+=( Vector3 & u, const Vector3 & v );
	friend void operator-=( Vector3 & u, const Vector3 & v );
	friend void operator*=( Vector3 & v, const float a );
	friend void operator/=( Vector3 & v, const float a );	
};

#endif
