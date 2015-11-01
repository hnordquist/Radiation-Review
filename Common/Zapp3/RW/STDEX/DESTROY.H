#ifndef _STDEX_DESTROY_H
#define _STDEX_DESTROY_H

template <class T>
inline void rw_destroy(T* item) 
#if defined RW_EXPLICIT_SCOPE_DESTROY
                                 { item->T::~T(); }
#else
                                 { item->~T(); }
#endif

#endif /* _STDEX_DESTROY_H */
