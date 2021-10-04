#pragma once
/*
(c) 2019 by dbj@dbj.prg -- Licence: CC BY SA 4.0

manage the 'char **' structure
it is said to be "self contained" because of the sentinel
at the end, it does not need a structure above it, trick,
to keep it's size for example, which is instead computed
whenever needed.

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

#include "../dbjclib.h"

DBJ_CLIB_BEGIN

typedef char* dbj_string_list_value_type;

typedef dbj_string_list_value_type* dbj_string_list_value_type_ptr;

/* last slot is reserved for sentinel */
static uint16_t dbj_string_list_max_capacity = UINT16_MAX - 1;
/*
returns the head of an new empty list

head: char  ** --------->[ char * ] ----------> SENTINEL
*/
dbj_string_list_value_type_ptr dbj_string_list_new();

/**
string to be appended is duplicated
return the head of the list
*/
dbj_string_list_value_type_ptr dbj_string_list_append(dbj_string_list_value_type_ptr, const dbj_string_list_value_type);

/**
minus sentinel
*/
uint16_t dbj_string_list_size(dbj_string_list_value_type_ptr);

/**
get by index,

first  argument is index
second argument is list head
third  argument is list size -- if 0, size will be computed on each call
so, if calling this repeatdely first call  dbj_string_list_size()
and use the size obtained

*/
dbj_string_list_value_type dbj_string_list_at_index(uint16_t, dbj_string_list_value_type_ptr, uint16_t);

/*
reset the list and return the head of the new empty list
*/
dbj_string_list_value_type_ptr dbj_string_list_reset(dbj_string_list_value_type_ptr);

/*
WARNING! Argument to this function is a list head. Afterwards it is invalid. It is NULL.
WARNING! This is of course valid for heap allocated strings only
		 which allways applies in here as we heap allocated inside append
*/
void dbj_string_list_free(dbj_string_list_value_type_ptr);


DBJ_CLIB_END
