#ifndef VECTOR2_H_
#define VECTOR2_H_

/*! \struct Vector2
\brief Dvourozmìrný (2D) vektor.

Implementace dvousložkového reálného vektoru podporující základní
matematické operace.

\note
Vektor se považuje za sloupcový, pøestože je v komentáøích pro jednoduchost
uvádìn jako øádkový.

\code{.cpp}
Vector2 v = Vector2( 2.0f, 4.5f );
v.Normalize();
\endcode

\author Tomáš Fabián
\version 0.9
\date 2007-2013
*/
struct /*ALIGN*/ Vector2
{
public:
	union	// anonymní unie
	{
		struct
		{
			float x; /*!< První složka vektoru. */
			float y; /*!< Druhá složka vektoru. */
		};

		float data[2]; /*!< Pole složek vektoru. */
	};

	//! Implicitní konstruktor.
	/*!
	Inicializuje všechny složky vektoru na hodnotu nula,
	\f$\mathbf{v}=\mathbf{0}\f$.
	*/
	Vector2() : x( 0 ), y( 0 ) { }	

	//! Obecný konstruktor.
	/*!
	Inicializuje složky vektoru podle zadaných hodnot parametrù,
	\f$\mathbf{v}=(x,y)\f$.

	\param x první složka vektoru.
	\param y druhá složka vektoru.
	*/
	Vector2( const float x, const float y ) : x( x ), y( y ) { }

	//! L2-norma vektoru.
	/*!
	\return x Hodnotu \f$\mathbf{||v||}=\sqrt{x^2+y^2}\f$.
	*/
	float L2Norm();

	//! Druhá mocnina L2-normy vektoru.
	/*!
	\return Hodnotu \f$\mathbf{||v||^2}=x^2+y^2\f$.
	*/
	float SqrL2Norm();

	//! Normalizace vektoru.
	/*!
	Po provedení operace bude mít vektor jednotkovou délku.
	*/
	void Normalize();

	//! Vektorový souèin.
	/*!
	\param v vektor \f$\mathbf{v}\f$.

	\return Hodnotu \f$\mathbf{u}_x \mathbf{v}_y + \mathbf{u}_y \mathbf{v}_x\f$.
	*/
	float CrossProduct( const Vector2 & v );

	//! Vektorový souèin.
	/*!
	\note Plyne z výpoètu determinantu matice \f$2\times2\f$.

	\return Hodnotu \f$(\mathbf{u}_y, -\mathbf{u}_x)\f$.
	*/
	Vector2 CrossProduct();

	//! Skalární souèin.
	/*!		
	\return Hodnotu \f$\mathbf{u}_x \mathbf{v}_x + \mathbf{u}_y \mathbf{v}_y)\f$.
	*/
	float DotProduct( const Vector2 & v );

	//! Rotace.
	/*!		
	\return Vektor orotovaný kolem poèátku o \f$\phi\f$ radiánù v kladném smyslu.
	*/
	Vector2 Rotate( const float phi );

	//! Index nejvìtší složky vektoru.
	/*!
	\param absolute_value index bude urèen podle absolutní hodnoty složky

	\return Index nejvìtší složky vektoru.
	*/
	char LargestComponent( const bool absolute_value = false );	

	//void Print();

	// --- operátory ------

	friend Vector2 operator-( const Vector2 & v );

	friend Vector2 operator+( const Vector2 & u, const Vector2 & v );
	friend Vector2 operator-( const Vector2 & u, const Vector2 & v );

	friend Vector2 operator*( const Vector2 & v, const float a );	
	friend Vector2 operator*( const float a, const Vector2 & v );
	friend Vector2 operator*( const Vector2 & u, const Vector2 & v );

	friend Vector2 operator/( const Vector2 & v, const float a );

	friend void operator+=( Vector2 & u, const Vector2 & v );
	friend void operator-=( Vector2 & u, const Vector2 & v );
	friend void operator*=( Vector2 & v, const float a );
	friend void operator/=( Vector2 & v, const float a );	
};

/*
Øešení chyby C2719, vzniká napø. pøi vkládání Vector2, 3 a 4 do std::vector
std::vector<AlignWrapper<Vector3>> vec;

http://stackoverflow.com/questions/9409591/self-contained-stl-compatible-implementation-of-stdvector/9414618#9414618
*/
template<typename T> struct AlignmentWrapper : public T
{
    AlignmentWrapper() { }
    AlignmentWrapper( const T & rhs ) : T( rhs ) { }
};

#endif
