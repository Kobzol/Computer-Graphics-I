#include "stdafx.h"

BVH::BVH( std::vector<Triangle *> * items, const int max_leaf_items )
{
	assert( ( items != NULL ) && ( max_leaf_items > 0 ) );	

	items_ = items;
	max_leaf_items_ = max_leaf_items;

	number_of_leafs_ = 0;
	number_of_nodes_ = 0;
	max_depth_ = 0;
	no_nonoverlapping_nodes_ = 0;
	processed_items_ = 0;

	ray_box_intersections_ = 0;
	ray_triangle_intersections_ = 0;

	printf( "Building BVH...\n" );

	double build_time = omp_get_wtime();
	root_ = BuildTree( 0, items_->size() - 1, 0 );	
	build_time = omp_get_wtime() - build_time;

	printf( "\r%d nodes (%0.1f KB), %d leafs, %d items, max depth %d\n",
		number_of_nodes_, number_of_nodes_ * sizeof( items_->at( 0 ) ) / 1024.0f,
		number_of_leafs_, root_->no_items(), max_depth_ );	
	printf( "%d pairs of non-overlapping nodes\n", no_nonoverlapping_nodes_ );
	
	printf( "Done in %s.\n\n", TimeToString( build_time ).c_str() );
}

BVH::~BVH()
{	
	SAFE_DELETE( root_ )

	items_ = NULL;
}

/*bool CompareTriangles( Triangle * a, Triangle * b )
{
	return true;
}*/

struct TriangleComparator
{
public:
	TriangleComparator( const char axis )
	{
		axis_ = axis;
	}

	bool operator() ( Triangle * a, Triangle * b )
	{
		return a->bounds().center().data[axis_] < b->bounds().center().data[axis_];
	}

private:
	char axis_;
};

//std::vector<Bin> bins = std::vector<Bin>( NO_AXIS_BINS );
//Bin bins[NO_AXIS_BINS];

