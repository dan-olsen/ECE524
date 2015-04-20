TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    given.c \
    Initialization.c \
    Stack.c \
    Cleanup.c \
    StorePaths.c \
    PatternSim.c

HEADERS += \
    given.h \
    Initialization.h \
    Stack.h \
    Cleanup.h \
    StorePaths.h \
    PathSet.h \
    PatternSim.h


unix:!macx: LIBS += -L$$PWD/../../../../../opt/cudd-2.5.1/cudd/ -lcudd

INCLUDEPATH += $$PWD/../../../../../opt/cudd-2.5.1/cudd
DEPENDPATH += $$PWD/../../../../../opt/cudd-2.5.1/cudd

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../../../opt/cudd-2.5.1/cudd/libcudd.a

unix:!macx: LIBS += -L$$PWD/../../../../../opt/cudd-2.5.1/dddmp/ -ldddmp

INCLUDEPATH += $$PWD/../../../../../opt/cudd-2.5.1/dddmp
DEPENDPATH += $$PWD/../../../../../opt/cudd-2.5.1/dddmp

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../../../opt/cudd-2.5.1/dddmp/libdddmp.a

unix:!macx: LIBS += -L$$PWD/../../../../../opt/cudd-2.5.1/epd/ -lepd

INCLUDEPATH += $$PWD/../../../../../opt/cudd-2.5.1/epd
DEPENDPATH += $$PWD/../../../../../opt/cudd-2.5.1/epd

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../../../opt/cudd-2.5.1/epd/libepd.a

unix:!macx: LIBS += -L$$PWD/../../../../../opt/cudd-2.5.1/mtr/ -lmtr

INCLUDEPATH += $$PWD/../../../../../opt/cudd-2.5.1/mtr
DEPENDPATH += $$PWD/../../../../../opt/cudd-2.5.1/mtr

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../../../opt/cudd-2.5.1/mtr/libmtr.a

unix:!macx: LIBS += -L$$PWD/../../../../../opt/cudd-2.5.1/st/ -lst

INCLUDEPATH += $$PWD/../../../../../opt/cudd-2.5.1/st
DEPENDPATH += $$PWD/../../../../../opt/cudd-2.5.1/st

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../../../opt/cudd-2.5.1/st/libst.a

unix:!macx: LIBS += -L$$PWD/../../../../../opt/cudd-2.5.1/util/ -lutil

INCLUDEPATH += $$PWD/../../../../../opt/cudd-2.5.1/util
DEPENDPATH += $$PWD/../../../../../opt/cudd-2.5.1/util

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../../../opt/cudd-2.5.1/util/libutil.a
