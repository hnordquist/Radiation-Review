# dependencies for Raima Object Manager

autoload.o: \
	autoload.$(CPP_EXTENSION) \
	$(ROM_INC)/vpp.hpp \
	$(ROM_INC)/concat.hpp \
	$(ROM_INC)/defunx.hpp \
	$(ROM_INC)/verrout.hpp \
	$(ROM_INC)/storeobj.hpp\
	$(ROM_INC)/dbaddr.hpp\
	$(ROM_INC)/vistaobj.hpp\
	$(ROM_INC)/storedb.hpp\
	$(ROM_INC)/navmacro.hpp\
	$(ROM_INC)/keyobj.hpp\
	$(ROM_INC)/setobj.hpp\
	$(ROM_INC)/nav.hpp\
	$(ROM_INC)/dbrdx.hpp\
	$(ROM_INC)/storetsk.hpp\
	$(ROM_INC)/taskrdx.hpp\
	$(ROM_INC)/activate.hpp\
	$(ROM_INC)/impllock.h
	$(CPP) $(CPP_FLAGS) $<

activate.o: \
	activate.$(CPP_EXTENSION)\
	$(ROM_INC)/vpp.hpp\
	$(ROM_INC)/concat.hpp \
	$(ROM_INC)/defunx.hpp\
	$(ROM_INC)/verrout.hpp \
	$(ROM_INC)/activate.hpp
	$(CPP) $(CPP_FLAGS) $<

contain.o: \
	contain.$(CPP_EXTENSION)\
	$(ROM_INC)/vpp.hpp\
	$(ROM_INC)/concat.hpp \
	$(ROM_INC)/defunx.hpp\
	$(ROM_INC)/verrout.hpp \
	$(ROM_INC)/contain.hpp\
	$(ROM_INC)/storeobj.hpp\
	$(ROM_INC)/dbaddr.hpp\
	$(ROM_INC)/vistaobj.hpp\
	$(ROM_INC)/storedb.hpp\
	$(ROM_INC)/navmacro.hpp\
	$(ROM_INC)/keyobj.hpp\
	$(ROM_INC)/setobj.hpp\
	$(ROM_INC)/nav.hpp\
	$(ROM_INC)/dbrdx.hpp\
	$(ROM_INC)/storetsk.hpp\
	$(ROM_INC)/taskrdx.hpp\
	$(ROM_INC)/activate.hpp\
	$(ROM_INC)/impllock.h
	$(CPP) $(CPP_FLAGS) $<

dbaddr.o: \
	dbaddr.$(CPP_EXTENSION)\
	$(ROM_INC)/vpp.hpp\
	$(ROM_INC)/concat.hpp \
	$(ROM_INC)/defunx.hpp\
	$(ROM_INC)/verrout.hpp \
	$(ROM_INC)/dbaddr.hpp
	$(CPP) $(CPP_FLAGS) $<

dbrdx.o: \
	dbrdx.$(CPP_EXTENSION)\
	$(ROM_INC)/vpp.hpp\
	$(ROM_INC)/concat.hpp \
	$(ROM_INC)/defunx.hpp\
	$(ROM_INC)/verrout.hpp \
	$(ROM_INC)/dbrdx.hpp\
	$(ROM_INC)/storetsk.hpp\
	$(ROM_INC)/taskrdx.hpp\
	$(ROM_INC)/storedb.hpp\
	$(ROM_INC)/storeobj.hpp\
	$(ROM_INC)/activate.hpp\
	$(ROM_INC)/impllock.h\
	$(ROM_INC)/dbaddr.hpp\
	$(ROM_INC)/vistaobj.hpp\
	$(ROM_INC)/navmacro.hpp\
	$(ROM_INC)/keyobj.hpp\
	$(ROM_INC)/setobj.hpp\
	$(ROM_INC)/nav.hpp
	$(CPP) $(CPP_FLAGS) $<

grlckrdx.o: \
	grlckrdx.$(CPP_EXTENSION)\
	$(ROM_INC)/vpp.hpp\
	$(ROM_INC)/concat.hpp \
	$(ROM_INC)/defunx.hpp\
	$(ROM_INC)/verrout.hpp \
	$(ROM_INC)/grlock.hpp\
	$(ROM_INC)/storedb.hpp\
	$(ROM_INC)/vistaobj.hpp\
	$(ROM_INC)/storeobj.hpp\
	$(ROM_INC)/singlelr.hpp\
	$(ROM_INC)/storetsk.hpp\
	$(ROM_INC)/grlckrdx.hpp\
	$(ROM_INC)/omblob.hpp\
	$(ROM_INC)/poly.hpp\
	$(ROM_INC)/dbrdx.hpp\
	$(ROM_INC)/dbaddr.hpp\
	$(ROM_INC)/navmacro.hpp\
	$(ROM_INC)/keyobj.hpp\
	$(ROM_INC)/setobj.hpp\
	$(ROM_INC)/nav.hpp\
	$(ROM_INC)/taskrdx.hpp\
	$(ROM_INC)/generic.hpp\
	$(ROM_INC)/activate.hpp\
	$(ROM_INC)/impllock.h
	$(CPP) $(CPP_FLAGS) $<