int BVH::FindPivot( Node * node, const int from, const int to, const char axis, TYPE_REAL & cost ) const
{
	//return n / 2 + from;
	

	std::vector<Bin> bins = std::vector<Bin>( MIN( node->no_items(), NO_AXIS_BINS ) );
	/*bins.clear();
	bins.resize( MIN( node->no_items(), NO_AXIS_BINS ) );	*/
	const int k = MIN( node->no_items(), NO_AXIS_BINS );
	/*for ( int i = 0; i < k; ++i )
	{
		bins[i] = Bin();
	}*/
	//const int k = bins.size();	

	{
		// biny mohou být rozložený mezi mezemi obalu uzlu
		/*TYPE_REAL & b0 = node->bounds.lower_bound().data[axis];
		TYPE_REAL & b1 = node->bounds.upper_bound().data[axis];*/
		
		// nebo o nìco hustìji mezi extrémními cetroidy položek v seznamu
		// On fast Construction of SAH-based Bounding Volume Hierarchies
		TYPE_REAL b0 = REAL_MAX;
		TYPE_REAL b1 = REAL_MIN;
		for ( int i = from; i <= to ; ++i )
		{
			const TYPE_REAL xi = items_->at( i )->bounds().center().data[axis];
			b0 = MIN( xi, b0 );
			b1 = MAX( xi, b1 );
		}

		const TYPE_REAL db = b1 - b0;

		if ( fabs( db ) < EPSILON )
		{
			cost = REAL_MAX;

			return node->no_items() / 2 + from;
		}

		const TYPE_REAL tmp = k / db;

		for ( int i = from; i <= to; ++i )
		{
			//printf("%d\n", i );
			const AABB item_bounds = items_->at( i )->bounds(); // aabb aktuálnì binovaného trojúhelníka
			const TYPE_REAL xi = item_bounds.center().data[axis]; // vybraná složka centroidu binovaného trojúhelníka
			//const int bi = MIN( k - 1, MAX( 0, static_cast<int>( floor( xi * k + tmp ) ) ) ) ; // index pøíslušného binu
			const int bi = MIN( k - 1, static_cast<int>( floor( ( xi - b0 ) * tmp ) ) ); // index pøíslušného binu
			if ( ( bi < 0 ) || ( bi > k - 1 ) )
			{
				printf("\n");
			}
			Bin & bin_xi = bins[bi]; // pøíslušný bin
			
			++bin_xi.no_items; // inkrementujeme poèet trojúhelníkù v binu
			bin_xi.bounds.Merge( item_bounds ); // a zvìtšíme obal binu
		}
	}

	// mùže nastat situace, že všechny trojúhelníky, resp. jejich centroidy, leží uvnitø jediného binu
	{
		int no_non_empty_bins = 0; // poèet neprázných binù
		int first_non_empty_bin = -1; // index prvního neprázdného binu

		for ( int i = 0; i < k; ++i )
		{		
			if ( bins[i].no_items > 0 )
			{
				++no_non_empty_bins;

				if ( no_non_empty_bins == 1 )
				{
					first_non_empty_bin = i;
				}

				if ( no_non_empty_bins > 1 )
				{
					// staèí vìdìt, že existují alespoò dva neprázdné biny
					break;
				}
			}
		}

		if ( no_non_empty_bins == 1 )
		{
			// existuje-li pouze jeden neprázdný bin, vypoèteme pivot jako støed intervalu
			const Bin & bin = bins[first_non_empty_bin];
			cost = bin.bounds.surface_area() * bin.no_items;;

			const int pivot = node->no_items() / 2 + from;

			assert( ( ( pivot - 1 ) - from + 1 ) + ( to - pivot + 1 ) == node->no_items() );

			return pivot;			
		}

		// v ostatních pøípadech víme, že existují alespoò dva biny, a mùžeme pokraèovat standardnì
	}

	int pivot = from + 1; // <from, pivot - 1> a <pivot, to>
	TYPE_REAL c_min = REAL_MAX;

	for ( int i = 1; i < k; ++i )
	{		
		int no_items[] = { 0, 0 };
		AABB bounds[2];

		// left
		for ( int j = 0; j < i; ++j )
		{
			const Bin & bin = bins[j];
			no_items[0] += bin.no_items;			
			bounds[0].Merge( bin.bounds );
		}		

		// right
		for ( int j = i; j < k; ++j )
		{
			const Bin & bin = bins[j];
			no_items[1] += bin.no_items;
			bounds[1].Merge( bin.bounds );
		}

		const TYPE_REAL c = bounds[0].surface_area() * no_items[0] +
			bounds[1].surface_area() * no_items[1];

		if ( c < c_min )
		{
			c_min = c;
			pivot = from + no_items[0];
		}
	}

	cost = c_min;

	return pivot;
}


