include $(MKINCDIR)/common.mk

CXXFLAGS = $(EXEFLAGS) $(PGEFLAGS) -O3
SRCDIR = src
INCDIR = include \
$(ROOTINCDIR) \
$(ROOTINCDIR)/angelscript
OBJDIR = obj
SRCNAMES = ScriptEngine.cpp \
AssetManager.cpp \
filemanager.cpp \
mathutils.cpp \
Gameobject.cpp \
Entity.cpp \
Player.cpp \
ScriptObject.cpp \
ScriptEntity.cpp \
map.cpp \
main.cpp

OBJ = $(addprefix $(OBJDIR)/, $(notdir $(SRCNAMES:.cpp=.o)))
INCDIR := $(addprefix -I, $(INCDIR))
BIN = $(BUILDDIR)/pixelgame/pixelgame$(EXE)
all: $(BIN)

$(BIN): $(OBJ)
	@echo -----------          Linking Pixel Game          -----------
	@$(CXX) $(LIBDIR)/angelscript.a $(LIBDIR)/olcPGE_static.a -o $@

$(OBJDIR):
	@-$(MKDIR) $(subst /,\,$(OBJDIR))

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(OBJDIR)
	@echo ^> $(notdir $<)
	@$(CXX) $(INCDIR) $(CXXFLAGS) -c $< -o $@

clean:
	@-$(DELDIR) $(subst /,\,$(OBJDIR))

.PHONY: all clean