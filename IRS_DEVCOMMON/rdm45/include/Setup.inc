#
# This file is included by makefiles to configure the build switches based 
# on the DEBUG and BUILD environment variables.
#
# Usage:
#
#    set DEBUG=1    (debug build)
# -or-
#    set BUILD=D    (debug build)
#    set BUILD=B    (beta release build)
#    set BUILD=N    (release build w/o compiler optimizations)
#    set BUILD=R    (final release build)
#


# convert to upper case
!if "$(BUILD)" == "d"
BUILD = D
!else
!if "$(BUILD)" == "b"
BUILD = B
!else
!if "$(BUILD)" == "n"
BUILD = N
!else
!if "$(BUILD)" == "r"
BUILD = R
!endif
!endif
!endif
!endif


# make sure DEBUG and BUILD don't conflict
!ifdef DEBUG
!ifndef BUILD
BUILD = D
!else
!if "$(BUILD)" != "D"
!error Conflict between BUILD and DEBUG environment variables.
!endif
!endif
!endif


# Default is No-opt release
!ifndef BUILD
BUILD = N
!endif


!if "$(BUILD)" == "D"
DEBUG = 1
NO_OPTS = 1
DB_DEBUG = -DDB_DEBUG
CONFIG = Debug build.

!else
!if "$(BUILD)" == "B"
!undef DEBUG
NO_OPTS = 1
DB_DEBUG = -DDB_DEBUG
CONFIG = Beta release build.

!else
!if "$(BUILD)" == "N"
!undef DEBUG
NO_OPTS = 1
!undef DB_DEBUG
CONFIG = No-optimization build.

!else
!if "$(BUILD)" == "R"
!undef DEBUG
!undef NO_OPTS
!undef DB_DEBUG
CONFIG = Final Release build.

!else
!error Invalid build option '$(BUILD)'.

!endif # R
!endif # N
!endif # B
!endif # D

# $Revision:   1.6  $
