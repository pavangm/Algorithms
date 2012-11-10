#include "stdafx.h"
#include "SudokuSolver.h"
#include <iostream>
#include <stdint.h>
#include "my570list.hpp"
#include "puzzleNode.h"

using namespace std;

SudokuSolver::SudokuSolver()
{
	this->_board_size = 9;
	this->_sudoku_input_matrix = new char*[this->_board_size];
	for(int i=0;i<this->_board_size;i++)
		this->_sudoku_input_matrix[i] = new char[this->_board_size];

	this->should_print_all = false;
	this->max_number_of_solution_to_be_searched = 0;
	this->print_priority_queues = false;

	this->is_option_count = false;
	this->is_option_count = false;
	this->is_option_solve = false;

	this->error_line_number = 0;
	this->error_message = "";
	_allocate_priority_queue();

	this->solution_list = new My570List();
	this->random_shaffle_input = NULL;

	this->solution_nodes_list = NULL;
}

void SudokuSolver::_allocate_priority_queue()
{
	this->priority_queue = new My570List *[9];
	for(int i=0;i<this->_board_size;i++)
		this->priority_queue[i] = new My570List();
}

void SudokuSolver::_delete_solution_list()
{
	if(this->solution_list != NULL)
	{
		My570ListElem *ielem = this->solution_list->First();
		while(ielem != NULL)
		{
			char** solution_matrix = (char **)ielem->Obj();
			for(int i=0;i<this->_board_size ; i++)
				delete solution_matrix[i];
			delete solution_matrix;
			My570ListElem *nelem = this->solution_list->Next(ielem);
			this->solution_list->Unlink(ielem);
			ielem = nelem;
		}
		delete this->solution_list;
		this->solution_list = NULL;
	}
}

SudokuSolver::~SudokuSolver()
{
	if(_sudoku_input_matrix != NULL)
	{
		for(int i=0;i<this->_board_size;i++)
			delete this->_sudoku_input_matrix[i];

		delete this->_sudoku_input_matrix;
		_sudoku_input_matrix = NULL;
	}

	if(priority_queue != NULL)
	{
		for(int i=0;i<this->_board_size;i++)
			delete this->priority_queue[i];

		delete this->priority_queue;
	}

	_delete_solution_list();
}

int get_current_row_index(My570List **priority_queue)
{
	for(int i=0; i<9; i++)
	{
		if(!priority_queue[i]->Empty())
			return i;
	}
	return -1;
}

int get_current_col_index(char input_matrix[][9], int row_index)
{
	for(int i=0;i<9;i++)
		if(input_matrix[row_index][i] == '.')
			return i;
	return -1;

}

My570List ** get_next_priority_queue(My570List **priority_queue, My570ListElem *elem_to_be_removed)
{
	My570List **next_priority_queue = new My570List *[9];
	for(int i=0;i<9;i++)
		next_priority_queue[i] = new My570List();

	for(int i=0;i<9;i++)
	{
		My570ListElem *ielem = priority_queue[i]->First();

		while(ielem != NULL)
		{
			char *cur_char = new char[1];
			*cur_char = *((char *)ielem->Obj());
			next_priority_queue[i]->Append(cur_char);
			ielem = priority_queue[i]->Next(ielem);
		}
	}

	for(int i=0;i<9;i++)
	{
		if(!next_priority_queue[i]->Empty())
		{
			char char_in_element_to_be_removed = *((char *)elem_to_be_removed->Obj());
			My570ListElem *ielement_pq = next_priority_queue[i]->First();
			while(ielement_pq != NULL)
			{
				char *element_pq_char = (char *)ielement_pq->Obj();
				if(*element_pq_char == char_in_element_to_be_removed) break;
				ielement_pq = next_priority_queue[i]->Next(ielement_pq);
			}

			delete (char *)ielement_pq->Obj();
			next_priority_queue[i]->Unlink(ielement_pq);
			break;
		}
	}

	return next_priority_queue;
}