grlock.o: \
	grlock.$(CPP_EXTENSION) \
	$(ROM_INC)/vpp.hpp\
	$(ROM_INC)/concat.hpp \
	$(ROM_INC)/defunx.hpp\
	$(ROM_INC)/verrout.hpp \
	$(ROM_INC)/grlock.hpp\
	$(ROM_INC)/storedb.hpp\
	$(ROM_INC)/vistaobj.hpp\
	$(ROM_INC)/storeobj.hpp\
	$(ROM_INC)/singlelr.hpp\
	$(ROM_INC)/storetsk.hpp\
	$(ROM_INC)/grlckrdx.hpp\
	$(ROM_INC)/omblob.hpp\
	$(ROM_INC)/poly.hpp\
	$(ROM_INC)/dbrdx.hpp\
	$(ROM_INC)/dbaddr.hpp\
	$(ROM_INC)/navmacro.hpp\
	$(ROM_INC)/keyobj.hpp\
	$(ROM_INC)/setobj.hpp\
	$(ROM_INC)/nav.hpp\
	$(ROM_INC)/taskrdx.hpp\
	$(ROM_INC)/generic.hpp\
	$(ROM_INC)/activate.hpp\
	$(ROM_INC)/impllock.h
	$(CPP) $(CPP_FLAGS) $<

generic.o: \
	generic.$(CPP_EXTENSION)\
	$(ROM_INC)/vpp.hpp\
	$(ROM_INC)/concat.hpp \
	$(ROM_INC)/defunx.hpp\
	$(ROM_INC)/verrout.hpp \
	$(ROM_INC)/storedb.hpp\
	$(ROM_INC)/storeobj.hpp\
	$(ROM_INC)/generic.hpp\
	$(ROM_INC)/hlfproto.h\
	$(ROM_INC)/dbrdx.hpp\
	$(ROM_INC)/storetsk.hpp\
	$(ROM_INC)/dbaddr.hpp\
	$(ROM_INC)/vistaobj.hpp\
	$(ROM_INC)/navmacro.hpp\
	$(ROM_INC)/keyobj.hpp\
	$(ROM_INC)/setobj.hpp\
	$(ROM_INC)/nav.hpp\
	$(ROM_INC)/taskrdx.hpp\
	$(ROM_INC)/activate.hpp\
	$(ROM_INC)/impllock.h
	$(CPP) $(CPP_FLAGS) $<

ce_hlf.o: \
	../se/ce_hlf.c\
	$(ROM_INC)/vpp.hpp\
	$(ROM_INC)/concat.hpp \
	$(ROM_INC)/defunx.hpp\
	$(ROM_INC)/verrout.hpp \
	$(ROM_INC)/hlfproto.h
	$(CC) $(CC_FLAGS) ../se/ce_hlf.c
#	$(CPP) $(CPP_FLAGS) ../se/ce_hlf.c

hlf.o: \
	../se/hlf.c\
	$(ROM_INC)/vpp.hpp\
	$(ROM_INC)/concat.hpp \
	$(ROM_INC)/defunx.hpp\
	$(ROM_INC)/verrout.hpp \
	$(ROM_INC)/hlfproto.h\
	$(ROM_INC)/hlf.h
	$(CC) $(CC_FLAGS) ../se/hlf.c

se_hlf.o: \
	../se/se_hlf.c\
	$(ROM_INC)/vpp.hpp\
	$(ROM_INC)/concat.hpp \
	$(ROM_INC)/defunx.hpp\
	$(ROM_INC)/verrout.hpp \
	$(ROM_INC)/hlfproto.h\
	$(ROM_INC)/se_hlf.h
	$(CC) $(CC_FLAGS) ../se/se_hlf.c

lc.o: lc.c
	$(CPP) $(CPP_FLAGS) $<
#	$(CC) $(CC_FLAGS) $<

