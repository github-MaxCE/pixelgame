include $(MKINCDIR)/common.mk

CXXFLAGS += -O3
SRCDIR = src
INCDIR = include \
$(ROOTINCDIR) \
$(ROOTINCDIR)/angelscript
OBJDIR = obj
LIBDEPS = angelscript.a \
olcPGE.a

SRCNAMES = AssetManager.cpp \
Entity.cpp \
filemanager.cpp \
Gameobject.cpp \
main.cpp \
map.cpp \
mathutils.cpp \
Player.cpp \
ScriptEngine.cpp \
ScriptEntity.cpp \
ScriptObject.cpp

OBJ = $(addprefix $(OBJDIR)/, $(notdir $(SRCNAMES:.cpp=.o)))
INCDIR := $(addprefix -I, $(INCDIR))
BIN = $(PXGDIR)/pixelgame$(EXE)
all: $(BIN)

$(BIN): $(OBJ)
	@echo -----------                Linking Pixel Game                -----------
	$(CXX) $^ $(addprefix $(LIBDIR)/, $(LIBDEPS)) -o $@

$(OBJDIR):
	@-$(MKDIR) $(subst /,\,$(OBJDIR))

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(OBJDIR)
	@echo ^> $(notdir $<)
	@$(CXX) $(INCDIR) $(CXXFLAGS) -c $< -o $@

clean:
	@-$(DELDIR) $(subst /,\,$(OBJDIR))

.PHONY: all clean