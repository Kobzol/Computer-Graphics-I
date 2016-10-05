#ifndef TRIANGLE_H_
#define TRIANGLE_H_

class Surface; // dop�edn� deklarace t��dy

/*! \class Triangle
\brief Struktura popisuj�c� troj�heln�k.

\author Tom� Fabi�n
\version 1.1
\date 2013-2015
*/
class Triangle
{
public:	
	//! V�choz� konstruktor.
	/*!
	Inicializuje v�echny slo�ky troj�heln�ku na hodnotu nula.
	*/
	Triangle() { }

	//! Obecn� konstruktor.
	/*!
	Inicializuje troj�heln�k podle zadan�ch hodnot parametr�.

	\param v0 prvn� vrchol troj�heln�ka.
	\param v1 druh� vrchol troj�heln�ka.
	\param v2 t�et� vrchol troj�heln�ka.
	\param surface ukazatel na plochu, j� je troj�heln�k �lenem.
	*/
	Triangle( const Vertex & v0, const Vertex & v1, const Vertex & v2, Surface * surface = NULL );

	//void Print();

	//! I-t� vrchol troj�heln�ka.
	/*!
	\param i index vrcholu troj�heln�ka.

	\return I-t� vrchol troj�heln�ka.
	*/
	Vertex vertex( const int i );

	//! Vypo�te norm�lu troj�heln�ka v zadan�m bod�.
	/*!	
	\param p bod troj�heln�ka, pro kter� se m� vypo��tat norm�la interpolac� vrcholov�ch norm�l.
	\param texture_coord nepovinn� ukazatel na texturovac� sou�adnice v bod� \a p.

	\return Norm�la troj�heln�ka v zadan�m bod�.
	*/
	Vector3 normal( const Vector3 & p, Vector2 * texture_coord = NULL );

	//! Vypo�te norm�lu troj�heln�ka v zadan�m bod�.
	/*!
	\param u baricentrick� sou�adnice bodu, pro kter� m� b�t norm�la interpolov�na.
	\param v baricentrick� sou�adnice bodu, pro kter� m� b�t norm�la interpolov�na.

	\return Norm�la troj�heln�ka v zadan�m bod�.
	*/
	Vector3 normal( const float u, const float v );

	//! Vypo�te texturovac� sou�adnici troj�heln�ka v zadan�m bod�.
	/*!
	\param u baricentrick� sou�adnice bodu, pro kter� m� b�t norm�la interpolov�na.
	\param v baricentrick� sou�adnice bodu, pro kter� m� b�t norm�la interpolov�na.

	\return Texturovac� sou�adnice v zadan�m bod�.
	*/
	Vector2 texture_coord( const float u, const float v );

	//! Baricentr troj�heln�ka.
	/*!
	\see http://mathworld.wolfram.com/topics/TriangleCenters.html

	\return Baricentr troj�heln�ka.
	*/
	Vector3 baricenter();

	//! Ukazatel na s�, j� je troj�heln�k �lenem.
	/*!
	\return Ukazatel na s�.
	*/
	Surface * surface();

protected:

private:
	Vertex vertices_[3]; /*!< Vrcholy troj�heln�ka. Nic jin�ho tu nesm� b�t, jinak padne VBO v OpenGL! */	
};

#endif
