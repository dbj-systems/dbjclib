#ifndef _DBJ_STRING_INC_
#define _DBJ_STRING_INC_

/*
Destination size can be less than strlen of destination ...
source_ must be zero limited
*/
char* dbj_strcpy_s(char* destination_, size_t destination_size, char const* source_);

/*
strdup and strndup are defined in POSIX compliant systems as :

char *strdup(const char *str);
char *strndup(const char *str, size_t len);
*/
char* dbj_strdup(const char* s);

#ifndef strdup
#define strdup dbj_strdup
#endif // !strdup


/*
The strndup() function copies at most len characters from the string str
always null terminating the copied string.
*/
char* dbj_strndup(const char* s, size_t n);

#ifndef strndup
#define strdup dbj_strndup
#endif // !strndup

// remove chars given in the string arg
// return the new string
// user has to free() eventually
// both arguments must be zero limited
char* dbj_str_remove(const char* str_, const char* chars_to_remove_);

#endif // !_DBJ_STRING_INC_
