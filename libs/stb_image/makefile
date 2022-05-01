include $(MKINCDIR)/common.mk

CFLAGS = $(ROOTFLAGS) -O3
OBJDIR = obj
SRCDIR = src
INCDIR = $(ROOTINCDIR)/stb_image
SRCNAMES = stb_image.c

OBJ = $(addprefix $(OBJDIR)/, $(notdir $(SRCNAMES:.c=.o)))
INCDIR := $(addprefix -I, $(INCDIR))
BIN = $(LIBDIR)/stb_image.a
all: $(BIN)

$(BIN): $(OBJ)
	@echo -----------            Archiving STBimage Library            -----------
	@$(AR) -rcs $@ $?

$(OBJDIR):
	@-$(MKDIR) $(subst /,\,$(OBJDIR))

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(OBJDIR)
	@echo ^> $(notdir $<)
	$(CC) $(INCDIR) $(CFLAGS) -c $< -o $@

clean:
	@-$(DELDIR) $(subst /,\,$(OBJDIR))

.PHONY: all clean