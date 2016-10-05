#ifndef TRIANGLE_H_
#define TRIANGLE_H_

class Surface; // dopøedné deklarace tøídy

/*! \class Triangle
\brief Struktura popisující trojúhelník.

\author Tomáš Fabián
\version 1.1
\date 2013-2015
*/
class Triangle
{
public:	
	//! Vıchozí konstruktor.
	/*!
	Inicializuje všechny sloky trojúhelníku na hodnotu nula.
	*/
	Triangle() { }

	//! Obecnı konstruktor.
	/*!
	Inicializuje trojúhelník podle zadanıch hodnot parametrù.

	\param v0 první vrchol trojúhelníka.
	\param v1 druhı vrchol trojúhelníka.
	\param v2 tøetí vrchol trojúhelníka.
	\param surface ukazatel na plochu, jí je trojúhelník èlenem.
	*/
	Triangle( const Vertex & v0, const Vertex & v1, const Vertex & v2, Surface * surface = NULL );

	//void Print();

	//! I-tı vrchol trojúhelníka.
	/*!
	\param i index vrcholu trojúhelníka.

	\return I-tı vrchol trojúhelníka.
	*/
	Vertex vertex( const int i );

	//! Vypoète normálu trojúhelníka v zadaném bodì.
	/*!	
	\param p bod trojúhelníka, pro kterı se má vypoèítat normála interpolací vrcholovıch normál.
	\param texture_coord nepovinnı ukazatel na texturovací souøadnice v bodì \a p.

	\return Normála trojúhelníka v zadaném bodì.
	*/
	Vector3 normal( const Vector3 & p, Vector2 * texture_coord = NULL );

	//! Vypoète normálu trojúhelníka v zadaném bodì.
	/*!
	\param u baricentrická souøadnice bodu, pro kterı má bıt normála interpolována.
	\param v baricentrická souøadnice bodu, pro kterı má bıt normála interpolována.

	\return Normála trojúhelníka v zadaném bodì.
	*/
	Vector3 normal( const float u, const float v );

	//! Vypoète texturovací souøadnici trojúhelníka v zadaném bodì.
	/*!
	\param u baricentrická souøadnice bodu, pro kterı má bıt normála interpolována.
	\param v baricentrická souøadnice bodu, pro kterı má bıt normála interpolována.

	\return Texturovací souøadnice v zadaném bodì.
	*/
	Vector2 texture_coord( const float u, const float v );

	//! Baricentr trojúhelníka.
	/*!
	\see http://mathworld.wolfram.com/topics/TriangleCenters.html

	\return Baricentr trojúhelníka.
	*/
	Vector3 baricenter();

	//! Ukazatel na sí, jí je trojúhelník èlenem.
	/*!
	\return Ukazatel na sí.
	*/
	Surface * surface();

protected:

private:
	Vertex vertices_[3]; /*!< Vrcholy trojúhelníka. Nic jiného tu nesmí bıt, jinak padne VBO v OpenGL! */	
};

#endif
