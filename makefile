export ARCH ?= x86
export CXX ?= gcc
export AR ?= ar
export LD ?= ld

THIS_PATH := $(abspath makefile)
export ROOTDIR := $(dir $(THIS_PATH))
export MKINCDIR := $(ROOTDIR)build/make/include
include $(MKINCDIR)/common.mk

AS_PATH = libs/angelscript/
PGE_PATH = libs/olcPGE_static
PXG_PATH = pixelgame


all: angelscript olcPGE_static pixelgame

$(BUILDDIR)/%:
	$(MKDIR) $(subst /,\,$@)

angelscript: $(AS_PATH)/makefile #$(BUILDDIR)/libs
	@echo ----------- Building AngelScript Scripting static library -----------
	@$(MAKE) -C $(AS_PATH)

olcPGE_static: $(PGE_PATH)/makefile $(BUILDDIR)/libs
	@echo -----------        Building olc PGE static library        -----------
	@$(MAKE) -C $(PGE_PATH)

pixelgame: $(PXG_PATH)/makefile $(BUILDDIR)/pixelgame pxgdata
	@echo -----------              Building Pixel Game              -----------
	@$(MAKE) -C $(PXG_PATH)

pxgdata:
	@$(COPYDIR) $(subst /,\,"$(COMMONDIR)/pxg" "$(BUILDDIR)/pixelgame/pxg")

clean:
	@-$(DELDIR) build\x86
	@-$(DELDIR) build\x64
	@echo ----------- Cleaning AngelScript static Library -----------
	@$(MAKE) -C $(AS_PATH) clean
	@echo -----------   Cleaning olc PGE static Library   -----------
	@$(MAKE) -C $(PGE_PATH) clean
	@echo -----------         Cleaning Pixel Game         -----------
	@$(MAKE) -C $(PXG_PATH) clean

.PHONY: angelscript olcPGE_static pixelgame clean