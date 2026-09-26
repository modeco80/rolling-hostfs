NAME = rolling_hostfs_$(REGION)
TYPE := elf
DEBUG ?= 0

# Runtime sources (startup code, and code
# needed to link the ml libs)
RUNTIME := \
	crt0.S \
	runtime/ml_abort.cpp

UTILS := \
	utils/log.cpp \
	utils/hook/trampoline.cpp

SOURCES := \
	$(RUNTIME) \
	$(UTILS) \
	main.cpp \
	wad_hooks.cpp

LINKSCRIPT = $(OBJDIR)/blob.ld
LINKLIBS := \
	../ml/lib/core/obj/libml3_core.a \
	../ml/lib/cxx/obj/libml3_cxx.a \
	../ml/lib/hw/obj/libml3_hw.a

# User C/CXX flags.
USER_CFLAGS = -DMOD_REGION_$(REGION) -I $(TOP)/src/rollapi/include
USER_CXXFLAGS := \
	-Wno-pmf-conversions

# Release builds (to remove mlASSERTs:)
ifeq ($(DEBUG),0)
USER_CFLAGS += -DNDEBUG
endif

# Pre deps
EXTRATARGETS_PRE = $(LINKSCRIPT)
EXTRATARGETS_POST = $(TOP)/elf/rolling_$(REGION)_hostfs_patched.elf

# All files that make up the mkpatch tool.
# Used to make sure that we rebuild if the Python code is changed.
MKPATCH_DEPS := \
	$(TOP)/tools/mkpatch.py \
	$(TOP)/tools/utils/elf.py \
	$(TOP)/tools/utils/ldparse.py \
	$(TOP)/tools/utils/mips.py

include $(TOP)/mk/makedef.mk

# Preprocess the linker script to include game symbols here.
$(OBJDIR)/blob.ld: $(OBJDIR)/ blob.ld.in
	$(info Preprocessing linker script $@)
	sed 's|@REGION@|$(REGION)|' blob.ld.in > $@

$(TOP)/elf/rolling_$(REGION)_hostfs_patched.elf: bin/$(NAME).elf $(MKPATCH_DEPS)
	$(info Making patched ELF $@)
	$(TOP)/tools/mkpatch.py $(REGION)