My570List ** get_next_priority_queue(My570List **priority_queue)
{
	My570List **next_priority_queue = new My570List *[9];
	for(int i=0;i<9;i++)
		next_priority_queue[i] = new My570List();

	for(int i=0;i<9;i++)
	{
		My570ListElem *ielem = priority_queue[i]->First();

		while(ielem != NULL)
		{
			char *cur_char = new char[1];
			*cur_char = *((char *)ielem->Obj());
			next_priority_queue[i]->Append(cur_char);
			ielem = priority_queue[i]->Next(ielem);
		}
	}

	for(int i=0;i<9;i++)
	{
		if(!next_priority_queue[i]->Empty())
		{
			My570ListElem *first_element_pq = next_priority_queue[i]->First();
			char *first_element_pq_char = (char *)first_element_pq->Obj();
			next_priority_queue[i]->Unlink(first_element_pq);
			delete first_element_pq_char;
			break;
		}
	}

	return next_priority_queue;
}

void deallocate_priority_queue(My570List **priority_queue)
{
	//for(int i=0;i<9;i++)
		//priority_queue[i] = new My570List();

	for(int i=0;i<9;i++)
	{
		My570ListElem *ielem = priority_queue[i]->First();

		while(ielem != NULL)
		{
			char *cur_char = (char *)ielem->Obj();
			My570ListElem *nelem = priority_queue[i]->Next(ielem);
			priority_queue[i]->Unlink(ielem);
			delete cur_char;
			ielem = nelem;
		}
	}

	for(int i=0;i<9;i++)
		delete priority_queue[i];

	delete priority_queue;
	priority_queue = NULL;
	return;

}

bool _can_pqchar_be_placed_at(char input_matrix[][9], int row_index, int col_index, char pq_char)
{
	//scan Row. Not required because pq is based on Row_index.

	//scan col.
	for(int i=0;i<9;i++)
	{
		if(input_matrix[i][col_index] == pq_char)
			return false;
	}

	//scan box
	int box_row_index = row_index/3;
	int box_col_index = col_index/3;
	for(int i=box_row_index*3 ; i<box_row_index*3 + 3; i++)
		for(int j=box_col_index*3 ; j<box_col_index*3 + 3; j++)
			if(input_matrix[i][j] == pq_char)
				return false;
	return true;
}

void _move_first_element_to_end(My570List *list)
{
	My570ListElem *first_element_pq = list->First();
	char *first_element_pq_char = (char *)first_element_pq->Obj();
	list->Unlink(first_element_pq);
	list->Append(first_element_pq_char);
}

void copy_arrays(char dest[][9], char src[][9])
{
	for(int i=0;i<9;i++)
		for(int j=0; j<9; j++)
			dest[i][j] = src[i][j];
	return;
}

void copy_arrays(char dest[][9], char** src)
{
	for(int i=0;i<9;i++)
		for(int j=0; j<9; j++)
			dest[i][j] = src[i][j];
	return;
}

void copy_arrays(char** dest, char src[][9])
{
	for(int i=0;i<9;i++)
		for(int j=0; j<9; j++)
			dest[i][j] = src[i][j];
	return;
}

void copy_arrays(char** dest, char** src)
{
	for(int i=0;i<9;i++)
		for(int j=0; j<9; j++)
			dest[i][j] = src[i][j];
	return;
}

void SudokuSolver::_print_matrix(char** next_input_matrix, bool should_print_boundaries, bool should_check_input)
{
	if(should_print_boundaries == true)
		cout<<"+---+---+---+"<<endl;
	for(int i=0;i<9;i++)
	{

		if(should_print_boundaries == true) cout<<"|";
		for(int j=0;j<9;j++)
		{
			if(should_check_input==true) 
				if(this->_sudoku_input_matrix[i][j] == '.')
					cout<<next_input_matrix[i][j];
				else
					cout<<".";
			else
				cout<<next_input_matrix[i][j];

			if(should_print_boundaries == true)
			if((j+1)%3 == 0)
				cout<<"|";
		}
		cout<<endl;
		if(should_print_boundaries == true)
		if((i+1) % 3 == 0)
			cout<<"+---+---+---+"<<endl;

	}
	return;
}

