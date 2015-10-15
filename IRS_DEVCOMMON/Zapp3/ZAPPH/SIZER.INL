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

inline zPoint& zReqGeom::at() {
	return _at;
}

inline void zReqGeom::at(const zPoint& p) {
	_at=p;
}

inline zDimension& zReqGeom::least() {
	return _least;
}

inline zDimension& zReqGeom::want() {
	return _want;
}

inline zDimension& zReqGeom::most() {
	return _most;
}

inline zGrav zReqGeom::affinity() const {
	return _affinity;
}

inline void zReqGeom::affinity(const zGrav g) {
	_affinity=g;
}

inline long& zReqGeom::userData() {
	return _userData;
}

inline void zReqGeom::userPtr(void* v) {
    _userPtr=v;
}

inline void* zReqGeom::userPtr() {
    return _userPtr;
}

inline zReqGeom& zReqGeom::operator=(zReqGeom& rg) { 
    _at=rg.at(); _least=rg.least(); _want=rg.want(); _most=rg.most(); 
	_affinity=rg.affinity(); 
	return *this;
}

inline zSizer::operator zRect() { 
	return zRect(_at,_size); 
}

inline zWindow* zSizer::ownedWindow() { 
	return (zWindow*)owned;
}

inline zSizer::operator zDimension() const { 
	return _size; 
}

inline zSizer::operator zPoint() const { 
	return _at; 
}

inline zRect zSizer::rect() { 
	return zRect(_at,_size); 
}

inline void zSizer::rect(const zRect&r) { 
	_at=r.topLeft(); 
	_size=r.size(); 
}

inline int zSizer::width() { 
	return _size.width(); 
}

inline int zSizer::height() { 
	return _size.height(); 
}

inline zPoint zSizer::pos() const { 
	return _at; 
}

inline zDimension zSizer::dimension() const { 
	return _size; 
} 

inline zSizer* zSizer::parent() { 
	return dad; 
}

inline void zSizer::parent(zSizer*s) { 
	dad=s; 
}

inline zGrav zSizer::affinity() const { 
	return _affinity; 
}

inline void zSizer::affinity(const zGrav g) { 
	_affinity=g; 
}

inline zSizerDlist* zSizer::kids() { 
	return _kids; 
}

inline int zSizer::numChildren() const { 
	return items; 
}


inline BOOL zSizer::isNull() {
  return (pos().x()==0 && pos().y()==0 && dimension().width()==0 &&
   dimension().height()==0) ||
   (pos().x()==CW_USEDEFAULT && pos().y()==CW_USEDEFAULT &&
   dimension().width()==CW_USEDEFAULT && dimension().height()==CW_USEDEFAULT);
}


inline zLayout* zSizer::layout() {
	return _layout; 
}

inline zLayout* zSizer::layout(zLayout *newLayout) { 
	zLayout *oldLayout = _layout;
	_layout = newLayout; 
	return oldLayout;
}

inline void *zSizer::asType(int type) {
  return (type == zSIZER) ? (void *)this : (void *)0;
}

inline void *zLeafSizer::asType(int type) {
  return (type == zSIZER_LEAF) ? (void *)this : zSizer::asType(type);
}

inline zReqGeom& zFillerSizer::space() { 
	return mySpace; 
}
	
inline void *zFillerSizer::asType(int type) {
  return (type == zSIZER_FILLER) ? (void *)this : zLeafSizer::asType(type);
}

inline void zFixedSizer::want(const zDimension d) { 
	_want=d; 
}

inline zDimension zFixedSizer::want() const { 
	return _want; 
}


inline void *zGravSizer::asType(int type) {
  return (type == zSIZER_GRAV) ? (void *)this : zSizer::asType(type);
}

inline void *zFixedSizer::asType(int type) {
  return (type == zSIZER_FIXED) ? (void *)this : zSizer::asType(type);
}

inline zGrav zGravSizer::gravity() const { 
	return _affinity; 
}

inline void zGravSizer::gravity(const zGrav g) { 
	_affinity=g; 
}

inline zDimension zGravSizer::useArea() const { 
	return _want; 
}

inline void zGravSizer::useArea(const zDimension d) { 
	_want=d; 
}

inline float zPercentSizer::vertical() const { 
	return vPer; 
}

inline void zPercentSizer::vertical(const float i) { 
	vPer=i; 
}

inline float zPercentSizer::horizontal() const { 
	return hPer; 
}

inline void zPercentSizer::horizontal(const float i) { 
	hPer=i; 
}

inline void *zPercentSizer::asType(int type) {
	return (type == zSIZER_PERCENT) ? (void *)this : zGravSizer::asType(type);
}

inline void *zGrowToFitSizer::asType(int type) {
	return (type == zSIZER_GROW) ? (void *)this : zGravSizer::asType(type);
}


inline unsigned int zExpandSizer::flags() const {
	return flags_;
}

inline void zExpandSizer::flags(unsigned int newFlags) {
	flags_ = newFlags;
	dirty_ = TRUE;
}

inline void *zExpandSizer::asType(int type) {
	return (type == zSIZER_EXPAND) ? (void *)this : zSizer::asType(type);
}

inline zLayout* zLayoutSizer::layout() { 
	return _layout; 
}

inline void zLayoutSizer::layout(zLayout *l) { 
    _layout=l; 
}

inline void *zLayoutSizer::asType(int type) {
  return (type == zSIZER_LAYOUT) ? (void *)this : zGravSizer::asType(type);
}

inline void *zSizeWithParent::asType(int type) {
  return (type == zSIZER_WITH_PARENT) ? (void *)this : zSizer::asType(type);
}






