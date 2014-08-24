# Written for GNU -*- makefile -*-.

mydirs = ../lib
mylibs = -lDTL

# Where to find -lDTL build
libDTLdir = ../lib

$(TARGET): $(OBJECTS) $(mylibs)
