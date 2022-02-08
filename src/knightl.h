#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <queue>
#include <chrono>

using namespace std::chrono;
using namespace std;

constexpr size_t nmax{50};	// max chessboard size

class Seeker{
	public:
		int n;	// size of chessboard (n*n)
		int k;	// knight's L_shape move (k, l)
		int l;

		queue<pair<int, int>> que;			// queue that contains blocks yet to explore
		bool is_pushed[nmax][nmax];			// true if a block is pushed from the queue
		bool is_added[nmax][nmax];			// true if a block is regarded new and added to the queue
		int count[nmax][nmax];				// min number of moves from start (s) to goal (g)

		Seeker(int n);						// constructor
		~Seeker();							// deconstructor

		void reset(int k, int l);			// reset L-shape, queue and function
		int run_dp_loop();					// run the shortest path search algorithm with a given L-shape
		bool check_approachable_block(int p, int q, int count_);	// check one of neighbor blocks of the block pushed from queue
};