void _print_matrix(char next_input_matrix[][9])
{
	cout<<"+---+---+---+"<<endl;;
	for(int i=0;i<9;i++)
	{
		cout<<"|";
		for(int j=0;j<9;j++)
		{
			cout<<next_input_matrix[i][j];
			cout<<".";

			if((j+1)%3 == 0)
				cout<<"|";
		}
		cout<<endl;
		if((i+1) % 3 == 0)
			cout<<"+---+---+---+"<<endl;
	}

}

void SudokuSolver::_add_to_solution_list(char matrix_to_be_added[][9])
{
	char **new_solution_matrix = new char*[this->_board_size];
	for(int i=0;i<this->_board_size;i++)
		new_solution_matrix[i] = new char[this->_board_size];

	for(int i=0; i<this->_board_size;i++)
		for(int j=0; j<this->_board_size; j++)
			new_solution_matrix[i][j] = matrix_to_be_added[i][j];

	this->solution_list->Append(new_solution_matrix);
	return;
}

bool SudokuSolver::_simple_solve_Recursive(char input_matrix[][9], My570List **priority_queue)
{
	int row_index = get_current_row_index(priority_queue);
	
	if(row_index == -1)
	{
		_add_to_solution_list(input_matrix);
		return true;
	}
	char next_input_matrix[9][9];

	copy_arrays(next_input_matrix, input_matrix);

	int col_index = get_current_col_index(input_matrix, row_index);
	
	My570ListElem *ielem = priority_queue[row_index]->First();
	
	while(ielem != NULL)
	{
		char *priority_char = (char *)ielem->Obj();
		char p_char = *priority_char;
		
		if(_can_pqchar_be_placed_at(input_matrix,row_index,col_index, *priority_char) == true)
			next_input_matrix[row_index][col_index] = *priority_char;
		else
		{
			ielem = priority_queue[row_index]->Next(ielem);
			continue;
		}

		My570ListElem *next_elem = priority_queue[row_index]->Next(ielem);
		char *next_elem_obj = NULL;
		if(next_elem != NULL)
			next_elem_obj = (char *)next_elem->Obj();
		priority_queue[row_index]->Unlink(ielem);
		ielem = NULL;
		
		bool is_solved_sucessfully = _simple_solve_Recursive(next_input_matrix, priority_queue);
		if(is_solved_sucessfully == true)
		{
			cout<<"Solved:";
			/*
			if(next_elem == NULL)
				priority_queue[row_index]->Append(priority_char);
			else
			{
				My570ListElem *next_elem = priority_queue[row_index]->Find(next_elem_obj);
				priority_queue[row_index]->InsertBefore(priority_char, next_elem);
			}
			*/
			return true;
		}
		else
		{
			if(next_elem == NULL)
			{
				priority_queue[row_index]->Append(priority_char);
			}
			else
			{
				cout<<"Priority Char"<<*priority_char;
				My570ListElem *next_elem = priority_queue[row_index]->Find(next_elem_obj);
				priority_queue[row_index]->InsertBefore(priority_char,next_elem);
			}
		}
		
		ielem = next_elem;
		
	}
	
	return false;
}

bool SudokuSolver::_count_solve_Recursive(char input_matrix[][9], My570List **priority_queue)
{
	int row_index = get_current_row_index(priority_queue);

	if(row_index == -1)
	{
		_add_to_solution_list(input_matrix);
		return true;
	}
	char next_input_matrix[9][9];

	copy_arrays(next_input_matrix, input_matrix);

	int col_index = get_current_col_index(input_matrix, row_index);
	int priority_queue_length = priority_queue[row_index]->Length();
	int iteration_num = 1;
	My570ListElem *ielem = priority_queue[row_index]->First();

	while(iteration_num <= priority_queue_length)
	{
		
		char priority_char = *((char *)ielem->Obj());
		if(_can_pqchar_be_placed_at(input_matrix,row_index,col_index, priority_char) == true)
			next_input_matrix[row_index][col_index] = priority_char;
		else
		{
			//_move_first_element_to_end(priority_queue[row_index]);
			ielem = this->priority_queue[row_index]->Next(ielem);
			iteration_num++;
			continue;
		}
		My570List **next_priority_queue = get_next_priority_queue(priority_queue, ielem);
		bool is_solved_sucessfully = _count_solve_Recursive(next_input_matrix, next_priority_queue);
		if(is_solved_sucessfully == true)
		{
			deallocate_priority_queue(next_priority_queue);
			if(this->max_number_of_solution_to_be_searched != 0 && this->solution_list->Length() == this->max_number_of_solution_to_be_searched)
				return true;
			else
			{
				_move_first_element_to_end(priority_queue[row_index]);
			}
		}
		else
		{
			deallocate_priority_queue(next_priority_queue);
			//_move_first_element_to_end(priority_queue[row_index]);
		}
		iteration_num++;
		ielem = this->priority_queue[row_index]->Next(ielem);
	}
	return false;
}

