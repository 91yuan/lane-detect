/* main1.cc */
/* we try to use gengetopt generated file in a C++ program */
/* we don't use autoconf and automake vars */
//for test
#include <iostream>
#include "stdlib.h"

#include "cmdline.h"
using namespace std;

int main (int argc, char **argv)
{
  gengetopt_args_info args_info;

  /* let's call our cmdline parser */
 if (cmdline_parser (argc, argv, &args_info) != 0)
    exit(1) ;

/*
 for ( unsigned i = 0 ; i < args_info.inputs_num ; ++i )
    cout << "file: " << args_info.inputs[i] << endl ;
*/
  if ( args_info.input_opt_given )
    cout << "yout file name is " << 
    args_info.input_opt_arg << "." << endl;

  if (args_info.type_opt_given)
    cout <<"The type is " << ( args_info.type_opt_flag ? "on" : "off" ) << endl ;
   
  return 0;
}