Node * BVH::BuildTree( int from, int to, const int depth )
{
	Node * node = new Node( from, to );
	const int n = node->no_items(); //to - from + 1; // aktuální poèet itemù v uzlu

//#pragma omp atomic // FIX proc atomic?
	++number_of_nodes_;

	// vygenerování obalu pro itemy v intervalu <from, to>
	for ( int i = from; i <= to; ++i )
	{
		node->bounds.Merge( items_->at( i )->bounds() );
	}

	if ( depth == 0 )
	{
		printf( "root bounds=(%0.3f, %0.3f, %0.3f) x (%0.3f, %0.3f, %0.3f)\n",
			node->bounds.lower_bound().x, node->bounds.lower_bound().y, node->bounds.lower_bound().z,
			node->bounds.upper_bound().x, node->bounds.upper_bound().y, node->bounds.upper_bound().z );
	}

	// pro pøípad ( to - from + 1 ) <= max_leaf_items_ je uzel zmìnìn na list
	if ( n <= max_leaf_items_ )
	{
//#pragma omp atomic
		++number_of_leafs_;
		//#pragma omp atomic
		max_depth_ = MAX( depth, max_depth_ );

//#pragma omp atomic
		processed_items_ += n;
		if ( processed_items_ % 100 == 0 ) // omezení èetnosti pomalých výpisù
		{
			printf( "\r%0.1f %%, max depth=%d", processed_items_ / ( items_->size() * 1e-2 ), max_depth_ );
		}

		return node; // ukonèení rekurze
	}

	// nejdelší osa aabb uzlu	
	//char axis = -1;
	int pivot = -1;
	TYPE_REAL cost = REAL_MAX;
	for ( char axis_actual = 0; axis_actual < 3; ++axis_actual )
	{
		TYPE_REAL cost_actual = 0;
		const int pivot_actual = FindPivot( node, from, to, axis_actual, cost_actual );

		/*if ( cost_actual == REAL_MAX )
		{
			printf("\n");
		}		*/

		if ( cost_actual < cost )
		{
			node->split_axis = axis_actual;
			pivot = pivot_actual;
			cost = cost_actual;			
		}
	}

	/*if ( pivot < 0 )
	{
		TYPE_REAL cost_actual = 0;
		int pivot_actual = FindPivot( node, from, to, 0, cost_actual );
		pivot_actual = FindPivot( node, from, to, 1, cost_actual );
		pivot_actual = FindPivot( node, from, to, 2, cost_actual );
		printf("\n");
	}*/
	
	
	/*TYPE_REAL cost = 0;
	const int pivot = FindPivot( node, from, to, node->bounds.dominant_axis(), cost );*/

	// mid point?
	//Vector3 centroid = node->bounding.Centroid();
	//const TYPE_REAL pivot = centroid.data[axis];
	//const int pivot = n / 2 + from;

	// check
	assert( ( ( pivot - 1 ) - from + 1 ) + ( to - pivot + 1 ) == n );
	assert( ( node->split_axis >= 0 ) && ( node->split_axis < 3 ) );

	/*const TYPE_REAL left_relative = static_cast<TYPE_REAL>( pivot - from ) / n;
	const TYPE_REAL right_relative = 1 - left_relative ;

	if ( ( left_relative < 0.05 ) || ( right_relative < 0.05 ) )
	{
		printf( "\n" );
	}*/

	// seøazení centroidù itemù podél osy axis (vzestupnì)
	//Sort( &items_[from], n, axis );
	std::vector<Triangle *>::iterator begin = items_->begin();
	//std::sort( begin + from, begin + to + 1, TriangleComparator( axis ) ); // O(n*log(n))
	// interval <from, pivot - 1> bude seøazen vzestupnì a v intervalu <pivot, to> budou všechny prvky vìtší než v prvním, ale budou nesetøídìné
	//std::partial_sort( begin + from, begin + pivot, begin + to + 1, TriangleComparator( node->split_axis ) );
	std::nth_element( begin + from, begin + pivot, begin + to + 1,  TriangleComparator( node->split_axis ) ); // O(n)

	// rozdìlení na dva podintervaly <from, pivot - 1> a <pivot, to> (podle poètu prvkù na dvì ~ poloviny)
	//axis = ( axis + 1 ) % 3; // výbìr následující osy
	node->children[0] = BuildTree( from, pivot - 1, depth + 1 );
	node->children[1] = BuildTree( pivot, to, depth + 1 );		

	if ( !BoxBoxIntersection( node->children[0]->bounds, node->children[1]->bounds ) )
	{
//#pragma omp atomic
		++no_nonoverlapping_nodes_;
	}

	return node;
}

void BVH::Traverse( Ray & ray, const TYPE_REAL t0 )
{
	//ray_box_intersections_ = ray_triangle_intersections_ = 0;

	Traverse( ray, root_, t0, ray.t, 0 );

#ifdef DEBUG_BVH
	printf( "ray box intersections=%d\n", ray_box_intersections_ );
	printf( "ray tri intersections=%d\n", ray_triangle_intersections_ );
	printf( "total intersections=%d (i.e. %0.2f times lesser tests)\n",
		ray_box_intersections_ + ray_triangle_intersections_,
		static_cast<TYPE_REAL>( number_of_items_ ) / ( ray_box_intersections_ + ray_triangle_intersections_ ) );
#endif
}

