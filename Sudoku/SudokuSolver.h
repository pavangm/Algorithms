#include<iostream>
#include "my570list.hpp"
using namespace std;

#ifndef SUDOKU_SOLVER_H
#define SUDOKU_SOLVER_H

class SudokuSolver
{

private:
	char **_sudoku_input_matrix;
	int _board_size;
	My570List *solution_nodes_list;

	bool _Initialize(istream&);
	My570List **priority_queue;
	bool _simple_solve_Recursive(char[][9], My570List**);
	bool _count_solve_Recursive(char input_matrix[][9], My570List **priority_queue);
	void _initialize_priority_queue();
	My570List *solution_list;
	void _add_to_solution_list(char[][9]);
	void _print_matrix(char** next_input_matrix, bool=true, bool=true);
	void _print_matrix_all_solutions();
	bool _generate_puzzle(istream&);
	void _initialize_random_shauffle_input();
	void _fill_priority_queues_using_randomshuffle();
	void _delete_solution_list();
	void _allocate_priority_queue();
	My570ListElem *_get_element_to_be_covered(int index);

public:
	bool should_print_all;
	int max_number_of_solution_to_be_searched;
	bool print_priority_queues;
	bool is_option_solve;
	bool is_option_count;
	bool is_option_gen;
	const char *error_message;
	int error_line_number;
	char ** random_shaffle_input;
	SudokuSolver();
	~SudokuSolver();

	bool Solve(istream&);

};

#endif
