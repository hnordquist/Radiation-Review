/*
 * (c) Copyright 1991 - 1996 Rogue Wave Software, Inc.
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
 **************************************************************************/

inline zDListNode::zDListNode() {
}

inline zDListNode::zDListNode(const void *obj_) : _obj((void *)obj_) {
}

inline zDListNode* zDListNode::next() const {
	return _next;
}

inline zDListNode* zDListNode::prev() const {
	return _prev;
}

inline zDListNode* zDListNode::next(const zDListNode *n) {
	return _next= (zDListNode *)n;
}

inline zDListNode* zDListNode::prev(const zDListNode *p) {
	return _prev= (zDListNode *)p;
}

inline void zDListNode::append(zDListNode *newNode) {
	next()->insert(newNode);
}

inline void zDListNode::insert(const void *obj_) {
    insert(ZNEWH zDListNode(obj_));
}

inline void zDListNode::append(const void *obj_) {
	 append(ZNEWH zDListNode(obj_));
}

inline void* zDListNode::obj()	const {
	return _obj;
}

inline void zDListNode::detach() {
	prev()->next(next()); 
	next()->prev(prev());
}

inline void zDListNode::removeNext() {
	zDListNode *temp = next();
	temp->detach(); 
	delete temp;
}

inline void zDListNode::removePrev() { 
	zDListNode *temp = prev();
	temp->detach(); 
	delete temp;
}

inline zDblList::zDblList(const zDblList &) {
}

inline zDblList&  zDblList::operator=(const zDblList&) {
    return *this;
}

inline void zDblList::insert(zDListNode *newNode) {
	zDListNode::append(newNode);
	// append after header node (at list's top)
	_length++;
}

inline void zDblList::append(zDListNode *newNode) {
	zDListNode::insert(newNode);
	// ins. before header node (at list's end)
	_length++;
}

inline void zDblList::insert(const void *obj_) {
        insert(ZNEWH zDListNode(obj_));
}

inline void zDblList::append(const void *obj_) {
	append(ZNEWH zDListNode(obj_));
}

inline void* zDblList::first() { 
	return next()->obj(); 
}

inline void* zDblList::last()	{
	return prev()->obj();
}

inline zDblList::zDblList()	{
	_obj = 0;
	_next = _prev = this;
	_length=0;
}

inline int zDblList::isEmpty() {
	return next()==this;
}

inline void zDblList::removeFirst() {
	removeNext(); 
	_length--;
}

inline void zDblList::removeLast() {
	removePrev();
	_length--;
}

inline long zDblList::length() {
	return _length;
}

inline void zDListIter::insert(zDListNode *p) {
	_curNode->insert(p);
	_list->_length++;
}

inline void zDListIter::append(zDListNode *p) {
	_curNode->append(p);
	_list->_length++;
}

inline void zDListIter::insert(const void *p) {
	_curNode->insert(p);
	_list->_length++;
}

inline void zDListIter::append(const void *p) {
	_curNode->append(p);
	_list->_length++;
}
	
inline void* zDListIter::operator++(int) {
	// postfix
	void *temp = _curNode->obj();
	_curNode=_curNode->next();
	return temp;
}
	
inline void* zDListIter::operator--(int) {
	// postfix
	void *temp = _curNode->obj();
	_curNode=_curNode->prev();
	return temp;
}
	
inline void* zDListIter::operator++() {
	_curNode=_curNode->next();
	return _curNode->obj();
}

inline void* zDListIter::operator--() {
	_curNode=_curNode->prev();
	return _curNode->obj();
}
	
inline void* zDListIter::value() {
	return _curNode->obj();
}


inline zDListIter::zDListIter() {
	_direction = 0;
}

inline void zDListIter::reset() {
	_curNode = _list;
}

inline zDListIter::zDListIter(const zDblList *list_) {
	_list= (zDblList *) list_; 
	reset();
	_direction=0;
}

inline int zDListIter::isFirst() {
	return _curNode == _list->next();
}

inline int zDListIter::isLast()	{
	return _curNode == _list->prev();
}

inline int zDListIter::isNowhere()	{
	return _curNode == _list;
}

inline void zDListIter::setDirForward()	{
	_direction = 0;
}

inline void zDListIter::setDirReverse() {
	_direction = 1;
}

inline int zDListIter::isForward() {
	return _direction==0;
}

inline zDListNode* zDListIter::curNode() {
	return _curNode;
}

inline void* zDListIter::operator()() {
	return (isForward() ? ++(*this) : --(*this)) ;
	// works even on last element because listHeader->obj() returns 0;
}


