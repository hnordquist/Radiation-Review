//getfile.cpp

#include <stdlib.h>			/* include for malloc prototype */
#include <string.h>
#include <search.h>
#include <io.h>
#include "getfile.h"
#include "gen.h"

//	12-Jul-2004	SFK	Modified to use longer filenames as defined by FILENAME_LEN
struct FILENAME {
    char filename[FILENAME_LEN];
    long filedate;
};

/*===========================================================================
 *
 *  Name	 :  name_compare
 *
 *  Purpose	 :  Qsort uses this routine to sort the names
 *
 *  Return value :  see c-function qsort
 *
 *  Special notes:
 *
 *  Implicit Input :
 *  Implicit Input/Output:
 *  Implicit Output:

 *  date    /	author	revision
 *  -----------------	--------
 *  03-Jul-1989 SFK	Created
 *	12-Jul-2004	SFK	Modified to use longer filenames as defined by FILENAME_LEN
 *
===========================================================================*/
static int name_compare(
    const void *ptr_file1, /* input - first name to sort */
    const void *ptr_file2) /* input - second name to sort */

{
    int result;

    result = strcmpi (((struct FILENAME *)ptr_file1)->filename,
				((struct FILENAME *)ptr_file2)->filename);
    return(result);
}

/*===========================================================================
 *
 *  Name	 :  date_compare_ascending
 *
 *  Purpose	 :  Qsort uses this routine to sort the dates in ascending order
 *
 *  Return value :  see c-function qsort
 *
 *  Special notes:
 *
 *  Implicit Input :
 *  Implicit Input/Output:
 *  Implicit Output:

 *  date    /	author	revision
 *  -----------------	--------
 *  03-Jul-1989 SFK	Created
 *	12-Jul-2004	SFK	Modified to use longer filenames as defined by FILENAME_LEN
 *
===========================================================================*/
static int date_compare_ascending(
    const void *ptr_file1, /* input - first date to sort */
    const void *ptr_file2) /* input - second date to sort */

{
    int result;

    result = 0;
    if (((struct FILENAME *)ptr_file1)->filedate >
				((struct FILENAME *)ptr_file2)->filedate) {
	result = 1;
    }
    else if (((struct FILENAME *)ptr_file1)->filedate <
				((struct FILENAME *)ptr_file2)->filedate) {
	result = -1;
    }

    return(result);
}






/*===========================================================================
 *
 *  Name	 :  date_compare_descending
 *
 *  Purpose	 :  Qsort uses this routine to sort the dates in descending order
 *
 *  Return value :  see c-function qsort
 *
 *  Special notes:
 *
 *  Implicit Input :
 *  Implicit Input/Output:
 *  Implicit Output:

 *  date    /	author	revision
 *  -----------------	--------
 *  03-Jul-1989 SFK	Created
 *	12-Jul-2004	SFK	Modified to use longer filenames as defined by FILENAME_LEN
 *
===========================================================================*/
static int date_compare_descending(
    const void *ptr_file1, /* input - first date to sort */
    const void *ptr_file2) /* input - second date to sort */

{
    int result;

    result = 0;
    if (((struct FILENAME *)ptr_file1)->filedate <
				((struct FILENAME *)ptr_file2)->filedate) {
	result = 1;
    }
    else if (((struct FILENAME *)ptr_file1)->filedate >
				((struct FILENAME *)ptr_file2)->filedate) {
	result = -1;
    }

    return(result);
}

/* <f> */
/*===========================================================================
 *
 *  Name	 :  gen_get_filenames
 *
 *  Purpose	 :  get filenames matching specified name and sort them
 *
 *  Return value : GEN_SUCCESS/GEN_HEAP_ERROR/GEN_NO_FILE_NAME_MATCH
 *
 *  Special notes:  The user must provide filename storage in a [x][FILENAME_LEN] array
 *		    where x is the number of filenames expected.
 *
 *  Implicit Input :
 *  Implicit Input/Output:
 *  Implicit Output:

 *  date    /	author	revision
 *  -----------------	--------
 *  03-Jul-1989 SFK	Created
 *  09-Sep-1990 WCH	Modified
 *  03-Oct-1990 WCH     Modified - changed return values
 *	12-Jul-2004	SFK	Modified to use longer filenames as defined by FILENAME_LEN,
 *					use strncpy to prevent any overflow if name longer than FILENAME_LEN
 *
===========================================================================*/
int gen_get_filenames(
    char *filename_to_match,	/* input: filename specifier, can contain *.c */
    unsigned int   max_filenames_allowed, /* input: max filenames allowed */
    int   sort_order,		/* input: way names are to be sorted
					  0 - alphabetically
					  1 - according to ascending date
					  2 - according to descending date */
    char ptr_filenames[][FILENAME_LEN],	/* output: where sorted filenames are stored */
    unsigned int  *ptr_num_filenames)	/* output: how many matching files were found */

