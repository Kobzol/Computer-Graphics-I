#ifndef BVH_H_
#define BVH_H_

#define NO_AXIS_BINS 64

/*
Originální verze
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

\author Tomáš Fabián
\version 0.9
\date 2011-2012
*/
struct Node
{
public:
	//! Kvádr obsahující všechny itemy v uzlu.
	AABB bounds;

	//! Uzavøený interval indexù všech itemù, které patøí do uzlu.
	int span[2];

	//! Levý a pravý poduzel.
	Node * children[2];

	//! Osa použitá pro dìlení tohoto uzlu.
	char split_axis;

	Node( const int from, const int to )
	{
		span[0] = from;
		span[1] = to;

		children[0] = children[1] = NULL;

		split_axis = -1; // nutné následnì aktualizovat podle skuteèné osy dìlení
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

\author Tomáš Fabián
\version 0.9
\date 2013
*/
struct Bin
{
public:
	//! Kvádr obsahující všechny položky v binu.
	AABB bounds;

	//! Poèet položek v binu.
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

	//! Vykreslení celého stromu
	/*void Draw()
	{
		Draw( root_, 0 );
	}*/

protected:
	//! Koøenový uzel.
	Node * root_;

	//! Poèet itemù.
	//int number_of_items_;

	//! Pole itemù.
	//Triangle ** items_;
	std::vector<Triangle *> * items_;

	//! Maximální poèet prvkù v listu.
	int max_leaf_items_;

	//! Poèet listù.
	int number_of_leafs_;

	//! Poèet všech uzlù vèetnì listù.
	int number_of_nodes_;

	//! Maximální hloubka vygenerovaného stromu. Koøen má hloubku 0.
	int max_depth_;

	long long ray_box_intersections_;

	long long ray_triangle_intersections_;

	int no_nonoverlapping_nodes_;

	//! Pomocný atribut, poèet itemù, které již byly zaèlenìny do listù.
	int processed_items_;

private:
	int FindPivot( Node * node, const int from, const int to, const char axis, TYPE_REAL & cost ) const;

	//! Generování stromu metodou top-down.
	Node * BuildTree( int from, int to, const int depth );

	void Traverse( Ray & ray, Node * node, TYPE_REAL t0, TYPE_REAL t1, int depth );

	//! Vykreslení zadaného uzlu a všech jeho potomkù.
	//void Draw( Node * node, int depth );
};

#endif
