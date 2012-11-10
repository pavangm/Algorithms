#include "stdafx.h"

#include "my570list.hpp"
#include<string>
#include <iostream>
#include "common.h"

using namespace std;

void print_usage()
{
	cerr<<"USAGE:"<<endl;
	cerr<<"./hw5 solve [file]"<<endl;
	cerr<<"./hw5 count [-printall] [-max=n] [file]"<<endl;
	cerr<<"./hw5 gen [-info] [rndfile]"<<endl;
	return;
}


char* get_option_name(char *option)
{
	int length = strlen(option);
	char *option_name = new char[strlen(option)];
	strncpy(option_name, (option + 1), strchr(option,'=') == NULL ? strlen(option + 1) : strchr(option,'=') - (option + 1));
	if(strchr(option,'=') == NULL)
		option_name[strlen(option + 1)] = '\0';
	else
		option_name[strchr(option,'=') - (option + 1)] = '\0';
	return option_name;
}

char * get_option_value(char * option)
{
	int length = strlen(option);
	char *option_value = new char[strlen(option)];
	strcpy(option_value, strchr(option,'=') + 1);
	return option_value;
}