void SudokuSolver::_initialize_priority_queue()
{
	char line[10];
	for(int i=0; i<this->_board_size; i++)
	{
		for(int j=0; j<this->_board_size + 1; j++)
			line[j] = '.';
		for(int j=0; j<this->_board_size; j++)
		{
			if(this->_sudoku_input_matrix[i][j] != '.')
				line[_sudoku_input_matrix[i][j] - '0'] = 'Y';
		}
		for(int j=0;j<this->_board_size;j++)
		{
			if(line[j+1] != 'Y')
			{
				char *c = new char[1];
				*c = (char)(j+1 + '0');
				this->priority_queue[i]->Append(c);
			}
		}
	}

	return;
}

bool SudokuSolver::_Initialize(istream &ist)
{
	int total_number_of_numbers_in_sudoku = this->_board_size * this->_board_size;
	int number_of_valid_characters = 0;
	int line_number = 1;
	while(!ist.eof())
	{
		char *cur_line = new char[1025];
		ist.getline(cur_line, 1025);
		int number_of_valid_chars_in_cur_line = 0;
		for(unsigned int i=0; i<strlen(cur_line);i++)
		{
			char cur_char = cur_line[i];
			if((cur_char - '0' >=0 && cur_char - '0' <=9 ) || (cur_char == '.'))
			{
				this->_sudoku_input_matrix[number_of_valid_characters/this->_board_size][number_of_valid_characters%this->_board_size] = cur_char;
				number_of_valid_characters++;
				number_of_valid_chars_in_cur_line++;
			}
			if(number_of_valid_chars_in_cur_line > 9)
			{
				this->error_message = "The number of valid characters in the line cannot be greater than 9.";
				this->error_line_number = line_number;
				return false;
			}
		}

		if(number_of_valid_chars_in_cur_line >0 && number_of_valid_chars_in_cur_line < 9)
		{
			this->error_message = "The number of valid characters in the line can be zero or 9.";
			this->error_line_number = line_number;
			return false;
		}
		line_number++;
	}
	
	if(number_of_valid_characters != total_number_of_numbers_in_sudoku)
	{
		this->error_message = "The File format is incorrect. Check the number of valid characters(. [0-9]). It should be 9*9 = 81.";
		return false;
	}
	_initialize_priority_queue();
	return true;
}

void SudokuSolver::_print_matrix_all_solutions()
{
	My570ListElem *ielem_solution_list = this->solution_list->First();

	while(ielem_solution_list != NULL)
	{
		char **next_input_matrix = (char **)ielem_solution_list->Obj();
		cout<<"+---+---+---+"<<endl;
		for(int i=0;i<9;i++)
		{
			cout<<"|";
			for(int j=0;j<9;j++)
			{
				if(this->_sudoku_input_matrix[i][j] == '.')
					cout<<next_input_matrix[i][j];
				else
					cout<<".";

				if((j+1)%3 == 0)
					cout<<"|";
			}
			cout<<endl;
			if((i+1) % 3 == 0)
				cout<<"+---+---+---+"<<endl;
		}
		cout<<endl;

		ielem_solution_list = this->solution_list->Next(ielem_solution_list);
	}

	return;
}

My570List * get_linear_list_from_1_to9()
{
	My570List *linear_list = new My570List();

	for(int i=1;i<10;i++)
	{
		char *cur_int = new char;
		*cur_int = i + '0';
		linear_list->Append(cur_int);
	}
	return linear_list;
}