nav.o: \
	nav.$(CPP_EXTENSION)\
	$(ROM_INC)/vpp.hpp\
	$(ROM_INC)/concat.hpp \
	$(ROM_INC)/defunx.hpp\
	$(ROM_INC)/verrout.hpp \
	$(ROM_INC)/storedb.hpp\
	$(ROM_INC)/storeobj.hpp\
	$(ROM_INC)/nav.hpp\
	$(ROM_INC)/hlfproto.h\
	$(ROM_INC)/dbrdx.hpp\
	$(ROM_INC)/storetsk.hpp\
	$(ROM_INC)/dbaddr.hpp\
	$(ROM_INC)/vistaobj.hpp\
	$(ROM_INC)/navmacro.hpp\
	$(ROM_INC)/keyobj.hpp\
	$(ROM_INC)/setobj.hpp\
	$(ROM_INC)/taskrdx.hpp\
	$(ROM_INC)/activate.hpp\
	$(ROM_INC)/impllock.h
	$(CPP) $(CPP_FLAGS) $<

omblob.o: \
	omblob.$(CPP_EXTENSION) \
	$(ROM_INC)/vpp.hpp\
	$(ROM_INC)/concat.hpp \
	$(ROM_INC)/defunx.hpp\
	$(ROM_INC)/verrout.hpp \
	$(ROM_INC)/storeobj.hpp\
	$(ROM_INC)/omblob.hpp\
	$(ROM_INC)/generic.hpp\
	$(ROM_INC)/ommem.hpp\
	$(ROM_INC)/omfile.hpp\
	$(ROM_INC)/hlfproto.h\
	$(ROM_INC)/dbaddr.hpp\
	$(ROM_INC)/vistaobj.hpp\
	$(ROM_INC)/storedb.hpp\
	$(ROM_INC)/navmacro.hpp\
	$(ROM_INC)/keyobj.hpp\
	$(ROM_INC)/setobj.hpp\
	$(ROM_INC)/nav.hpp\
	$(ROM_INC)/dbrdx.hpp\
	$(ROM_INC)/storetsk.hpp\
	$(ROM_INC)/taskrdx.hpp\
	$(ROM_INC)/activate.hpp\
	$(ROM_INC)/impllock.h
	$(CPP) $(CPP_FLAGS) $<

omfile.o: \
	omfile.$(CPP_EXTENSION)\
	$(ROM_INC)/vpp.hpp\
	$(ROM_INC)/concat.hpp \
	$(ROM_INC)/defunx.hpp\
	$(ROM_INC)/verrout.hpp \
	$(ROM_INC)/omfile.hpp
	$(CPP) $(CPP_FLAGS) $<

ommem.o: \
	ommem.$(CPP_EXTENSION)\
	$(ROM_INC)/vpp.hpp\
	$(ROM_INC)/concat.hpp \
	$(ROM_INC)/defunx.hpp\
	$(ROM_INC)/verrout.hpp \
	$(ROM_INC)/ommem.hpp
	$(CPP) $(CPP_FLAGS) $<

omquery.o: \
	omquery.$(CPP_EXTENSION)\
	$(ROM_INC)/vpp.hpp\
	$(ROM_INC)/concat.hpp \
	$(ROM_INC)/defunx.hpp\
	$(ROM_INC)/verrout.hpp \
	$(ROM_INC)/omquery.hpp\
	$(ROM_INC)/qtask.hpp\
	$(ROM_INC)/storedb.hpp\
	$(ROM_INC)/storeobj.hpp\
	$(ROM_INC)/dbrdx.hpp\
	$(ROM_INC)/storetsk.hpp\
	$(ROM_INC)/dbaddr.hpp\
	$(ROM_INC)/vistaobj.hpp\
	$(ROM_INC)/navmacro.hpp\
	$(ROM_INC)/keyobj.hpp\
	$(ROM_INC)/setobj.hpp\
	$(ROM_INC)/nav.hpp\
	$(ROM_INC)/taskrdx.hpp\
	$(ROM_INC)/activate.hpp\
	$(ROM_INC)/impllock.h
	$(CPP) $(CPP_FLAGS) $<

poly.o: \
	poly.$(CPP_EXTENSION)\
	$(ROM_INC)/vpp.hpp\
	$(ROM_INC)/concat.hpp \
	$(ROM_INC)/defunx.hpp\
	$(ROM_INC)/verrout.hpp \
	$(ROM_INC)/storedb.hpp\
	$(ROM_INC)/storeobj.hpp\
	$(ROM_INC)/poly.hpp\
	$(ROM_INC)/dbrdx.hpp\
	$(ROM_INC)/storetsk.hpp\
	$(ROM_INC)/dbaddr.hpp\
	$(ROM_INC)/vistaobj.hpp\
	$(ROM_INC)/navmacro.hpp\
	$(ROM_INC)/keyobj.hpp\
	$(ROM_INC)/setobj.hpp\
	$(ROM_INC)/nav.hpp\
	$(ROM_INC)/generic.hpp\
	$(ROM_INC)/taskrdx.hpp\
	$(ROM_INC)/activate.hpp\
	$(ROM_INC)/impllock.h
	$(CPP) $(CPP_FLAGS) $<

