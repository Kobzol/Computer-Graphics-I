#ifndef VECTOR2_H_
#define VECTOR2_H_

/*! \struct Vector2
\brief Dvourozm�rn� (2D) vektor.

Implementace dvouslo�kov�ho re�ln�ho vektoru podporuj�c� z�kladn�
matematick� operace.

\note
Vektor se pova�uje za sloupcov�, p�esto�e je v koment���ch pro jednoduchost
uv�d�n jako ��dkov�.

\code{.cpp}
Vector2 v = Vector2( 2.0f, 4.5f );
v.Normalize();
\endcode

\author Tom� Fabi�n
\version 0.9
\date 2007-2013
*/
struct /*ALIGN*/ Vector2
{
public:
	union	// anonymn� unie
	{
		struct
		{
			float x; /*!< Prvn� slo�ka vektoru. */
			float y; /*!< Druh� slo�ka vektoru. */
		};

		float data[2]; /*!< Pole slo�ek vektoru. */
	};

	//! Implicitn� konstruktor.
	/*!
	Inicializuje v�echny slo�ky vektoru na hodnotu nula,
	\f$\mathbf{v}=\mathbf{0}\f$.
	*/
	Vector2() : x( 0 ), y( 0 ) { }	

	//! Obecn� konstruktor.
	/*!
	Inicializuje slo�ky vektoru podle zadan�ch hodnot parametr�,
	\f$\mathbf{v}=(x,y)\f$.

	\param x prvn� slo�ka vektoru.
	\param y druh� slo�ka vektoru.
	*/
	Vector2( const float x, const float y ) : x( x ), y( y ) { }

	//! L2-norma vektoru.
	/*!
	\return x Hodnotu \f$\mathbf{||v||}=\sqrt{x^2+y^2}\f$.
	*/
	float L2Norm();

	//! Druh� mocnina L2-normy vektoru.
	/*!
	\return Hodnotu \f$\mathbf{||v||^2}=x^2+y^2\f$.
	*/
	float SqrL2Norm();

	//! Normalizace vektoru.
	/*!
	Po proveden� operace bude m�t vektor jednotkovou d�lku.
	*/
	void Normalize();

	//! Vektorov� sou�in.
	/*!
	\param v vektor \f$\mathbf{v}\f$.

	\return Hodnotu \f$\mathbf{u}_x \mathbf{v}_y + \mathbf{u}_y \mathbf{v}_x\f$.
	*/
	float CrossProduct( const Vector2 & v );

	//! Vektorov� sou�in.
	/*!
	\note Plyne z v�po�tu determinantu matice \f$2\times2\f$.

	\return Hodnotu \f$(\mathbf{u}_y, -\mathbf{u}_x)\f$.
	*/
	Vector2 CrossProduct();

	//! Skal�rn� sou�in.
	/*!		
	\return Hodnotu \f$\mathbf{u}_x \mathbf{v}_x + \mathbf{u}_y \mathbf{v}_y)\f$.
	*/
	float DotProduct( const Vector2 & v );

	//! Rotace.
	/*!		
	\return Vektor orotovan� kolem po��tku o \f$\phi\f$ radi�n� v kladn�m smyslu.
	*/
	Vector2 Rotate( const float phi );

	//! Index nejv�t�� slo�ky vektoru.
	/*!
	\param absolute_value index bude ur�en podle absolutn� hodnoty slo�ky

	\return Index nejv�t�� slo�ky vektoru.
	*/
	char LargestComponent( const bool absolute_value = false );	

	//void Print();

	// --- oper�tory ------

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
�e�en� chyby C2719, vznik� nap�. p�i vkl�d�n� Vector2, 3 a 4 do std::vector
std::vector<AlignWrapper<Vector3>> vec;

http://stackoverflow.com/questions/9409591/self-contained-stl-compatible-implementation-of-stdvector/9414618#9414618
*/
template<typename T> struct AlignmentWrapper : public T
{
    AlignmentWrapper() { }
    AlignmentWrapper( const T & rhs ) : T( rhs ) { }
};

#endif
