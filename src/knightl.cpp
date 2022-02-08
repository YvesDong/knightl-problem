/*
 * Solution to the KnightL challege based on a dynamic programming approach.
 *
 * Background introduction:
 * https://www.hackerrank.com/challenges/knightl-on-chessboard/problem
 * 
 * Input: the size of the chessboard.
 * Output: a 2D integer array, in which each line i (1 <= i < n) contains (n-1) space-separated integers describing the minimum 
 * number of moves KnightL(i,j) must make for each respective j (1 <= j < n). If some KnightL(i,j) cannot reach position (0,0), 
 * print -1 instead.
 *
 * Author: Yifei Dong
 * Affiliation: F&P Robotics
 * 
 * Reference:
 * – Problem statements:
 * https://www.hackerrank.com/challenges/knightl-on-chessboard/problem
 * https://docs.google.com/document/d/1cuhZcJokYDMjm4Np8cnU9U0b1O9qnQAAn17EjREvjOU/edit#
 * – Dynamic Programming (DP):
 * https://en.wikipedia.org/wiki/Dynamic_programming
 * – Implementation:
 * https://medium.com/codebrace/knightl-on-a-chessboard-384c1efaf9a1
 */

#include "knightl.h"

size_t number_of_digits(int n)
{
	/*
	 * Determine the number of digits of an integer.
	 * 
	 * Input:
	 * int n: the integer of interest.
	 */

	std::ostringstream strs;
	strs << n;
	return strs.str().size();
}

void print_matrix(int M[nmax][nmax], int n, int m)
{
	/*
	 * A helper function to print clean and nice matrix.
	 *
	 * Input: 
	 * int M[nmax][nmax]: a 2D array to be printed.
	 * int n: the row number.
	 * int m: the column number.
	 */

	int max_len_per_column[nmax];

	for (int j = 0; j < m; j++)
	{
		int max_len = 0;
		for (int i = 0; i < n; i++)
		{
			int num_length = number_of_digits(M[i][j]);
			if (num_length > max_len)
			{
				max_len = num_length;
			}
		}
		max_len_per_column[j] = max_len;
	}
	for (int i = 0; i < n; i++)
	{
		cout << endl
			 << setw(2) << "|" << setw(1);
		for (int j = 0; j < m; j++)
		{
			cout << setw(max_len_per_column[j] + 1);
			cout << M[i][j];
			if (j == m - 1)
				cout << setw(1) << " |";
		}
	}
}

Seeker::Seeker(int n_)
{
	/*
	 * Instantiation of the constructor of the shortest path seeker.
	 * 
	 * Input: 
	 * int n_: the user-input size n of the chessboard.
	 */

	n = n_;
}

Seeker::~Seeker(void){}

void Seeker::reset(int k_, int l_)
{
	/*
	 * The L-shape and the queue are reset after each time a shortest path is found for a specific L-shape.
	 *
	 * Input: 
	 * int k_: incoming L-shape (k_,l_)
	 * int l_: incoming L-shape (k_,l_)
	 */

	// reset L-shape
	k = k_;
	l = l_;
	
	// reset queue by swapping with an empty queue
	queue<pair<int, int>> empty;
	std::swap(que, empty);

	// reset function matrices
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
		{
			is_pushed[i][j] = false;
			is_added[i][j] = false;
			count[i][j] = 0;
		}
}

bool Seeker::check_approachable_block(int p, int q, int count_)
{
	/*
	 * Check one of all eight reachable blocks in the neighborhood.
	 *
	 * Input: 
	 * int M[nmax][nmax]: a 2D array to be printed.
	 * int n: the row number.
	 * int m: the column number.
	 * 
	 * Return:
	 * bool reached_target: true only if the target block is reached. 
	 */

	bool reached_target = false;
	// check if the block is inside the chessboard
	if (p >= 0 && q >= 0 && p < n && q < n)
	{
		// check if the block was pushed
		if (is_pushed[p][q] == true)
			count[p][q] = min(count[p][q], count_ + 1);
		else
		{
			count[p][q] = count_ + 1;
			// check if the target is reached
			if (p == 0 && q == 0)
			{
				is_pushed[0][0] = true;
				reached_target = true;
				return reached_target;
			}
			// check if the block was added to queue
			if (is_added[p][q] == false)
			{
				que.push(make_pair(p, q));
				is_added[p][q] = true;
			}
		}
	}
	return reached_target;
}

int Seeker::run_dp_loop()
{
	/*
	 * Find the shortest path on a given chessboard with a given L-shape knight, based on DP approach.
	 * The function utilizes a queue to explore unmarked blocks and mark explored blocks.
	 * The loop ends when the exploration set is empty or the target block is reached.
	 *
	 * Return:
	 * int min_move: min number of moves using the current L-shape.
	 */

	int x, y;
	int count_;
	bool reached_target_ = false;

	// push the start block
	que.push(make_pair(n-1, n-1));
	is_added[n-1][n-1] = true;

	// running the loop till the queue is empty or the loop breaks inside
	while (que.size() != 0)
	{
		// push the frontier of the queue
		x = que.front().first;
		y = que.front().second;
		is_pushed[x][y] = true;
		count_ = count[x][y];
		que.pop();
		int p, q;

		// check and process all eight approachable blocks
		int candidate_block_set[8][2] = {{x+k, y+l}, {x+k, y-l}, {x-k, y+l}, {x-k, y-l}, 
										{x+l, y+k}, {x+l, y-k}, {x-l, y+k}, {x-l, y-k}};
		for (int i = 0; i < 8; i++)
		{
			p = candidate_block_set[i][0];
			q = candidate_block_set[i][1];
			reached_target_ = check_approachable_block(p, q, count_);
			if (reached_target_ == true)
				break;
		}
		if (reached_target_ == true)
			break;
	}

	//if target is not reached at last, set min move to -1
	int min_move = -1;
	if (is_pushed[0][0] == true)
		min_move = count[x][y] + 1;

	return min_move;
}

int main()
{
	/*
	 * Main function of the KnightL problem.
	 */

	// Input the chessboard size
	int n;
	cout << "Please input the size n of the chessboard (n*n, 1<n<=50): " << endl;
	cin >> n;
	while (n > nmax || n <= 1)
	{
		cout << "Your input is NOT valid! Please input the size n, s.t. 1<n<=50: " << endl;
		cin >> n;
	}
	int result_matrix[nmax][nmax];
	Seeker seeker(n);
	auto start = high_resolution_clock::now();

	// iterate over all available L-shapes
	for (int k = 1; k <= n-1; k++)
	{
		for (int l = k; l <= n-1; l++)
		{
			seeker.reset(k, l);
			int min_move = seeker.run_dp_loop();
			result_matrix[k-1][l-1] = min_move;
			result_matrix[l-1][k-1] = min_move;
		}
	}

	// print result matrix
	print_matrix(result_matrix, n-1, n-1);

	// print elapsed time
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop-start);
	cout << endl << "Total elapsed time (ms): " << duration.count() << endl;
}