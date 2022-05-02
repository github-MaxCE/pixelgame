ifeq ($(OS), Windows_NT)
EXEext = .exe
DEL = del /f /q
DELDIR = rmdir /s /q
MKDIR = mkdir
COPY = xcopy /h/e/c/i/d/y
COPYDIR := $(COPY)
PGEFLAGS = -luser32 -lShlwapi -ldwmapi -lopengl32 -DOLC_IMAGE_STB -DOLC_GFX_OPENGL33
else
EXEext = 
DEL = rm -f
DELDIR = rmdir -r
MKDIR = mkdir -p
COPY = cp
COPYDIR = cp -r
PGEFLAGS = 
endif
ARCHFLAG = $(subst 86,32,$(subst x,-m,$(ARCH)))
ROOTFLAGS := -Wall -Wno-unknown-pragmas -Wno-switch -Wno-sign-compare
CXXFLAGS = $(ROOTFLAGS) -Wno-reorder -lstdc++ -lstdc++fs -std=c++20
CFLAGS = $(ROOTFLAGS)
ROOTINCDIR = $(ROOTDIR)include
COMMONDIR = $(ROOTDIR)build/common
BUILDDIR = $(ROOTDIR)build/$(ARCH)
LIBDIR = $(BUILDDIR)/libs
PXGDIR = $(BUILDDIR)/pixelgame

$(OBJDIR):
	@-$(MKDIR) $(subst /,\,$(OBJDIR))