void SudokuSolver::_initialize_random_shauffle_input()
{
	if(this->random_shaffle_input == NULL)
	{
		this->random_shaffle_input = new char*[9];
		for(int i=0;i<9;i++)
			this->random_shaffle_input[i] = new char[9];
		
		for(int i=0;i<9;i++)
			for(int j=0;j<9;j++)
				this->random_shaffle_input[i][j] = '0';
	}
	return;
}

char* get_char_indexed_at(My570List *cur_linear_list, int index)
{
	char* return_char;

	int i=0;
	My570ListElem *ielem = cur_linear_list->First();
	while(ielem != NULL)
	{
		if(i == index)
		{
			return_char = (char *)ielem->Obj();
			break;
		}
		i++;
		ielem = cur_linear_list->Next(ielem);
	}
	return return_char;
}

void SudokuSolver::_fill_priority_queues_using_randomshuffle()
{
	for(int i=0;i<9;i++)
		for(int j=0;j<9;j++)
		{
			char *cur_char = new char;
			*cur_char = this->random_shaffle_input[i][j];
			this->priority_queue[i]->Append(cur_char);
		}
	return;
}

My570ListElem* SudokuSolver::_get_element_to_be_covered(int index)
{
	int i=0;
	My570ListElem *ielem = this->solution_nodes_list->First();
	while(i<index)
	{
		ielem = this->solution_nodes_list->Next(ielem);
		i++;
	}
	return ielem;
}

bool SudokuSolver::_generate_puzzle(istream &ist)
{
	int number_of_bytes_read = 0;
	My570List *cur_linear_list = get_linear_list_from_1_to9();
	//My570List *random_shuffle_list = new My570List();
	if(this->random_shaffle_input == NULL)
		_initialize_random_shauffle_input();
	int cur_index = 0;
	bool is_full_generated = false;
	int number_of_un_covered_cells = 81;
	char input_matrix_final[9][9];
	while(ist.eof() != true)
	{
		char* char_buffer = new char;
		uint32_t *random_number = new uint32_t;
		*random_number = 0;
		
		if(cur_linear_list->Length() == 0)
		{
			delete cur_linear_list;
			cur_linear_list = get_linear_list_from_1_to9();
		}

		for(int i=0;i<4;i++)
		{
			//ist.get(char_buffer);
			ist.read(char_buffer, 1);
			if(ist.eof() == true)
			{
				cout<<"Number of bytes read = "<<number_of_bytes_read + i;
				this->error_message = "There are not enough bytes for random number generation.";
				return false;
			}

			*random_number = *random_number << 8;
			*random_number |= (unsigned char)(*char_buffer);
		}
		delete char_buffer;

		number_of_bytes_read += 4;
		
		if(cur_index < 81)
		{
			int random_index_for_number_to_be_removed = *random_number % cur_linear_list->Length();
			My570ListElem *cur_char_list_elem = cur_linear_list->Find(get_char_indexed_at(cur_linear_list,random_index_for_number_to_be_removed));

			this->random_shaffle_input[cur_index/9][cur_index%9] = *((char *)cur_char_list_elem->Obj());
			delete (char *)cur_char_list_elem->Obj();
			cur_linear_list->Unlink(cur_char_list_elem);
			cur_index++;
			delete random_number;

			
		}
		else
		{
			if(is_full_generated == false)
			{
				this->_print_matrix(this->random_shaffle_input,false,false);
				//break;

				_fill_priority_queues_using_randomshuffle();
				char input_matrix[9][9];
				for(int i=0;i<9;i++)
					for(int j=0; j<9; j++)
						input_matrix[i][j] = '.';
				
				_simple_solve_Recursive(input_matrix, this->priority_queue);

				this->_print_matrix((char **)this->solution_list->First()->Obj(),true,false);
				is_full_generated = true;
				
				for(int i=0;i<9;i++)
					for(int j=0; j<9; j++)
						input_matrix_final[i][j] = '.';
				copy_arrays(this->_sudoku_input_matrix, (char **)this->solution_list->First()->Obj());

				if(this->solution_nodes_list == NULL)
				{
					this->solution_nodes_list = new My570List();
					for(int i=0;i<9;i++)
						for(int j=0; j<9; j++)
						{
							PuzzleNode *cur_node = new PuzzleNode();
							cur_node->row_index = i;
							cur_node->col_index = j;
							cur_node->value = this->_sudoku_input_matrix[i][j];

							this->solution_nodes_list->Append(cur_node);
						}
				}
			}
			
			int index_of_element_to_be_covered = *random_number % number_of_un_covered_cells;
			My570ListElem *elem_to_be_covered = _get_element_to_be_covered(index_of_element_to_be_covered);

			int row_index_of_element_to_be_covered = ((PuzzleNode *)elem_to_be_covered->Obj())->row_index;
			int col_index_of_element_to_be_covered = ((PuzzleNode *)elem_to_be_covered->Obj())->col_index;
			char value_of_element_to_be_covered = ((PuzzleNode *)elem_to_be_covered->Obj())->value;
			this->solution_nodes_list->Unlink(elem_to_be_covered);

			this->_sudoku_input_matrix[row_index_of_element_to_be_covered][col_index_of_element_to_be_covered] = '.';
			deallocate_priority_queue(this->priority_queue);
			
			this->_allocate_priority_queue();
			this->_initialize_priority_queue();
			
			copy_arrays(input_matrix_final, this->_sudoku_input_matrix);
			this->_delete_solution_list();
			this->solution_list = new My570List();

			this->_count_solve_Recursive(input_matrix_final, this->priority_queue);
			
			if(this->solution_list->Length() > 1)
			{
				cout<<"Unassigning row "<<row_index_of_element_to_be_covered+ 1<<" column "<<col_index_of_element_to_be_covered + 1<<". "<<this->solution_list->Length()<<"solutions found."<<endl;

				this->_sudoku_input_matrix[row_index_of_element_to_be_covered][col_index_of_element_to_be_covered] = value_of_element_to_be_covered;
				cout<<"Row "<<row_index_of_element_to_be_covered+ 1<<" column "<<col_index_of_element_to_be_covered + 1<<" restored."<<endl;

				cout<<endl<<"Solution (after "<<81 - number_of_un_covered_cells<<"values unassigned):"<<endl;
				this->_delete_solution_list();
				this->solution_list = new My570List();
				this->_allocate_priority_queue();
				this->_initialize_priority_queue();
				char input_matrix[9][9];
				copy_arrays(input_matrix, this->_sudoku_input_matrix);

				this->_simple_solve_Recursive(input_matrix,this->priority_queue);
				this->_print_matrix(this->_sudoku_input_matrix,true,false);
				
				break;
			}
			else
			{
				cout<<"Unassigning row "<<row_index_of_element_to_be_covered+ 1<<" column "<<col_index_of_element_to_be_covered + 1<<"..."<<endl;
				number_of_un_covered_cells--;
			}
		}
		
	}
	return true;
}

