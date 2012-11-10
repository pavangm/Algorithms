// homework5.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<iostream>
#include<fstream>
#include "common.h"
#include "SudokuSolver.h"

using namespace std;

int main(int argc, char** argv)
{
	
	if(argc <2)
	{
		cerr<<"Too few arguments passed."<<endl;
		print_usage();
		return 1;
	}

	char *file_path = NULL;//file_path can be random file for gen option.
	bool should_print_all = false;
	int max_number_of_solution_to_be_searched = -1;
	bool file_path_scanned = false;
	bool print_priority_queues = false;
	bool is_option_solve = false;
	bool is_option_count = false;
	bool is_option_gen = false;

	if(strcmp(argv[1], "solve") == 0)
	{
		is_option_solve = true;
		if(argc > 3)
		{
			cerr<<"Too many arguments."<<endl;
			print_usage();
			return 1;
		}
		else if(argc == 3)
		{
			file_path = new char[strlen(argv[2])+1];
			strcpy(file_path, argv[2]);
		}
	}
	else if(strcmp(argv[1], "count") == 0)
	{
		is_option_count = true;
		if(argc > 5)
		{
			cerr<<"Too many arguments."<<endl;
			print_usage();
			return 1;
		}

		for(int i=2; i<argc;i++)
		{
			if(strcmp(argv[i], "-printall") == 0)
				should_print_all = true;
			else if(strcmp(get_option_name(argv[i]) , "max") == 0)
			{
				char *option_value = get_option_value(argv[i]);
				for(unsigned int i=0;i<strlen(option_value);i++)
					if(!(option_value[i] - '0' >=0 && option_value[i] - '0' <=9))
					{
						cerr<<"Invalid Arguments passed.";
						print_usage();
						return 1;
					}

				sscanf(option_value,"%d",&max_number_of_solution_to_be_searched);
			}
			else
			{
				if(file_path_scanned != true)
				{
					file_path = new char[strlen(argv[i]) + 1];
					strcpy(file_path, argv[i]);
					file_path_scanned = true;
				}
				else
				{
					cerr<<"Invalid arguments passed."<<endl;
					print_usage();
					return 1;
				}
			}
		}
	}//end count
	else if(strcmp(argv[1], "gen") == 0)
	{
		is_option_gen = true;
		if(argc > 4)
		{
			cerr<<"Too many arguments passed."<<endl;
			print_usage();
			return 1;
		}

		for(int i=2;i<argc;i++)
		{
			if(strcmp(argv[i], "-info") == 0)
				print_priority_queues = true;
			else
			{
				if(file_path_scanned != true)
				{
					file_path = new char[strlen(argv[i]) + 1];
					strcpy(file_path, argv[i]);
					file_path_scanned = true;
				}
				else
				{
					cerr<<"Invalid arguments passed."<<endl;
					print_usage();
					return 1;
				}
			}
		}
	}
	else
	{
		cerr<<"Invalid arguments passed."<<endl;
		print_usage();
		return 1;
	}
	//end argc>2

	SudokuSolver *sudoku_sol = new SudokuSolver();
	sudoku_sol->is_option_count = is_option_count;
	sudoku_sol->is_option_gen = is_option_gen;
	sudoku_sol->is_option_solve = is_option_solve;
	sudoku_sol->max_number_of_solution_to_be_searched = max_number_of_solution_to_be_searched;
	sudoku_sol->print_priority_queues = print_priority_queues;
	sudoku_sol->should_print_all = should_print_all;
	bool is_sudoku_solved_sucessfully = false; 

	if(file_path != NULL)
	{
		ifstream sudoku_sol_ifstream;
		sudoku_sol_ifstream.open((const char *)file_path, std::ios::binary);
		if(sudoku_sol_ifstream.is_open())
		{
			is_sudoku_solved_sucessfully = sudoku_sol->Solve(sudoku_sol_ifstream);
		}
		else
		{
			cerr<<"The file could not be opened. Check if file exists and appropriate permissions are given."<<endl;
			delete sudoku_sol;
			return 1;
		}
		sudoku_sol_ifstream.close();
	}
	else
	{
		is_sudoku_solved_sucessfully = sudoku_sol->Solve(cin);
	}

	if(is_sudoku_solved_sucessfully == false)
	{
		cerr<<"Error Occurred:"<<endl;
		if(sudoku_sol->error_line_number > 0)
			cerr<<"Error Line Number:"<<sudoku_sol->error_line_number;
		cerr<<"Error Message:"<<sudoku_sol->error_message<<endl;
		return 1;
	}
	
	delete sudoku_sol;

	return 0;
}
