# This piece builds the object list from the target's provided sources
__OBJS_INTERMEDIATE := $(addprefix $(OBJDIR)/,$(SOURCES))
__OBJS_INTERMEDIATE := $(__OBJS_INTERMEDIATE:.S=.o)
__OBJS_INTERMEDIATE := $(__OBJS_INTERMEDIATE:.c=.o)
__OBJS_INTERMEDIATE := $(__OBJS_INTERMEDIATE:.cpp=.o)
OBJECTS := $(__OBJS_INTERMEDIATE)