qtask.o: \
	qtask.$(CPP_EXTENSION)\
	$(ROM_INC)/vpp.hpp\
	$(ROM_INC)/concat.hpp \
	$(ROM_INC)/defunx.hpp\
	$(ROM_INC)/verrout.hpp \
	$(ROM_INC)/qtask.hpp\
	$(ROM_INC)/storedb.hpp\
	$(ROM_INC)/dbrdx.hpp\
	$(ROM_INC)/storetsk.hpp\
	$(ROM_INC)/taskrdx.hpp\
	$(ROM_INC)/activate.hpp\
	$(ROM_INC)/impllock.h
	$(CPP) $(CPP_FLAGS) $<

setobj.o: \
	setobj.$(CPP_EXTENSION)\
	$(ROM_INC)/vpp.hpp\
	$(ROM_INC)/concat.hpp \
	$(ROM_INC)/defunx.hpp\
	$(ROM_INC)/verrout.hpp \
	$(ROM_INC)/setobj.hpp\
	$(ROM_INC)/storedb.hpp\
	$(ROM_INC)/dbrdx.hpp\
	$(ROM_INC)/storetsk.hpp\
	$(ROM_INC)/taskrdx.hpp\
	$(ROM_INC)/activate.hpp
	$(CPP) $(CPP_FLAGS) $<


singlelr.o: \
	singlelr.$(CPP_EXTENSION)\
	$(ROM_INC)/vpp.hpp\
	$(ROM_INC)/concat.hpp \
	$(ROM_INC)/defunx.hpp\
	$(ROM_INC)/verrout.hpp \
	$(ROM_INC)/storedb.hpp\
	$(ROM_INC)/vistaobj.hpp\
	$(ROM_INC)/storeobj.hpp\
	$(ROM_INC)/singlelr.hpp\
	$(ROM_INC)/storetsk.hpp\
	$(ROM_INC)/dbrdx.hpp\
	$(ROM_INC)/dbaddr.hpp\
	$(ROM_INC)/navmacro.hpp\
	$(ROM_INC)/nav.hpp\
	$(ROM_INC)/keyobj.hpp\
	$(ROM_INC)/setobj.hpp\
	$(ROM_INC)/taskrdx.hpp\
	$(ROM_INC)/activate.hpp\
	$(ROM_INC)/impllock.h
	$(CPP) $(CPP_FLAGS) $<

storedb.o: \
	storedb.$(CPP_EXTENSION)\
	$(ROM_INC)/vpp.hpp\
	$(ROM_INC)/concat.hpp \
	$(ROM_INC)/defunx.hpp\
	$(ROM_INC)/verrout.hpp \
	$(ROM_INC)/storedb.hpp\
	$(ROM_INC)/hlfproto.h\
	$(ROM_INC)/dbrdx.hpp\
	$(ROM_INC)/storetsk.hpp\
	$(ROM_INC)/storeobj.hpp\
	$(ROM_INC)/taskrdx.hpp\
	$(ROM_INC)/dbaddr.hpp\
	$(ROM_INC)/vistaobj.hpp\
	$(ROM_INC)/navmacro.hpp\
	$(ROM_INC)/keyobj.hpp\
	$(ROM_INC)/setobj.hpp\
	$(ROM_INC)/nav.hpp\
	$(ROM_INC)/activate.hpp\
	$(ROM_INC)/impllock.h
	$(CPP) $(CPP_FLAGS) $<

storeobj.o: \
	storeobj.$(CPP_EXTENSION)\
	$(ROM_INC)/vpp.hpp\
	$(ROM_INC)/concat.hpp \
	$(ROM_INC)/defunx.hpp\
	$(ROM_INC)/verrout.hpp \
	$(ROM_INC)/storedb.hpp\
	$(ROM_INC)/storeobj.hpp\
	$(ROM_INC)/keyobj.hpp\
	$(ROM_INC)/nav.hpp\
	$(ROM_INC)/hlfproto.h\
	$(ROM_INC)/dbrdx.hpp\
	$(ROM_INC)/storetsk.hpp\
	$(ROM_INC)/dbaddr.hpp\
	$(ROM_INC)/vistaobj.hpp\
	$(ROM_INC)/navmacro.hpp\
	$(ROM_INC)/setobj.hpp\
	$(ROM_INC)/taskrdx.hpp\
	$(ROM_INC)/activate.hpp\
	$(ROM_INC)/impllock.h
	$(CPP) $(CPP_FLAGS) $<

