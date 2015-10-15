#if ! defined HDR_listsubr // {
#define HDR_listsubr

/* ======================================================================= */
/* ============================= listsubr.h ============================== */
/* ======================================================================= */

/*+/mod/TOC------------------------------------------------------------------
* listsubr.h - list-handling routines
*
* Purpose:
*		Provide list-handling capability:
*
*		For doubly-threaded lists:
*		o	append an item to the end of the list
*		o	remove an item from the list
*		o	insert an item before or after a specified item
*
* Notes:
*
* Date     Author		Revision
* -------- ------------	--------
* ??-??-?? R. Cole		created
*--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
* Copyright 1995-97, The Regents Of the University of California.  This
* program was prepared by the Regents of the University of California at
* Los Alamos National Laboratory (the University) under Contract No. W-7405-
* ENG-36 with the U.S. Department of Energy (DOE).  The University has
* certain rights in the program pursuant to the contract and the program
* should not be copied or distributed outside your organization.  All rights
* in the program are reserved by the DOE and the University.  Neither the
* U.S. Government nor the University makes any warranty express or implied,
* or assumes any liability or responsibility for the use of this software.
*
* This software was produced by the Safeguards Science and Technology
* Group (NIS-5).
*--------------------------------------------------------------------------*/

/*/+/TOC----------------------------------------------------------------------
*
*
* DLIST_XXX
*
* Purpose:
*		The DLIST_XXX macros handle doubly linked lists of structures.  The
*		structure is defined as desired, with the addition of two additional
*		structure members.
*
*		In the code fragments here, the structure name is ex_struct, which
*		is totally arbitrary.  I have adopted the convention of always having
*		the added members at the top of the structure; this is an arbitrary
*		choice.  The names of the added members, however, MUST be pPrev
*		and pNext.  These members must be declared as pointers to the
*		structure.  For illustrative purposes, the structure appears as:
*
*		struct ex_struct {
*			struct ex_struct *pPrev;		link to previous item
*			struct ex_struct *pNext;		link to next item
*			int		iFirst;
*			float	fSecond;
*		};
*
*		In addition to adding link members to the structure, it is required
*		to have two additional pointers for the head and tail of the list.
*		These pointers must have the same (or longer) "lifetime" as the
*		the list of structures will have.  These pointers must be initialized
*		to NULL prior to their first use.  Their names are arbitrary.  I have
*		adopted the convention of having names begin with something that
*		suggests the use for the structure and end with Head and Tail.  For
*		example:
*
*		struct ex_struct *pExHead=NULL;		pointer to head item on list
*		struct ex_struct *pExTail=NULL;		pointer to tail item on list
*
*		Now the list is ready to use.  Create the instances of the structure
*		in whatever your usual manner is.  For illustrative purposes, the
*		fragment below adds new instances at the end of the list.
*
*		struct ex_struct *pEx;
*
*		pEx = (struct ex_struct *)malloc(sizeof(struct ex_struct));
*		DLIST_APPEND(pEx, pExHead, pExTail);
*		pEx = (struct ex_struct *)malloc(sizeof(struct ex_struct));
*		DLIST_APPEND(pEx, pExHead, pExTail);
*		...		// create and append as many as needed
*
*		To traverse the list from the item at the head all the way to
*		the item at the tail (much like you'd subscript your way through
*		an array), the following is effective:
*
*		for (pEx=pExHead; pEx!=NULL; pEx=pEx->pNext) {
*			ProcessItem(pEx);
*		}
*
*		Adding and deleting items at arbitrary positions in the list looks
*		a little clumsy compared with a simple 'for' loop dealing with the
*		tradional array of structures.  The advantage of using a list is tha
*		you don't have to do intricate reallocating of memory, copying of
*		unchanged array items, adjusting pointers, etc.
*
*		To delete the nth item in the list:
*		int		i=0;
*		for (pEx=pExHead; pEx!=NULL; pEx=pEx->pNext) {
*			if (i == the_nth_item) {
*				DLIST_REMOVE(pEx, pExHead, pExTail);
*				free(pEx);
*				break;		// we've found it and got rid of it; leave loop
*			}
*			i++;
*		}
*
*		To add a new item after the nth item in the list:
*		pExNew = (struct ex_struct *)malloc(sizeof(struct ex_struct));
*		int		i=0;
*		for (pEx=pExHead; pEx!=NULL; pEx=pEx->pNext) {
*			if (i == the_nth_item) {
*				DLIST_INSERT(pExNew, pEx, pExHead, pExTail);
*				free(pEx);
*				break;		// we've found it and got rid of it; leave loop
*			}
*			i++;
*		}
*
*		It's easy to tell if the list is empty--if pExHead == NULL, there's
*		nothing there.  It's not so easy to tell how many items are in the
*		list.  You can traverse it and count.  Typically when I'm interested
*		in the number of items, I keep a counter that I increment with
*		DLIST_APPEND or DLIST_INSERT and decrement with DLIST_REMOVE.
*
* Synopsis:
*
* DLIST_APPEND(pItem, pListHead, pListTail)
*		append an item to the end of a doubly linked list
*
* DLIST_INSERT(pItem, pPlace, pListHead, pListTail)
*		insert an item following a specified item; if pPlace is NULL, the
*		item is inserted at the head of the list
*
* DLIST_REMOVE(pItem, pListHead, pListTail)
*		remove an item from a doubly linked list
*---------------------------------------------------------------------------*/
#define DLIST_APPEND(pItem,pHead,pTail) \
{\
    (pItem)->pNext = NULL;\
    (pItem)->pPrev = pTail;\
    if (pTail != NULL)\
        pTail->pNext = (pItem);         /* link previous tail to here */\
    pTail = (pItem);\
    if (pHead == NULL)\
        pHead = (pItem);                /* link to head if first item */\
}

#define DLIST_INSERT(pItem,pPlace,pHead,pTail) \
{\
    if (pPlace != NULL) {\
        (pItem)->pNext = pPlace->pNext;\
        (pItem)->pPrev = pPlace;\
        pPlace->pNext = (pItem);\
        if ((pItem)->pNext != NULL)\
        	(pItem)->pNext->pPrev = (pItem);\
        else\
                pTail = (pItem);        /* link previous tail to here */\
    }\
    else {\
        (pItem)->pNext = pHead;\
        (pItem)->pPrev = NULL;\
        if (pHead != NULL)\
        	pHead->pPrev = (pItem);\
        pHead = (pItem);\
        if (pTail == NULL)\
            pTail = (pItem);\
    }\
}

#define DLIST_REMOVE(pItem,pHead,pTail) \
{\
    if ((pItem)->pPrev != NULL)\
        ((pItem)->pPrev)->pNext = (pItem)->pNext;   /* link prev to next */\
    else\
        pHead = (pItem)->pNext;                   /* link list head to next */\
    if ((pItem)->pNext != NULL)\
        ((pItem)->pNext)->pPrev = (pItem)->pPrev;   /* link next to prev */\
    else\
        pTail = (pItem)->pPrev;           /* link list tail to prev */\
    (pItem)->pNext = NULL;\
    (pItem)->pPrev = NULL;\
}

#if defined __cplusplus
extern "C" {
#endif

#if defined MAKE_DLL && defined WIN32
#define EXPORT16
#define EXPORT32 __declspec(dllexport)
#elif defined MAKE_DLL
#define EXPORT16 __export
#define EXPORT32
#else
#define EXPORT16
#define EXPORT32
#endif

#undef EXPORT16
#undef EXPORT32

#if defined __cplusplus
}
#endif

#endif // ! defined HDR_listsubr }
