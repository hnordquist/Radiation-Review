/*
This work was supported by the United States Member State Support Program to IAEA Safeguards;
the U.S. Department of Energy, Office of Nonproliferation and National Security, International
Safeguards Division; and the U.S. Department of Energy, Office of Safeguards and Security.

LA-CC-14-040. This software was exported from the United States in accordance with the Export
Administration Regulations. Diversion contrary to U.S. law prohibited.

Copyright 2015, Los Alamos National Security, LLC. This software application and associated
material ("The Software") was prepared by the Los Alamos National Security, LLC. (LANS), under
Contract DE-AC52-06NA25396 with the U.S. Department of Energy (DOE). All rights in the software
application and associated material are reserved by DOE on behalf of the Government and LANS
pursuant to the contract.

Redistribution and use in source and binary forms, with or without modification, are permitted
provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of
conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of
conditions and the following disclaimer in the documentation and/or other materials provided
with the distribution.
3. Neither the name of the "Los Alamos National Security, LLC." nor the names of its contributors
may be used to endorse or promote products derived from this software without specific prior
written permission.

THIS SOFTWARE IS PROVIDED BY LOS ALAMOS NATIONAL SECURITY, LLC AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL LOS ALAMOS
NATIONAL SECURITY, LLC OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRAT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
THE POSSIBILITY OF SUCH DAMAGE.
*/
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