bool SudokuSolver::Solve(istream &ist)
{
	bool is_solved_sucessfully = false;
	if(this->is_option_gen)
	{
		is_solved_sucessfully = _generate_puzzle(ist);
		return is_solved_sucessfully;
	}

	this->_Initialize(ist);
	char input_matrix[9][9];
	for(int i=0;i<9;i++)
		for(int j=0; j<9; j++)
			input_matrix[i][j] = this->_sudoku_input_matrix[i][j];
	

	if(this->is_option_solve == true)
	{
		is_solved_sucessfully = _simple_solve_Recursive(input_matrix, this->priority_queue);
		if(this->solution_list->Length() > 0)
		_print_matrix((char **)this->solution_list->First()->Obj());
		else
		{
			this->error_message = "There is no solution to this puzzle.";
			is_solved_sucessfully = false;
		}
	}
	
	else if(this->is_option_count == true)
	{
		this->_count_solve_Recursive(input_matrix, this->priority_queue);
		int number_of_solutions = this->solution_list->Length();
		if(number_of_solutions >= 1)
		{
			is_solved_sucessfully = true;
			this->_print_matrix_all_solutions();
			
			if(number_of_solutions == 1)
				cout<<"1 solution found";
			else if(number_of_solutions > 1)
				cout<<number_of_solutions<<"solutions found";
		}
		else
			is_solved_sucessfully = false;
		
	}
	
	
	if(is_solved_sucessfully == true)
		return true;
	else
		return false;
}
