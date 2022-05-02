export ARCH = x64
export BITS = $(subst 86,32,$(subst x,,$(ARCH)))
export CXX = g++ -m$(BITS)
export CC = gcc -m$(BITS)
export AR = ar -crs
export LD = ld

THIS_PATH := $(abspath makefile)
export ROOTDIR := $(dir $(THIS_PATH))
export MKINCDIR := $(ROOTDIR)build/make/include
include $(MKINCDIR)/common.mk

# Project Names
libAS = angelscript
libPGE = olcPGE
libSTB_image = stb_image
exePXG = pixelgame

all: $(exePXG)
#@echo $(lastword $(subst /, ,$(ROOTDIR)))

$(BUILDDIR)/%:
	@-$(MKDIR) $(subst /,\,$@)

$(libAS) $(LIBDIR)/$(libAS).a: $(LIBDIR)
	@echo ----------- Building AngelScript Scripting static library -----------
	@$(MAKE) -C libs/$(libAS)

$(libSTB_image) $(LIBDIR)/$(libSTB_image).a: $(LIBDIR)
	@echo -----------           Building STBimage Library           -----------
	@$(MAKE) -C libs/$(libSTB_image)

$(libPGE) $(LIBDIR)/$(libPGE).a: $(LIBDIR) $(libSTB_image)
	@echo -----------        Building olc PGE static library        -----------
	@$(MAKE) -C libs/$(libPGE)

$(exePXG) $(LIBDIR)/$(exePXG)/$(exePXG)$(EXEext): pxgdata $(libAS) $(libPGE)
	@echo -----------              Building Pixel Game              -----------
	@$(MAKE) -C $(exePXG)

pxgdata: $(PXGDIR)
	@$(COPYDIR) $(subst /,\,"$(COMMONDIR)/pxg" "$(BUILDDIR)/$(exePXG)/pxg")

clean:
	@-$(DELDIR) build\x86
	@-$(DELDIR) build\x64
	@echo ----------- Cleaning AngelScript Language Scripting Library -----------
	@$(MAKE) -C libs/$(libAS) clean
	@echo -----------            Cleaning STBimage Library            -----------
	@$(MAKE) -C libs/$(libSTB_image) clean
	@echo -----------             Cleaning olcPGE Library             -----------
	@$(MAKE) -C libs/$(libPGE) clean
	@echo -----------               Cleaning Pixel Game               -----------
	@$(MAKE) -C $(exePXG) clean

.PHONY: all $(libAS) $(libSTB_image) $(libPGE) $(exePXG) pxgdata clean