ifeq ($(OS), Windows_NT)
EXE = .exe
DEL = del /f /q
DELDIR = rmdir /s /q
MKDIR = mkdir
COPY = xcopy /h /e /c /i /d
COPYDIR := $(COPY)
PGEFLAGS = -luser32 -lgdi32 -lgdiplus -lShlwapi -ldwmapi -lopengl32
else
EXE = 
DEL = rm -f
DELDIR = rmdir -r
MKDIR = mkdir -p
COPY = cp
COPYDIR = cp -r
PGEFLAGS = 
endif
ROOTFLAG := $(subst x,-m,$(ARCH)) $(PGEFLAGS) -Wall -lstdc++ -lc++fs -std=c++20
ROOTINCDIR = $(ROOTDIR)include
COMMONDIR = $(ROOTDIR)build/common
BUILDDIR = $(ROOTDIR)build/$(ARCH)
LIBDIR = $(BUILDDIR)/libs
PXGDIR = $(BUILDDIR)/pixelgame