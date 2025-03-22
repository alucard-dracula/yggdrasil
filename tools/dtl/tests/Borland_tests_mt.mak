BORLAND_PATH = C:\Borland\BCC55
DEFS = -I..\lib -DWIN32
CC = $(BORLAND_PATH)\Bin\bcc32.exe
LINK = $(BORLAND_PATH)\Bin\ILINK32.EXE

!ifndef BMODE
BMODE = DEBUG
!endif

!if $(BMODE) != RELEASE && $(BMODE) != DEBUG
!  error Illegal value for BMODE option
!endif

CPPFLAGS = -w-8027 -w-8026 -w-8057 -w-8022 -w-8004 -w-8008 -tWC -tWM 
LDFLAGS  = -q -ap -Gn -L$(BORLAND_PATH)\lib\PSDK;..\lib

!if $(BMODE) == RELEASE
	DEFS=-DNDEBUG $(DEFS)
	CPPFLAGS=$(DEFS) -v- -vi- -O1 -k- $(CPPFLAGS)
	LDFLAGS= -v- -x $(LDFLAGS)
	!message Building release version of project
!else
	DEFS=-D_DEBUG $(DEFS)
	CPPFLAGS=$(DEFS) -v -vi- -Od -k -R -Q -M -y -r- $(CPPFLAGS)
	LDFLAGS= -v -m -M $(LDFLAGS)
	!message Building debug version of project
!endif

OBJECTS =  IterSemanticsTest.obj print_util.obj main.obj \
test_join.obj test_nullable.obj TestExceptions.obj JoinExample.obj Example.obj test_dtl_posix_time.obj test_dtl_time.obj  test_dtl_date.obj

all: tests

clean:
	-@if exist *.obj del *.obj                 >nul 
	-@if exist tests.exe del tests.exe    >nul


tests: $(OBJECTS) $(DEPENDENCIES)
	$(LINK) $(LDFLAGS) c0x32.obj $(OBJECTS), tests, , CW32mt.LIB IMPORT32.LIB
