include $(MKINCDIR)/common.mk

CXXFLAGS += $(PGEFLAGS) -O3
OBJDIR = obj
SRCDIR = src
INCDIR = $(ROOTINCDIR) \
$(ROOTINCDIR)/stb_image
LIBDEPS = stb_image.a
SRCNAMES = olcPixelGameEngine.cpp \
olcPGEX_Graphics2D.cpp

OBJ = $(addprefix $(OBJDIR)/, $(notdir $(SRCNAMES:.cpp=.o)))
INCDIR := $(addprefix -I, $(INCDIR))
BIN = $(LIBDIR)/olcPGE.a
all: $(BIN)

$(BIN): $(OBJ)
	@echo -----------       Archiving olcPGE Library       -----------
	@$(AR) -rcs $@ $?

$(OBJDIR):
	@-$(MKDIR) $(subst /,\,$(OBJDIR))

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(OBJDIR)
	@echo ^> $(notdir $<)
	@$(CXX) $(INCDIR) $(CXXFLAGS) -c $< -o $@ -l$(addprefix $(LIBDIR),$(LIBDEPS))

clean:
	@-$(DELDIR) $(subst /,\,$(OBJDIR))

.PHONY: all clean