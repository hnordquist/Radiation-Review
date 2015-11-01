#ifndef CONCAT_HPP
#define CONCAT_HPP

#ifdef __cplusplus

#ifdef STD_CONCAT /* Standard C */

#define OMTYPE(name)\
    typedef name OM_FAR * P##name;\
    typedef const name OM_FAR * CP##name;\
    typedef name OM_FAR & R##name;\
    typedef const name OM_FAR & CR##name;
#define OMCLASS(name) \
    class OM_EXPORT name; \
    OMTYPE(name)
#define DB_INIT(DB) \
    PStoreDb OM_FAR DB##::DbPtr = 0;
#define STOREDIN(DB) \
    PStoreDb OMLOADDS _GetDb() const { return DB##::DbPtr; }

#else
#ifdef SYSV_CONCAT /* System V */

#define OMTYPE(name)\
    typedef name OM_FAR * P/**/name;\
    typedef const name OM_FAR * CP/**/name;\
    typedef name OM_FAR & R/**/name;\
    typedef const name OM_FAR & CR/**/name;
#define OMCLASS(name) \
    class OM_EXPORT name; \
    OMTYPE(name)
#define DB_INIT(DB) \
    PStoreDb OM_FAR DB/**/::DbPtr = 0;
#define STOREDIN(DB) \
    PStoreDb OMLOADDS _GetDb() const { return DB/**/::DbPtr; }

#else
#ifdef BSD_CONCAT /* BSD */

#define OMTYPE(name)\
    typedef name OM_FAR * P\
name; \
    typedef const name OM_FAR * CP\
name; \
    typedef name OM_FAR & R\
name; \
    typedef const name OM_FAR & CR\
name;
#define OMCLASS(name) \
    class OM_EXPORT name; \
    OMTYPE(name)
#define DB_INIT(DB) \
    PStoreDb OM_FAR DB\
::DbPtr = 0;
#define STOREDIN(DB) \
    PStoreDb OMLOADDS _GetDb() const { return DB\
::DbPtr; }

#else
#error you have an unknown system type

#endif /* BSD_CONCAT */
#endif /* SYSV_CONCAT */
#endif /* STD_CONCAT */

#else
#define OMTYPE(name)  /* */
#define OMCLASS(name) class OM_EXPORT name;
#endif /* __cplusplus */

#endif
