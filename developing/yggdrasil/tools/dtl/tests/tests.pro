include (../variables.pri)

TARGET=tests
DESTDIR=../bin
TEMPLATE=app
LIBS += -lDTL
CONFIG += console

HEADERS += \
   Example.h \
   IterSemanticsTest.h \
   JoinExample.h \
   print_util.h \
   TestExceptions.h \

SOURCES += \
   Example.cpp \
   IterSemanticsTest.cpp \
   JoinExample.cpp \
   main.cpp \
   print_util.cpp \
   TestExceptions.cpp \
   test_nullable.cpp \
   test_join.cpp \
   tests.cpp \

