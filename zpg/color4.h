#ifndef COLOR4_H_
#define COLOR4_H_

/*! \struct Color4
\brief RGBA barva.

Implementace �ty�slo�kov�ho pixelu.

\code{.cpp}
Color4 pixel = Color4( 0.5f, 0.25f, 1.0f, 1.0f );
\endcode

\author Tom� Fabi�n
\version 0.9
\date 2007-2013
*/
struct /*ALIGN*/ Color4
{
public:
	union	// anonymn� unie
	{
		struct
		{
			float r; /*!< �erven� kan�l. */
			float g; /*!< Zelen� kan�l. */
			float b; /*!< Modr� kan�l. */
			float a; /*!< Alfa kan�l. */
		};

		float data[3]; /*!< Pole slo�ek vektoru. */
	};

	//! V�choz� konstruktor.
	/*!
	Inicializuje slo�ky na �ernou nepr�hlednou barvu.
	*/
	Color4() : r( 0 ), g( 0 ), b( 0 ), a( 0 ) { }

	Color4( const Vector3 color, const float alpha ) : r( color.x ), g( color.y ), b( color.z ), a( alpha ) { }

	//! Obecn� konstruktor.
	/*!
	Inicializuje barevn� slo�ky podle zadan�ch hodnot parametr�.	

	\param r �erven� kan�l.
	\param g zelen� kan�l.
	\param b modr� kan�l.
	\param a alfa kan�l.
	*/
	Color4( const float r, const float g, const float b, const float a ) : r( r ), g( g ), b( b ), a( a ) { }

	//void Print();

	// --- oper�tory ------

	//friend Color4 operator-( const Color4 & v );

	friend Color4 operator+( const Color4 & c1, const Color4 & c2 );
	//friend Color4 operator-( const Color4 & u, const Color4 & v );

	friend Color4 operator*( const Color4 & c, const float a );	
	friend Color4 operator*( const float a, const Color4 & c );
	friend Color4 operator*( const Vector3 & v, const Color4 & c );
	friend Color4 operator*( const Color4 & u, const Color4 & v );

	//friend Color4 operator/( const Color4 & v, const float a );

	friend void operator+=( Color4 & c1, const Color4 & c2 );
	//friend void operator-=( Color4 & u, const Color4 & v );
	//friend void operator*=( Color4 & v, const float a );
	friend void operator/=( Color4 & c, const float a );
};

#endif
