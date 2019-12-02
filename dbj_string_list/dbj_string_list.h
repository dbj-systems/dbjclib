#pragma once
/*
(c) 2019 by dbj@dbj.prg -- Licence: GPLv3

manage the 'char **' structure
it is said to be "self contained" because of the sentinel
at the end, it does not need a structure above it,
to keep it's size for example.

 empty:
		  char  ** --------->[ char * ] ----------> SENTINEL

 populated:

 head     char  ** --------->[ char * ] ----------> "ABRA"
		             		 [ char * ] ----------> "KA"
							 [ char * ] ----------> "DABRA"
sentinel		   --------->[ char * ] ----------> SENTINEL  (size == 3)
							 [   0    ] 
							 [   0    ] 
							 [   0    ] 
								...
			max_size		 [   0    ] (reserved for SENTINEL)
		

  Maximum size of this structure is 0xFFFFui8 aka UINT16_MAX aka 65535
*/

#include <stdlib.h>
#include <stdint.h>


#ifdef __cplucplus
extern "C" {
#endif

	typedef char * dbj_string_list_value_type;

	typedef dbj_string_list_value_type * dbj_string_list_type;

	/* last slot is reserved for sentinel */
	static uint16_t dbj_string_list_max_capacity = UINT16_MAX - 1;
	/*
	returns the head of an new empty list

	head: char  ** --------->[ char * ] ----------> SENTINEL
	*/
	dbj_string_list_type dbj_string_list_new();

	/**
	string to be appended is duplicated
	return the head of the list
	*/
	dbj_string_list_type dbj_string_list_append(dbj_string_list_type, const dbj_string_list_value_type );

	/**
	minus sentinel
	*/
	    uint16_t dbj_string_list_size(dbj_string_list_type);
    
	/**
	get by index, 
	
	first  argument is index
	second argument is list head
	third  argument is list size -- if 0, size will be computed on each call

	if calling this repeatdely first call  dbj_string_list_size() and use the size obtained

	*/
	dbj_string_list_value_type dbj_string_list_at_index( uint16_t, dbj_string_list_type , uint16_t );

	/*
	reset the list and return the head of the new empty list
	*/
	dbj_string_list_type dbj_string_list_reset(dbj_string_list_type);

	/*
	WARNING! Argument to this funcitom is a list head. Afterwards it is invalid. It is NULL.
	*/
	void dbj_string_list_free(dbj_string_list_type);


#ifdef __cplucplus
}
#endif
