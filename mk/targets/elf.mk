.PHONY: all clean

all: $(BINDIR)/ $(EXTRATARGETS_PRE) $(BINDIR)/$(NAME).elf $(EXTRATARGETS_POST)

clean:
	rm -rf $(EXTRATARGETS_PRE) $(EXTRATARGETS_POST) $(BINDIR)/$(NAME).elf $(BINDIR)/ $(OBJDIR)/

$(BINDIR)/:
	mkdir -p $@

$(BINDIR)/$(NAME).elf: $(OBJECTS)
	$(info Creating ELF $@)
	$(LD) -T $(LINKSCRIPT) $(EE_CRTBEGIN) $(OBJECTS) $(EE_CRTEND) $(LINKLIBS) -o $@