storetsk.o: \
	storetsk.$(CPP_EXTENSION)\
	$(ROM_INC)/vpp.hpp\
	$(ROM_INC)/concat.hpp \
	$(ROM_INC)/defunx.hpp\
	$(ROM_INC)/verrout.hpp \
	$(ROM_INC)/storetsk.hpp\
	$(ROM_INC)/taskrdx.hpp\
	$(ROM_INC)/storedb.hpp\
	$(ROM_INC)/storeobj.hpp\
	$(ROM_INC)/activate.hpp\
	$(ROM_INC)/impllock.h\
	$(ROM_INC)/dbrdx.hpp\
	$(ROM_INC)/dbaddr.hpp\
	$(ROM_INC)/vistaobj.hpp\
	$(ROM_INC)/navmacro.hpp\
	$(ROM_INC)/keyobj.hpp\
	$(ROM_INC)/setobj.hpp\
	$(ROM_INC)/nav.hpp
	$(CPP) $(CPP_FLAGS) $<

taskrdx.o: \
	taskrdx.$(CPP_EXTENSION)\
	$(ROM_INC)/vpp.hpp\
	$(ROM_INC)/concat.hpp \
	$(ROM_INC)/defunx.hpp\
	$(ROM_INC)/verrout.hpp \
	$(ROM_INC)/taskrdx.hpp\
	$(ROM_INC)/hlfproto.h\
	$(ROM_INC)/activate.hpp\
	$(ROM_INC)/impllock.h\
	$(ROM_INC)/storeobj.hpp\
	$(ROM_INC)/dbaddr.hpp\
	$(ROM_INC)/vistaobj.hpp\
	$(ROM_INC)/storedb.hpp\
	$(ROM_INC)/navmacro.hpp\
	$(ROM_INC)/keyobj.hpp\
	$(ROM_INC)/setobj.hpp\
	$(ROM_INC)/nav.hpp\
	$(ROM_INC)/dbrdx.hpp\
	$(ROM_INC)/storetsk.hpp
	$(CPP) $(CPP_FLAGS) $<

transact.o: \
	transact.$(CPP_EXTENSION)\
	$(ROM_INC)/vpp.hpp\
	$(ROM_INC)/concat.hpp \
	$(ROM_INC)/defunx.hpp\
	$(ROM_INC)/verrout.hpp \
	$(ROM_INC)/transact.hpp\
	$(ROM_INC)/grlock.hpp\
	$(ROM_INC)/storetsk.hpp\
	$(ROM_INC)/grlckrdx.hpp\
	$(ROM_INC)/omblob.hpp\
	$(ROM_INC)/poly.hpp\
	$(ROM_INC)/taskrdx.hpp\
	$(ROM_INC)/storedb.hpp\
	$(ROM_INC)/storeobj.hpp\
	$(ROM_INC)/singlelr.hpp\
	$(ROM_INC)/generic.hpp\
	$(ROM_INC)/activate.hpp\
	$(ROM_INC)/impllock.h\
	$(ROM_INC)/dbrdx.hpp\
	$(ROM_INC)/dbaddr.hpp\
	$(ROM_INC)/vistaobj.hpp\
	$(ROM_INC)/navmacro.hpp\
	$(ROM_INC)/keyobj.hpp\
	$(ROM_INC)/setobj.hpp\
	$(ROM_INC)/nav.hpp
	$(CPP) $(CPP_FLAGS) $<

verrout.o: \
	verrout.$(CPP_EXTENSION)\
	$(ROM_INC)/vpp.hpp\
	$(ROM_INC)/concat.hpp \
	$(ROM_INC)/defunx.hpp\
	$(ROM_INC)/verrout.hpp\
	$(ROM_INC)/storetsk.hpp\
	$(ROM_INC)/romerr.h\
	$(ROM_INC)/taskrdx.hpp\
	$(ROM_INC)/storedb.hpp\
	$(ROM_INC)/activate.hpp\
	$(ROM_INC)/impllock.h\
	$(ROM_INC)/dbrdx.hpp
	$(CPP) $(CPP_FLAGS) $<

vistaobj.o: \
	vistaobj.$(CPP_EXTENSION)\
	$(ROM_INC)/vpp.hpp\
	$(ROM_INC)/concat.hpp \
	$(ROM_INC)/defunx.hpp\
	$(ROM_INC)/verrout.hpp\
	$(ROM_INC)/vistaobj.hpp
	$(CPP) $(CPP_FLAGS) $<

