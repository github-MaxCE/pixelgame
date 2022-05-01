export ARCH = x86
export CXX = g++ $(subst 86,32,$(subst x,-m,$(ARCH)))
export CC = gcc $(subst 86,32,$(subst x,-m,$(ARCH)))
export AR = ar
export LD = ld

THIS_PATH := $(abspath makefile)
export ROOTDIR := $(dir $(THIS_PATH))
export MKINCDIR := $(ROOTDIR)build/make/include
include $(MKINCDIR)/common.mk

angelscript = libs/angelscript
olcPGE = libs/olcPGE
stb_image = libs/stb_image
pixelgame = pixelgame

all: angelscript stb_image olcPGE pixelgame

$(BUILDDIR)/%:
	$(MKDIR) $(subst /,\,$@)

angelscript: $(angelscript)/makefile $(LIBDIR)
	@echo ----------- Building AngelScript Scripting static library -----------
	@$(MAKE) -C $(angelscript)

stb_image: $(stb_image)/makefile $(LIBDIR)
	@echo -----------           Building STBimage Library           -----------
	@$(MAKE) -C $(stb_image)

olcPGE: $(olcPGE)/makefile $(LIBDIR)
	@echo -----------        Building olc PGE static library        -----------
	@$(MAKE) -C $(olcPGE)

pixelgame: $(pixelgame)/makefile $(PXGDIR) pxgdata
	@echo -----------              Building Pixel Game              -----------
	@$(MAKE) -C $(pixelgame)

pxgdata:
	@$(COPYDIR) $(subst /,\,"$(COMMONDIR)/pxg" "$(BUILDDIR)/pixelgame/pxg")

clean:
	@-$(DELDIR) build\x86
	@-$(DELDIR) build\x64
	@echo ----------- Cleaning AngelScript Language Scripting Library -----------
	@$(MAKE) -C $(angelscript) clean
	@echo -----------             Cleaning olcPGE Library             -----------
	@$(MAKE) -C $(olcPGE) clean
	@echo -----------               Cleaning Pixel Game               -----------
	@$(MAKE) -C $(pixelgame) clean
	@echo -----------            Cleaning STBimage Library            -----------
	@$(MAKE) -C $(stb_image) clean

.PHONY: angelscript olcPGE_static pixelgame clean