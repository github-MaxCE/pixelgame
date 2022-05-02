OBJDIR = obj
include $(MKINCDIR)/common.mk

CFLAGS = $(ROOTFLAGS) -O3
SRCDIR = src
INCDIR = $(ROOTINCDIR)/stb_image
SRCNAMES = stb_image.c

OBJ = $(addprefix $(OBJDIR)/, $(notdir $(SRCNAMES:.c=.o)))
INCDIR := $(addprefix -I, $(INCDIR))
BIN = $(LIBDIR)/stb_image.a
all: $(OBJDIR) $(BIN)

$(BIN): $(OBJ)
	@echo -----------            Archiving STBimage Library            -----------
	@$(AR) $@ $?

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo ^> $(notdir $<)
	@$(CC) $(INCDIR) $(CFLAGS) -c $< -o $@

clean:
	@-$(DELDIR) $(subst /,\,$(OBJDIR))

.PHONY: all clean