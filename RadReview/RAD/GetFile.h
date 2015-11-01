
/* GetFile.H  Function Prototypes */

#ifndef GetFile_H
	#define GetFile_H

	#define FILENAME_LEN 50
	int gen_get_filenames(
		char *filename_to_match,	/* input: filename specifier, can contain *.c */
		unsigned int   max_filenames_allowed, /* input: max filenames allowed */
		int   sort_order,		/* input: way names are to be sorted
						  0 - alphabetically
						  1 - according to ascending date
						  2 - according to descending date */
		char ptr_filenames[][FILENAME_LEN],	/* output: where sorted filenames are stored */
		unsigned int  *ptr_num_filenames);	/* output: how many matching files were found */

	void gen_count_filenames(
		char * filename_to_match,	/* input: filename specifier, can contain *.c */
		int * count_ptr);		/* output: pointer to number of files counted */


#endif
