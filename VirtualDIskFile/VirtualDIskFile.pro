QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        api/api.cpp \
        api/lua/lapi.c \
        api/lua/lauxlib.c \
        api/lua/lbaselib.c \
        api/lua/lbitlib.c \
        api/lua/lcode.c \
        api/lua/lcorolib.c \
        api/lua/lctype.c \
        api/lua/ldblib.c \
        api/lua/ldebug.c \
        api/lua/ldo.c \
        api/lua/ldump.c \
        api/lua/lfunc.c \
        api/lua/lgc.c \
        api/lua/linit.c \
        api/lua/liolib.c \
        api/lua/llex.c \
        api/lua/lmathlib.c \
        api/lua/lmem.c \
        api/lua/loadlib.c \
        api/lua/lobject.c \
        api/lua/lopcodes.c \
        api/lua/loslib.c \
        api/lua/lparser.c \
        api/lua/lstate.c \
        api/lua/lstring.c \
        api/lua/lstrlib.c \
        api/lua/ltable.c \
        api/lua/ltablib.c \
        api/lua/ltm.c \
        api/lua/lua.c \
        api/lua/luac.c \
        api/lua/lundump.c \
        api/lua/lutf8lib.c \
        api/lua/lvm.c \
        api/lua/lzio.c \
        disk.cpp \
        diskhandler.cpp \
        utils/diskutil.cpp \
        main.cpp \
        masterbootrecord.cpp \
        partition.cpp \
        utils/cli.cpp \
        utils/mbrutils.cpp \
        utils/utils.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    FileSystemType.h \
    api/api.h \
    api/lua/lapi.h \
    api/lua/lauxlib.h \
    api/lua/lcode.h \
    api/lua/lctype.h \
    api/lua/ldebug.h \
    api/lua/ldo.h \
    api/lua/lfunc.h \
    api/lua/lgc.h \
    api/lua/llex.h \
    api/lua/llimits.h \
    api/lua/lmem.h \
    api/lua/lobject.h \
    api/lua/lopcodes.h \
    api/lua/lparser.h \
    api/lua/lprefix.h \
    api/lua/lstate.h \
    api/lua/lstring.h \
    api/lua/ltable.h \
    api/lua/ltm.h \
    api/lua/lua.h \
    api/lua/lua.hpp \
    api/lua/luaconf.h \
    api/lua/lualib.h \
    api/lua/lundump.h \
    api/lua/lvm.h \
    api/lua/lzio.h \
    disk.h \
    diskhandler.h \
    utils/diskutil.h \
    masterbootrecord.h \
    partition.h \
    progressbar.h \
    utils/cli.h \
    utils/mbrutils.h \
    utils/utils.h
