# Angelscript MingW makefile
include $(MKINCDIR)/common.mk

# Debug flags: -ggdb -DAS_DEBUG
# Release flags: -O2
#  to exclude the script compiler: -DAS_NO_COMPILER
#  to use only generic calling convention: -DAS_MAX_PORTABILITY
CXXFLAGS += -O0 -fno-strict-aliasing -DANGELSCRIPT_EXPORT
SRCDIR = src
INCDIR = include \
$(ROOTINCDIR)/angelscript
OBJDIR = obj
SRCNAMES = as_atomic.cpp \
as_builder.cpp \
as_bytecode.cpp \
as_callfunc.cpp \
as_callfunc_mips.cpp \
as_callfunc_ppc_64.cpp \
as_callfunc_ppc.cpp \
as_callfunc_sh4.cpp \
as_callfunc_x86.cpp \
as_callfunc_x64_mingw.cpp \
as_compiler.cpp \
as_configgroup.cpp \
as_context.cpp \
as_datatype.cpp \
as_generic.cpp \
as_gc.cpp \
as_globalproperty.cpp \
as_memory.cpp \
as_module.cpp \
as_objecttype.cpp \
as_outputbuffer.cpp \
as_parser.cpp \
as_restore.cpp \
as_scriptcode.cpp \
as_scriptengine.cpp \
as_scriptfunction.cpp \
as_scriptnode.cpp \
as_scriptobject.cpp \
as_string.cpp \
as_string_util.cpp \
as_thread.cpp \
as_tokenizer.cpp \
as_typeinfo.cpp \
as_variablescope.cpp

OBJ = $(addprefix $(OBJDIR)/, $(notdir $(SRCNAMES:.cpp=.o)))
INCDIR := $(addprefix -I, $(INCDIR))
BIN = $(LIBDIR)/angelscript.a
all: $(BIN)

$(BIN): $(OBJ)
	@echo ----------- Archiving AngelScript Language Scripting Library -----------
	@$(AR) -rcs $@ $?

$(OBJDIR):
	@-$(MKDIR) $(subst /,\,$(OBJDIR))

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(OBJDIR)
	@echo ^> $(notdir $<)
	@$(CXX) $(INCDIR) $(CXXFLAGS) -c $< -o $@

clean:
	@-$(DELDIR) $(subst /,\,$(OBJDIR))

.PHONY: all clean