OBJDIR = obj
include $(MKINCDIR)/common.mk

CXXFLAGS += $(PGEFLAGS) -O3
SRCDIR = src
INCDIR = $(ROOTINCDIR) \
$(ROOTINCDIR)/stb_image
LIBDEPS = stb_image.a
SRCNAMES = olcPixelGameEngine.cpp \
olcPGEX_Graphics2D.cpp

OBJ = $(addprefix $(OBJDIR)/, $(notdir $(SRCNAMES:.cpp=.o)))
INCDIR := $(addprefix -I, $(INCDIR))
BIN = $(LIBDIR)/olcPGE.a
all: $(OBJDIR) $(BIN)

$(BIN): $(OBJ)
	@echo -----------       Archiving olcPGE Library       -----------
	@$(AR) $@ $? $(addprefix $(LIBDIR)/,$(LIBDEPS))

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@echo ^> $(notdir $<)
	@$(CXX) $(INCDIR) $(CXXFLAGS) -c $< -o $@

clean:
	@-$(DELDIR) $(subst /,\,$(OBJDIR))

.PHONY: all clean