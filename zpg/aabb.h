#ifndef AABB_H_
#define AABB_H_

/*! \class AABB
\brief Obalov� struktura.

Obalov� struktura v podob� osov� zarovnan�ho kv�dru (axis aligned bounding
box), st�ny kv�dru jsou kolm� na sou�adn� osy. Rozsah obalov� struktury je
ur�en dvojic� vektor� p�edstavuj�c� doln� a horn� mez.

\author Tom� Fabi�n
\version 1.0
\date 2011-2012
*/
class AABB
{
public:
	//! Implicitn� konstruktor.
	/*!
	Inicializuje meze struktury na
	hodnoty \f$(+\infty,+\infty,+\infty)\f$ a� \f$(-\infty,-\infty,-\infty)\f$,
	tj. struktura nic neobsahuje.
	*/
	AABB();

	//! Specializovan� konstruktor.
	/*!
	Inicializuje meze struktury na hodnoty \a p0 a� \a p1.

	\param p0 spodn� mez.
	\param p1 horn� mez.
	*/
	AABB( const Vector3 & p0, const Vector3 & p1 );

	//! Slou�en� dvou obalov�ch struktur.
	/*!
	Nov� meze struktury budou obsahovat zadanou \a aabb strukturu.

	\param aabb druh� obalov� struktura.
	*/
	void Merge( const AABB & aabb );

	//! Zahrnut� bodu do obalov� struktury.
	/*!
	Obalov� struktura bude roz���ena tak, aby obsahovala zadan� bod \a p.

	\param p bod.
	*/
	void Merge( const Vector3 & p );

	//! Index dominantn� osy.
	/*!	
	\return Index dominantn� osy.
	*/
	char dominant_axis() const;

	//! Vypo�te geometrick� st�ed obalov� struktury.
	/*!	
	\return St�ed obalov� struktury.
	*/
	Vector3 center() const;

	//! Vypo�te plochu st�n obalov� struktury.
	/*!	
	\return Plocha st�n obalov� struktury.
	*/
	TYPE_REAL surface_area() const;

	//! Doln� mez obalov� struktury.
	/*!	
	\return Doln� mez obalov� struktury.
	*/
	Vector3 lower_bound() const;

	//! Horn� mez obalov� struktury.
	/*!	
	\return Horn� mez obalov� struktury.
	*/
	Vector3 upper_bound() const;	

	Vector3 & operator[]( int i )
	{
		return bounds_[i];
	}

	void print() const
	{
		printf( "_box %0.6f,%0.6f,%0.6f %0.6f,%0.6f,%0.6f %0.6f\n",
			bounds_[0].x, bounds_[0].y, bounds_[0].z,
			bounds_[1].x, bounds_[1].y, bounds_[0].z,
			bounds_[1].z - bounds_[0].z );
	}

private:
	Vector3 bounds_[2]; /*!< Doln� [0] a horn� [1] mez obalov� struktury. */
};

#endif
