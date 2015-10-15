#ifndef __RWBINTREE_H__
#define __RWBINTREE_H__

/*
 * Binary tree of pointers to RWCollectable objects
 *
 * $Id: bintree.h@#/main/20  08/19/97 16:15:20  kevinj (TOOLS707_WIN32_971021)
 *
 * (c) Copyright 1989-1997 Rogue Wave Software, Inc.
 * ALL RIGHTS RESERVED
 *
 * The software and information contained herein are proprietary to, and
 * comprise valuable trade secrets of, Rogue Wave Software, Inc., which
 * intends to preserve as trade secrets such software and information.
 * This software is furnished pursuant to a written license agreement and
 * may be used, copied, transmitted, and stored only in accordance with
 * the terms of such license and with the inclusion of the above copyright
 * notice.  This software and information or any other copies thereof may
 * not be provided or otherwise made available to any other person.
 *
 * Notwithstanding any other lease or license that may pertain to, or
 * accompany the delivery of, this computer software and information, the
 * rights of the Government regarding its use, reproduction and disclosure
 * are as set forth in Section 52.227-19 of the FARS Computer
 * Software-Restricted Rights clause.
 * 
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions as set forth in subparagraph (c)(1)(ii) of the Rights in
 * Technical Data and Computer Software clause at DFARS 252.227-7013.
 * Contractor/Manufacturer is Rogue Wave Software, Inc.,
 * P.O. Box 2328, Corvallis, Oregon 97339.
 *
 * This computer software and information is distributed with "restricted
 * rights."  Use, duplication or disclosure is subject to restrictions as
 * set forth in NASA FAR SUP 18-52.227-79 (April 1985) "Commercial
 * Computer Software-Restricted Rights (April 1985)."  If the Clause at
 * 18-52.227-74 "Rights in Data General" is specified in the contract,
 * then the "Alternate III" clause applies.
 *
 ***************************************************************************
 *
 *
 */

#include "rw/colclass.h"
#include "rw/iterator.h"
#include "rw/mempool.h"
#include "rw/gqueue.h"
#include "rw/gstack.h"
STARTWRAP
#include <stddef.h>
ENDWRAP

// Forward declarations:
class RWExport RWTreeNode;
class RWExport RWGQueue(RWTreeNode);
//////////////////////////////////////////////////////////////////////
// the following declaration is deprecated along with some protected
// member functions that will go away in a subsequent release.
class RWExport RWGQueue(RWCollectable);
//////////////////////////////////////////////////////////////////////
declare(RWGStack,RWTreeNode)

/****************************************************************
 *                                                              *
 *                      RWBinaryTree                            *
 *                                                              *
 ****************************************************************/

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

class RWExport RWBinaryTree : public RWCollection {
friend class RWExport RWBinaryTreeIterator;

  RWDECLARE_COLLECTABLE(RWBinaryTree)

public:

  RWBinaryTree();
  RWBinaryTree(const RWBinaryTree&);
  virtual ~RWBinaryTree();

  void                          operator=(const RWBinaryTree&);

  // Subset of bt
#ifndef RW_GLBL_SCP_BUG2
  RWBoolean         operator<=(const RWBinaryTree&) const;
#else
// We must define an operator<=() that won't be overriden by ::operator<=()
  RWBoolean         rwBinaryTree_operator_le(const RWBinaryTree& bt) const;
#endif

  RWBoolean                     operator==(const RWBinaryTree& bt) const;

