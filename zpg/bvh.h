#ifndef BVH_H_
#define BVH_H_

#define NO_AXIS_BINS 64

/*
Origin�ln� verze
---
Building BVH...
root bounds=(-3.150, -3.842, -0.060) x (3.118, 2.426, 1.541)
131071 nodes (512.0 KB), 65536 leafs, 174117 items, max depth 16
Done in 8.661 s.
---

std::vector a sort
---
Building BVH...
root bounds=(-3.150, -3.842, -0.060) x (3.118, 2.426, 1.541)
131071 nodes (512.0 KB), 65536 leafs, 174117 items, max depth 16
Done in 9.814 s.
---

std::vector a partial_sort
---
Building BVH...
root bounds=(-3.150, -3.842, -0.060) x (3.118, 2.426, 1.541)
131071 nodes (512.0 KB), 65536 leafs, 174117 items, max depth 16
Done in 7.296 s.
---

*/

//void Sort( Triangle ** items, const int number_of_items, const char axis );

// Readings: Real-Timie Collision Detection, p. 235 - 284
// http://ompf.org/forum/viewtopic.php?f=17&t=2226

/*! \struct Node
\brief Uzel BVH stromu.

\author Tom� Fabi�n
\version 0.9
\date 2011-2012
*/
struct Node
{
public:
	//! Kv�dr obsahuj�c� v�echny itemy v uzlu.
	AABB bounds;

	//! Uzav�en� interval index� v�ech item�, kter� pat�� do uzlu.
	int span[2];

	//! Lev� a prav� poduzel.
	Node * children[2];

	//! Osa pou�it� pro d�len� tohoto uzlu.
	char split_axis;

	Node( const int from, const int to )
	{
		span[0] = from;
		span[1] = to;

		children[0] = children[1] = NULL;

		split_axis = -1; // nutn� n�sledn� aktualizovat podle skute�n� osy d�len�
	}

	~Node()
	{
		SAFE_DELETE( children[1] );
		SAFE_DELETE( children[0] );
	}

	bool is_leaf() const
	{
		return ( ( children[0] == NULL ) && ( children[1] == NULL ) );
	}

	int no_items() const
	{
		return span[1] - span[0] + 1;
	}
};

/*! \struct Bin
\brief Bin.

\author Tom� Fabi�n
\version 0.9
\date 2013
*/
struct Bin
{
public:
	//! Kv�dr obsahuj�c� v�echny polo�ky v binu.
	AABB bounds;

	//! Po�et polo�ek v binu.
	int no_items;	

	Bin()
	{
		no_items = 0;
	}
};

//! Bounding volume hierarchy
/*!
*/
class BVH
{
public:
	BVH( std::vector<Triangle *> * items, const int min_leaf_items );
	~BVH();
	
	void Traverse( Ray & ray, const TYPE_REAL t0 = 0 );	

	void print_stats();

	//! Vykreslen� cel�ho stromu
	/*void Draw()
	{
		Draw( root_, 0 );
	}*/

protected:
	//! Ko�enov� uzel.
	Node * root_;

	//! Po�et item�.
	//int number_of_items_;

	//! Pole item�.
	//Triangle ** items_;
	std::vector<Triangle *> * items_;

	//! Maxim�ln� po�et prvk� v listu.
	int max_leaf_items_;

	//! Po�et list�.
	int number_of_leafs_;

	//! Po�et v�ech uzl� v�etn� list�.
	int number_of_nodes_;

	//! Maxim�ln� hloubka vygenerovan�ho stromu. Ko�en m� hloubku 0.
	int max_depth_;

	long long ray_box_intersections_;

	long long ray_triangle_intersections_;

	int no_nonoverlapping_nodes_;

	//! Pomocn� atribut, po�et item�, kter� ji� byly za�len�ny do list�.
	int processed_items_;

private:
	int FindPivot( Node * node, const int from, const int to, const char axis, TYPE_REAL & cost ) const;

	//! Generov�n� stromu metodou top-down.
	Node * BuildTree( int from, int to, const int depth );

	void Traverse( Ray & ray, Node * node, TYPE_REAL t0, TYPE_REAL t1, int depth );

	//! Vykreslen� zadan�ho uzlu a v�ech jeho potomk�.
	//void Draw( Node * node, int depth );
};

#endif
