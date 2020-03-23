#ifndef VALSTAT_INTEROP_H_
#define VALSTAT_INTEROP_H_

#ifndef dbj_clib_STRINGIFY
#define dbj_clib_STRINGIFY(  x )  dbj_clib_STRINGIFY_( x )
#define dbj_clib_STRINGIFY_( x )  #x
#endif // dbj_clib_STRINGIFY

/*
--------------------------------------------------------------------------------------------
The strong type macro
*/
#ifdef STRONG
#error STRONG already defined?
#else
#define dbj_clib_STRONG(N,T) typedef struct N final { T v; } N
#define STRONG dbj_clib_STRONG
#endif

/*
--------------------------------------------------------------------------------------------
The whole of valstat C interop is in the two macros bellow
*/
#ifdef valstat
#error name 'valstat' already found, please rename in here or at your location if possible
#endif

#ifdef valstat_type
#error name 'valstat_type' already found, please rename in here or at your location if possible
#endif

#define valstat__( T ) typedef struct valstat_##T \
{ T * val ; const char * stat  ; } valstat_##T

#define valstat( T ) valstat__( T )

/* 
use this to declare (not define) valstat  type name
*/
#define valstat_type( T ) valstat_##T 

/*
they will become a custom very qucikly but please do know the valstat type names created

examples:

valstat(int); -- creates --> typedef struct valstat_int { int * val, const char * stat } valstat_int;
typename created: valstat_int

valstat(uint64_t); -- creates --> typedef struct valstat_uint64_t { int * val, const char * stat } valstat_uint64_t;
typename created: valstat_uint64_t

--------------------------------------------------------------------------------------------
following macros are not necessary for valstat concept to be used fully
but we do use them bravely
*/

#ifdef valstat_status
#error name 'valstat_status' already found, please rename in here or at your location if possible
#endif
/* 
we like to use json formated strings as valstat status
here is the simplest possible form
 */
#define valstat_status(M) \
"{ \"file\": \"" __FILE__  "\", " \
 " \"line\": \""  dbj_clib_STRINGIFY(__LINE__) "\", "  "\"message\": \"" M "\", \"timestamp\": \"" __TIMESTAMP__  "\" }" 

/* 
---------------------------------------------------------------------------------------
use the following macros to create valstat instance for four possible states of valstat
*/
#ifdef valstat_info
#error name 'valstat_info' already found, please rename in here or at your location if possible
#endif
#ifdef valstat_error
#error name 'valstat_error' already found, please rename in here or at your location if possible
#endif
#ifdef valstat_ok
#error name 'valstat_ok' already found, please rename in here or at your location if possible
#endif
#ifdef valstat_empty
#error name 'valstat_empty' already found, please rename in here or at your location if possible
#endif
/*
type_ has to be a value type
value_ has to be addressable entity
message_ has to be a string literal
 */
#define valstat_info(type_, value_, message_) (type_) { & value_, valstat_status(message_) }
#define valstat_error(type_, message_) (type_) { NULL, valstat_status(message_) }
#define valstat_ok(type_, value_) (type_) { & value_, NULL }
#define valstat_empty(type_) (type_) { MULL, NULL }
/* 
use the following macros to check valstat instance for four possible states of valstat
 */
#ifdef is_valstat_info
#error name 'is_valstat_info' already found, please rename in here or at your location if possible
#endif
#ifdef is_valstat_error
#error name 'is_valstat_error' already found, please rename in here or at your location if possible
#endif
#ifdef is_valstat_ok
#error name 'is_valstat_ok' already found, please rename in here or at your location if possible
#endif
#ifdef is_valstat_empty
#error name 'is_valstat_empty' already found, please rename in here or at your location if possible
#endif

#define is_valstat_info(vstat_) ( (vstat_.val) && (vstat_.stat)  ) 
#define is_valstat_error(vstat_) ( (! vstat_.val) && (vstat_.stat)  ) 
#define is_valstat_ok(vstat_) ( (vstat_.val) && ( ! vstat_.stat)  ) 
#define is_valstat_empty(vstat_) ( (! vstat_.val) && ( ! vstat_.stat)  ) 

/* the full example in a comment; now:

valstat(uint64_t); -- creates --> typedef struct valstat_uint64_t { int * val, const char * stat } valstat_uint64_t;

typename created: valstat_uint64_t
valstat used is: valstat_uint64_t

valstat_type(uint64_t)
  divider (uint64_t	dividend, uint64_t	divisor)
{
	if (0 == divisor)
		return	valstat_error(valstat_uint64_t, "invalid_argument: zero divisor") ;

	if ((dividend) >= UINT16_MAX)
		return 	valstat_error(valstat_uint64_t,"dividend too large");

	if ((divisor) >= UINT16_MAX)
		return 	valstat_error(valstat_uint64_t,"divisor too large");

	// valstat value is a pointer 
    // quick and dirty solution
	static uint64_t rezult_anchor = (dividend / divisor);

	return valstat_ok( valstat_uint64_t, & rezult_anchor );
}

consuming site: ---------------------------------------------------

valstat_uint64_t rez = divider( d, r );

     if ( is_valstat_ok( rez ) ) 
         printf("rezult is: %d\n", rez.val );
     else          
         printf("error status: %d\n", rez.stat );

prety usable solution to: the "Semi Predicate Problem"  (https://en.wikipedia.org/wiki/Semipredicate_problem#References)

*/

#endif // VALSTAT_INTEROP_H_