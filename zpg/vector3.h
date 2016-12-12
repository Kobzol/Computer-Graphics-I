#ifndef VECTOR3_H_
#define VECTOR3_H_

/*! \struct Vector3
\brief Trojrozm�rn� (3D) vektor.

Implementace t��slo�kov�ho re�ln�ho vektoru podporuj�c� z�kladn�
matematick� operace.

\note
Vektor se pova�uje za sloupcov�, p�esto�e je v koment���ch pro jednoduchost
uv�d�n jako ��dkov�.

\code{.cpp}
Vector3 v = Vector3( 2.0f, 4.5f, 7.8f );
v.Normalize();
\endcode

\author Tom� Fabi�n
\version 0.95
\date 2007-2015
*/
struct /*ALIGN*/ Vector3
{
public:
	union	// anonymn� unie
	{
		struct
		{
			float x; /*!< Prvn� slo�ka vektoru. */
			float y; /*!< Druh� slo�ka vektoru. */
			float z; /*!< T�et� slo�ka vektoru. */
		};

		float data[3]; /*!< Pole slo�ek vektoru. */
	};

	//! V�choz� konstruktor.
	/*!
	Inicializuje v�echny slo�ky vektoru na hodnotu nula,
	\f$\mathbf{v}=\mathbf{0}\f$.
	*/
	Vector3() : x( 0 ), y( 0 ), z( 0 ) { }	

	//! Obecn� konstruktor.
	/*!
	Inicializuje slo�ky vektoru podle zadan�ch hodnot parametr�,
	\f$\mathbf{v}=(x,y,z)\f$.

	\param x prvn� slo�ka vektoru.
	\param y druh� slo�ka vektoru.
	\param z t�et� slo�ka vektoru.
	*/
	Vector3( const float x, const float y, const float z ) : x( x ), y( y ), z( z ) { }

	//! Konstruktor z pole.
	/*!
	Inicializuje slo�ky vektoru podle zadan�ch hodnot pole,

	\param v ukazatel na prvn� slo�ka vektoru.	
	*/
	Vector3( const float * v );

	//! L2-norma vektoru.
	/*!
	\return x Hodnotu \f$\mathbf{||v||}=\sqrt{x^2+y^2+z^2}\f$.
	*/
	float L2Norm() const;

	//! Druh� mocnina L2-normy vektoru.
	/*!
	\return Hodnotu \f$\mathbf{||v||^2}=x^2+y^2+z^2\f$.
	*/
	float SqrL2Norm() const;

	//! Normalizace vektoru.
	/*!
	Po proveden� operace bude m�t vektor jednotkovou d�lku.
	*/
	void Normalize();

	//! Vektorov� sou�in.
	/*!
	\param v vektor \f$\mathbf{v}\f$.

	\return Vektor \f$(\mathbf{u}_x \mathbf{v}_z - \mathbf{u}_z \mathbf{v}_y,
	\mathbf{u}_z \mathbf{v}_x - \mathbf{u}_x \mathbf{v}_z,
	\mathbf{u}_x \mathbf{v}_y - \mathbf{u}_y \mathbf{v}_x)\f$.
	*/
	Vector3 CrossProduct( const Vector3 & v ) const;	

	Vector3 Abs() const;

	Vector3 Max( const float a = 0 ) const;

	//! Skal�rn� sou�in.
	/*!		
	\return Hodnotu \f$\mathbf{u}_x \mathbf{v}_x + \mathbf{u}_y \mathbf{v}_y + \mathbf{u}_z \mathbf{v}_z)\f$.
	*/
	float DotProduct( const Vector3 & v ) const;

	//! Rotace.
	/*!		
	\return Vektor orotovan� kolem po��tku o \f$\phi\f$ radi�n� v kladn�m smyslu.
	*/
	//Vector2 Rotate( const float phi );

	//! Index nejv�t�� slo�ky vektoru.
	/*!
	\param absolute_value index bude ur�en podle absolutn� hodnoty slo�ky

	\return Index nejv�t�� slo�ky vektoru.
	*/
	char LargestComponent( const bool absolute_value = false );	

	void Print();

	Vector3 reflect(Vector3 normal) const
	{
		Vector3 input = *this;
		input.Normalize();
		normal.Normalize();

		input = -input;

		return 2 * (input.DotProduct(normal)) * normal - input;
	}
	Vector3 refract(Vector3 normal, float n1, float n2) const
	{
		Vector3 l = *this;
		l.Normalize();
		normal.Normalize();

		float r = n1 / n2;
		float c = (-normal).DotProduct(l);

		if (c < 0)
		{
			normal = -normal;
			c = (-normal).DotProduct(l);
		}

		return r * l + (r * c - sqrt(1 - (r * r) * (1 - (c * c)))) * normal;
	}
	Vector3 reverseRefract(Vector3 normal, float n1, float n2) const
	{
		Vector3 rf = -(*this);
		rf.Normalize();
		normal.Normalize();

		float r = n1 / n2;
		
		float cos_02 = normal.DotProduct(rf);

		if (cos_02 < 0.0f)
		{
			normal = -normal;
			cos_02 = normal.DotProduct(rf);
		}

		float phi = 1 - (r *r) * (1 - (cos_02 * cos_02));

		// tir
		if (phi < 0.0f)
		{
			return Vector3(0.0f, 0.0f, 0.0f);
		}

		float cos_01 = sqrt(phi);

		Vector3 rr = r * rf - (r * cos_02 + cos_01) * normal;
		rr.Normalize();
		Vector3 l = rr - (2 * (normal.DotProduct(rr))) * normal;
		l.Normalize();

		return -l;
	}

	bool IsZero()
	{
		return
			this->x == 0.0f &&
			this->y == 0.0f &&
			this->z == 0.0f;
	}

	inline cv::Vec3f ToOpenCV()
	{
		return cv::Vec3f(this->x, this->y, this->z);
	}

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

	// --- konverze ------

};

#endif
