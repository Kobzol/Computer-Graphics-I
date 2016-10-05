#ifndef QUATERNION_H_
#define QUATERNION_H_

/*! \struct Quaternion
\brief Trojrozm�rn� (3D) vektor.

Implementace kvaternionu jako�to nekomutativn� �ty�slo�kov�ho zobecn�n� komplexn�ch ��sel.
Jedn� se o line�rn� kombinaci prvk� 1, \f$\mathbf{i}\f$, \f$\mathbf{j}\f$ a \f$\mathbf{k}\f$ 
Kvaternion \f$\mathbf{q}\f$ zapisujeme \f$w + x\mathbf{i} + y\mathbf{j} + z\mathbf{k}\f$
nebo ve zkr�cen�m tvaru \f$\mathbf{q}= (w, \mathbf{v}\f$), kde \f$w \in \mathbb{R}\f$ a \f$\mathbf{v}=(x, y, z)\f$.
Vektor \f$\mathbf{v}\f$ mu�eme tedy ch�pat jako vektor v \f$\mathbb{R}^3\f$.

P��klad rotace vektoru kolem libovoln� osy
\code{.cpp}
Vector3 axis = Vector3( 2, 0, 2 );		
Vector3 u = Vector3( 1, 0, 0 );
Quaternion q = Quaternion( u );
Quaternion r = Quaternion( axis, DEG2RAD( 90 ) );		
Quaternion w = r * q;
r.Conjugation();
w *= r;
Vector3 v = w.ToVector3(); // vektor u oto�en� kole osy axis o angle stupn�� (cw)
\endcode

\author Tom� Fabi�n
\version 0.9
\date 2007-2013
*/
struct /*ALIGN*/ Quaternion
{
public:
	union	// anonymn� unie
	{
		struct
		{
			float x; /*!< Prvn� slo�ka kvaternionu. */
			float y; /*!< Druh� slo�ka kvaternionu. */
			float z; /*!< T�et� slo�ka kvaternionu. */
			float w; /*!< Skal�r. */
		};

		float data[4]; /*!< Pole slo�ek kvaternionu. */
	};

	//! V�choz� konstruktor.
	/*!
	Inicializuje v�echny slo�ky kvaternionu na hodnotu nula.
	*/
	Quaternion() : x( 0 ), y( 0 ), z( 0 ), w( 0 ) { }	

	//! Obecn� konstruktor.
	/*!
	Inicializuje slo�ky kvaternionu podle zadan�ch hodnot parametr�.

	\param x prvn� slo�ka kvaternionu.
	\param y druh� slo�ka kvaternionu.
	\param z t�et� slo�ka kvaternionu.
	\param w skal�rn� slo�ka kvaternionu.
	*/
	Quaternion( const float x, const float y, const float z, const float w ) : x( x ), y( y ), z( z ), w( w ) { }

	//! Konstruktor.
	/*!
	Inicializuje slo�ky kvaternionu podle zadan�ho vektoru.

	\param v vektor.
	*/
	Quaternion( const Vector3 & v ) : x( v.x ), y( v.y ), z( v.z ), w( 0 ) { }

	//! Konstruktor rota�n�ho kvaternionu.
	/*!
	Inicializuje slo�ky kvaternionu podle zadan� osy rotace a �hlu.

	\param axis vektor reprezentuj�c� osu rotace.
	\param angle �hel rotace.
	*/
	Quaternion( Vector3 axis, const float angle );

	//! Konstruktor kvaternionu z Eulerov�ch �hl�.
	/*!
	Inicializuje slo�ky kvaternionu podle zadan�ch Eulerov�ch �hl�.

	\param pitch �hel stop�n�.
	\param yaw �hel vybo�en�.
	\param roll �hel nato�en�.
	*/
	Quaternion( const float pitch, const float yaw, const float roll );

	//! Norma kvaternionu.
	/*!
	\return x Hodnotu \f$\mathbf{|q|}=\sqrt{\mathbf{q} \={\mathbf{q}}\f$.
	*/
	float Norm();

	//! Druh� mocnina normy kvaternionu.
	/*!
	\return x Hodnotu \f$\mathbf{|q|}^2=\mathbf{q} \={\mathbf{q}\f$.
	*/
	float SqrNorm();

	//! Konjugace.
	/*!
	\return Hodnotu \f$\={\mathbf{q}\f$.
	*/
	void Conjugation();

	//! Normalizace kvaternionu.
	/*!
	Po proveden� operace bude m�t vektor jednotkovou d�lku.
	*/
	void Normalize();

	//! P�evede kvaternion na 3D vektor.
	/*!
	\return Vektor\f$\mathbf{v}=(x,y,z)\f$.
	*/
	Vector3 ToVector3();

	//! P�evede kvaternion na matici.
	/*!
	\return Vektor\f$(x,y,z)\f$.
	*/
	Matrix4x4 ToMatrix4x4();

	// --- oper�tory ------
	
	friend Quaternion operator*( const Quaternion & u, const Quaternion & v );

	friend void operator*=( Quaternion & u, const Quaternion v );	
};

#endif