void BVH::Traverse( Ray & ray, Node * node, TYPE_REAL t0, TYPE_REAL t1, int depth )
{
#pragma omp atomic
	++ray_box_intersections_;

	if ( RayBoxIntersection( ray, node->bounds, t0, t1 ) )
	{
		if ( node->is_leaf() )
		{
			// testovat všechny itemy listu pro <t0, t1>
			// a atomicky aktualizovat ray.t
#ifdef DEBUG_BVH
			printf( "Entering leaf of %d items at depth %d, t in <%0.3f, %0.3f>\n", node->NumberOfItems(), depth, t0, t1 );
#endif

			for ( int i = node->span[0]; i <= node->span[1]; ++i )
			{				
#pragma omp atomic
				++ray_triangle_intersections_;

				if ( RayTriangleIntersectionMT97( ray, items_->at( i ) ) ) // FIX intersection patri do Triangle
				{
					//ray.target.surface = 0;
					//ray.target.face = i;					
					//ray.target.triangle = items_[i];
					
#ifdef DEBUG_BVH
					Vector3 intersection = ray.Eval( ray.t );
					printf( "hit item[%d] at (%0.3f, %0.3f, %0.3f)\n", i, intersection.x, intersection.y, intersection.z );
#endif
				}
			}
		}
		else
		{
			// testovat potomky
			char first = 0;
			char second = 1;

			if ( ray.direction.data[node->split_axis] < 0 )
			{
				first = 1;
				second = 0;
			}

			// testujeme prvního potomka
			Traverse( ray, node->children[first], t0, ray.t, depth + 1 );
			// a druhého jen v pøípadì, že to má smysl
			Traverse( ray, node->children[second], t0, ray.t, depth + 1 );
		}
	}
	else
	{
		// paprsek letí mimo uzel, konec
#ifdef DEBUG_BVH
		printf( "Closing node at depth %d, %d items rejected\n", depth, node->NumberOfItems() );
#endif
	}
}

void BVH::print_stats()
{
	printf( "ray box intersections=%lld\n", ray_box_intersections_ );
	printf( "ray triangle intersections=%lld\n", ray_triangle_intersections_ );
}

/*void BVH::Draw( Node * node, int depth )
{
//TYPE_REAL k = 1 - depth / static_cast<TYPE_REAL>( max_depth_ );
TYPE_REAL k = depth / static_cast<TYPE_REAL>( max_depth_ );
glColor3f( 1.0f * k, 0.25f * k, 0.25f * k );
CreateWiredBox( node->bounding.bounds[0], node->bounding.bounds[1] );

if ( node->children[0] != NULL )
{
Draw( node->children[0], depth + 1 );
}

if ( node->children[1] != NULL )
{
Draw( node->children[1], depth + 1 );
}
}*/


//http://www.java2s.com/Code/Java/Collections-Data-Structure/FastQuickSort.htm
//http://alienryderflex.com/quicksort/
/*void Sort( Triangle ** items, const int number_of_items, const char axis )
{
#define MAX_LEVELS 300
	int beg[MAX_LEVELS], end[MAX_LEVELS], i = 0, L, R, swap;
	Triangle * pivot = NULL;

	beg[0] = 0;
	end[0] = number_of_items;

	while ( i >= 0 )
	{
		L = beg[i];
		R = end[i] - 1;

		if ( L < R )
		{
			pivot = items[L];

			while ( L < R )
			{
				while ( items[R]->bounds().center().data[axis] >=
					pivot->bounds().center().data[axis] && L < R )
				{
					--R;
				}

				if ( L < R )
				{
					items[L++] = items[R];
				}

				while ( items[L]->bounds().center().data[axis] <=
					pivot->bounds().center().data[axis] && L < R )
				{
					++L;
				}

				if ( L < R )
				{
					items[R--] = items[L];
				}
			}

			items[L] = pivot;
			beg[i + 1] = L + 1;
			end[i + 1] = end[i];
			end[i++] = L;

			if ( end[i] - beg[i] > end[i - 1] - beg[i - 1] )
			{
				swap = beg[i]; beg[i] = beg[i - 1]; beg[i - 1] = swap;
				swap = end[i]; end[i] = end[i - 1]; end[i - 1] = swap;
			}
		}
		else
		{
			--i;
		}
	}
}
*/