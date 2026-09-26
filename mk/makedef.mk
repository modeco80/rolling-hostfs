# This master makefile orcestrates the build for all subprojects.
#
# These files in //mk are intended to be the only major source of most recipes
# and rules in the build system, excluding any special extra rules.

include $(TOP)/mk/ports.mk
include $(TOP)/mk/ee.mk
include $(TOP)/mk/objects.mk

# Include the final rules for a target here
include $(TOP)/mk/targets/$(TYPE).mk

$(OBJDIR)/:
	mkdir -p $@

# Fun make trick to allow make V=1 to work as a
# verbose flag.
$V.SILENT:

