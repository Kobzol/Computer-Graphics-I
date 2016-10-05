#ifndef VECTOR4_H_
#define VECTOR4_H_

/*! \struct Vector4
\brief �ty�rozm�rn� (4D) vektor.

Implementace �ty�slo�kov�ho re�ln�ho vektoru podporuj�c� z�kladn�
matematick� operace.

\note
Vektor se pova�uje za sloupcov�, p�esto�e je v koment���ch pro jednoduchost
uv�d�n jako ��dkov�.

\code{.cpp}
Vector4 v = Vector3( 2.0f, 4.5f, 7.8f, 1.0f );
v.Normalize();
\endcode

\author Tom� Fabi�n
\version 0.9
\date 2007-2013
*/
struct /*ALIGN*/ Vector4
{
public:
	union	// anonymn� unie
	{
		struct
		{
			float x; /*!< Prvn� slo�ka vektoru. */
			float y; /*!< Druh� slo�ka vektoru. */
			float z; /*!< T�et� slo�ka vektoru. */
			float w; /*!< �tvrt� (homogen�) slo�ka vektoru. */
		};

		float data[4]; /*!< Pole slo�ek vektoru. */
	};

	//! V�choz� konstruktor.
	/*!
	Inicializuje v�echny slo�ky vektoru na hodnotu nula,
	\f$\mathbf{v}=\mathbf{0}\f$.
	*/
	Vector4() : x( 0 ), y( 0 ), z( 0 ), w( 0 ) { }	

	//! Obecn� konstruktor.
	/*!
	Inicializuje slo�ky vektoru podle zadan�ch hodnot parametr�,
	\f$\mathbf{v}=(x,y,z,w)\f$.

	\param x prvn� slo�ka vektoru.
	\param y druh� slo�ka vektoru.
	\param z t�et� slo�ka vektoru.
	\param w �tvrt� (homogen�) slo�ka vektoru.
	*/
	Vector4( const float x, const float y, const float z, const float w ) : x( x ), y( y ), z( z ), w( w ) { }	

	Vector4( const Vector3 & v ) : x( v.x ), y( v.y ), z( v.z ), w( 1 ) { }

	void Print();

	// --- oper�tory ------

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
