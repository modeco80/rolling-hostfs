.PHONY: all clean

all: $(OBJDIR)/ $(OBJDIR)/$(NAME).a

clean:
	rm -rf $(EXTRATARGETS_PRE) $(EXTRATARGETS_POST) $(OBJDIR)/$(NAME).a $(OBJDIR)/

$(OBJDIR)/$(NAME).a: $(OBJECTS)
	$(info Creating library $@)
	$(AR) r $@ $(OBJECTS)