{

    struct _finddata_t c_file;
    struct FILENAME *files;
    unsigned int i;
    long handle;

    i = 0;
    *ptr_num_filenames = 0;
       
   /* ------------------------------------------------------------------
    *	Get storage for file info
    * ----------------------------------------------------------------*/
    if (max_filenames_allowed == 0)
	return (GEN_NO_FILE_NAME_MATCH);
    files = (struct FILENAME *)calloc(max_filenames_allowed, sizeof(struct FILENAME));
    if (files == 0)
        return (GEN_HEAP_ERROR);

   /* ------------------------------------------------------------------
    *	Find first filenames matching specification
    * ----------------------------------------------------------------*/
    handle = _findfirst(filename_to_match,&c_file);
    if (handle == -1L)
    {
        free(files);
	return (GEN_NO_FILE_NAME_MATCH);
    }
    //strcpy(files[i].filename,c_file.name);
	strncpy(files[i].filename,c_file.name, FILENAME_LEN-1);
	files[i].filename[FILENAME_LEN] = '\0';
    files[i].filedate = c_file.time_write;
    i++;

   /* ------------------------------------------------------------------
    *	Find remaining filenames matching specification
    * ----------------------------------------------------------------*/
    while ((_findnext (handle, &c_file) != -1L) && (i < max_filenames_allowed)) {
	strcpy(files[i].filename,c_file.name);
	files[i].filedate = c_file.time_write;
	i++;
    }
    _findclose (handle);

    *ptr_num_filenames = i;


   /* ------------------------------------------------------------------
    *	Sort according to specified sort method. Default is to sort
    *	alphabetically.
    * ----------------------------------------------------------------*/

    switch (sort_order) {

	case GEN_SORT_ASC_DATE :
	qsort ((void *)files, *ptr_num_filenames, sizeof(struct FILENAME),
						date_compare_ascending);
	break;

	case GEN_SORT_DES_DATE :
	qsort ((void *)files, *ptr_num_filenames, sizeof(struct FILENAME),
						date_compare_descending);
	break;

	case GEN_SORT_ALPHA :
	default :
	qsort ((void *)files, *ptr_num_filenames, sizeof(struct FILENAME),
						name_compare);
	break;
    }

       
   /* ------------------------------------------------------------------
    *	Copy sorted filenames into user's array
    * ----------------------------------------------------------------*/
    for (i=0; i < *ptr_num_filenames; i++) {
	strcpy(ptr_filenames[i],files[i].filename);
    }
       
   /* ------------------------------------------------------------------
    *	Deallocate the storage
    * ----------------------------------------------------------------*/
    free(files);

    return (GEN_SUCCESS);

}



/* <f> */
/*===========================================================================
 *
 *  Name	 :  gen_count_filenames
 *
 *  Purpose	 :  count filenames matching specified name
 *
 *  Return value :
 *
 *  Special notes: 
 *
 *  Implicit Input :
 *  Implicit Input/Output:
 *  Implicit Output:

 *  date    /	author	revision
 *  -----------------	--------
 *  04-Apr-1990 WCH	Created
 *
===========================================================================*/

void gen_count_filenames(
char * filename_to_match,	/* input: filename specifier, can contain *.c */
int * count_ptr)		/* output: pointer to number of files counted */

{
    struct _finddata_t c_file;
    long handle;

    *count_ptr = 0;
    handle = _findfirst (filename_to_match, &c_file);
    if (handle == -1L)
	return;
    (*count_ptr)++;
    while ((_findnext (handle, &c_file)) != -1)
	(*count_ptr)++;
    _findclose (handle);
    return;
}
