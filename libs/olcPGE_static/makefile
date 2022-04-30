include $(MKINCDIR)/common.mk

CXXFLAGS = $(ROOTFLAGS) -O3
OBJDIR = obj
SRCDIR = src
INCDIR = $(ROOTINCDIR)
SRCNAMES = olcPixelGameEngine.cpp \
olcPGEX_Graphics2D.cpp \
olcPGEX_FrostUI.cpp


OBJ = $(addprefix $(OBJDIR)/, $(notdir $(SRCNAMES:.cpp=.o)))
INCDIR := $(addprefix -I, $(INCDIR))
BIN = $(LIBDIR)/olcPGE_static.a
all: $(BIN)

$(BIN): $(OBJ)
	@echo -----------     Archiving PGE static Library     -----------
	@$(AR) -rcs $@ $?

$(OBJDIR):
	@-$(MKDIR) $(subst /,\,$(OBJDIR))

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(OBJDIR)
	@echo ^> $(notdir $<)
	@$(CXX) $(INCDIR) $(CXXFLAGS) -c $< -o $@

clean:
	@-$(DELDIR) $(subst /,\,$(OBJDIR))

.PHONY: all clean