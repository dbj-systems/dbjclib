
#ifdef DO_NOT_AVOID 

#include "../dbj_string/dbj_front_back_string.h"
#include <stdio.h>
#include <assert.h>

    int dbj_front_back_string_test(FILE *fp_)
    {
// specimen starts from 1
// thus sub(5,7) is '567'
#define SPECIMEN "1234567890"

        dbj_string *sub = dbj_string_from("456", 1, 3);

        assert(
            /* "456" == "456" */
            dbj_string_compare(
                dbj_string_from("12456", 3, 5),
                dbj_string_from("45612", 1, 3)));

        dbj_string *o2z = dbj_string_make_view(SPECIMEN);
        // this yields DBJ_NPOS since "3" is different memory chunk
        assert(DBJ_NPOS == dbj_p_is_in_range("3", o2z));
        // this yields 2
        assert(2 == dbj_p_is_in_range(o2z->front + 2, o2z));
        // this yields 2
        assert(2 == dbj_c_is_in_range('3', o2z));
        /*
	if subrange is made, that means sub is
	found to be a substring  of the string
	by comparing the *contents*
	*/
        dbj_string *sub_range = dbj_to_subrange(o2z, sub);
        assert(sub_range);

        /*
	thus we can use it to remove the subrange
	*/
        dbj_string *rez = dbj_remove_substring(o2z, sub_range);

        dbj_string_free(sub_range);
        dbj_string_free(sub);
        dbj_string_free(rez);

		return 42;
    }

#endif // DO_NOT_AVOID 