  // Special member function to balance a binary tree:
  void                          balance();
  // nodes from root to far leaf. 1-item tree has height 1;
  size_t                        height() const;

/********  Standard Member Functions for Collection Classes ********/  
  virtual void                  apply(RWapplyCollectable, void*);
  virtual void                  clear();
//virtual void                  clearAndDestroy();
//virtual RWBoolean             contains(const RWCollectable*) const;
  virtual size_t                entries() const;                        // Total entries
  virtual RWCollectable*        find(const RWCollectable*) const;               // First occurrence
  virtual RWCollectable*        insert(RWCollectable*);
  virtual RWBoolean             isEmpty() const {return root==rwnil;}
  virtual RWBoolean             isEqual(const RWCollectable*) const;
  virtual size_t                occurrencesOf(const RWCollectable*) const;
  virtual RWCollectable*        remove(const RWCollectable*);           // Remove first occurrence
//virtual void                  removeAndDestroy(const RWCollectable*); 
  virtual void                  saveGuts(RWvostream&) const;
  virtual void                  saveGuts(RWFile&) const;

protected:
  void                          applyChildren(const RWTreeNode*, RWapplyCollectable, void*);
  RWTreeNode*                   balanceChildren(size_t, RWGQueue(RWTreeNode)&);
  void                          deleteChildren(RWTreeNode*);
  RWCollectable*                deleteNode(RWTreeNode* victim, RWTreeNode* parent);
  void                          insertChildrenOf(const RWTreeNode*);
  void                          insertNode(RWTreeNode*, const RWTreeNode*);
  void                          levelApply(RWapplyCollectable, void*);
  size_t                        nodeHeight(RWTreeNode*) const;
  void                          queueNodes(RWTreeNode*,
                                           RWGQueue(RWTreeNode)& uniq,
                                           RWGQueue(RWTreeNode)& dupq
                                          );
//////////////////////////////////////////////////////////////////////
// The following functions are now unused by internal code, and 
// will go away in a subsequent release.
protected:
  RWTreeNode*       balanceChildren(size_t, RWGQueue(RWCollectable)&);
  void              balanceUnique();
  void              countChildren(const RWTreeNode*, size_t&) const;
//////////////////////////////////////////////////////////////////////
private:

#ifdef RDEBUG
  void  printChildren(const RWTreeNode*, ostream&, size_t&, char) const;
  friend ostream& operator<<(ostream&, const RWBinaryTree& bt);
#endif
  RWTreeNode*                   root;      // root = top-level item in tree
};    

/****************************************************************
 *                                                              *
 *              RWBinaryTreeIterator                            *
 *                                                              *
 ****************************************************************/

class RWExport RWBinaryTreeIterator : public RWIterator {
protected:
  const RWBinaryTree*           tree;           // Binary tree for this iterator.
  const RWTreeNode*             here;           // Current node.
  RWGStack(RWTreeNode)          stack;
private:
  void                          descendLeft();
public:
  RWBinaryTreeIterator(const RWBinaryTree&);
  virtual ~RWBinaryTreeIterator();

/*********** Virtual functions inherited from class RWIterator ***********/
  virtual RWCollectable*        findNext(const RWCollectable*);         // Find next matching item
  virtual RWCollectable*        key() const;                            // Return current value
  virtual RWCollectable*        operator()();                           // Advance iterator
  virtual void                  reset();
  
  RW_TYPESPEC  /* This macro usually expands to nothing */
};


/****************************************************************
 *                                                              *
 *                      RWTreeNode                              *
 *                                                              *
 ****************************************************************/

// Node in binary tree:
class RWExport RWTreeNode   RWMemoryPool_OPTION {
private:
friend class RWExport RWBinaryTree;
friend class RWExport RWBinaryTreeIterator;
  RWTreeNode*           right;  // Pointer to right node.
  RWTreeNode*           left;   // Pointer to left node.
  RWCollectable*        e;      // Pointer to RWCollectable object.
private:
  // Private constructor:
  RWTreeNode(RWCollectable* a, RWTreeNode* p=rwnil, RWTreeNode* n=rwnil) 
    { e = a; left = p; right = n; }
  
  RW_TYPESPEC  /* This macro usually expands to nothing */
};


//Special fix for Borland 5.0, this is related to bug #2437.  Borland 5.0
//will not choose class scope over global scope ever, but will recognize
//this as a function template specialization.

#if !defined(RW_NO_STL) && defined(RW_GLOBAL_BEFORE_CLASS_SCOPE)
inline bool operator<=
(const RWBinaryTree& wasThis, const RWBinaryTree& btr)
{
#  ifndef RW_GLBL_SCP_BUG2
    return wasThis.operator<=(btr);
#  else
    return wasThis.rwBinaryTree_operator_le(btr);
#  endif  /* RW_GLBL_SCP_BUG2 */
}
#endif

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif /* __RWBINTREE_H__ */

