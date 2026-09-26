# EE-specific core make rules.
EE_TOOLSDIR := $(TOP)/tools/cc/$(GCC_VERSION)

# EE tools
CC := $(EE_TOOLSDIR)/bin/ee-gcc
CXX := $(EE_TOOLSDIR)/bin/ee-g++
AR := $(EE_TOOLSDIR)/bin/ee-ar
LD := $(EE_TOOLSDIR)/bin/ee-ld
OBJCOPY := $(EE_TOOLSDIR)/bin/ee-objcopy
OBJDUMP := $(EE_TOOLSDIR)/bin/ee-objdump
STRIP := $(EE_TOOLSDIR)/bin/ee-strip

ML_INCLUDE := -I$(TOP)/src/ml/include

CCFLAGS := -G0 -O3 -ffreestanding -nostdlib $(ML_INCLUDE) $(USER_CFLAGS)
CXXFLAGS := -G0 -O3 -ffreestanding -fno-rtti -fno-exceptions -nostdlib $(ML_INCLUDE) $(USER_CFLAGS) $(USER_CXXFLAGS)

BINDIR := bin
OBJDIR := obj

# Needed to get the paths to crt(begin|end).o.
EE_LIBDIR=$(EE_TOOLSDIR)/lib/gcc-lib/ee/$(GCC_VERSION)
EE_CRTBEGIN=$(EE_LIBDIR)/crtbegin.o
EE_CRTEND=$(EE_LIBDIR)/crtend.o

# Compile rules.

$(OBJDIR)/%.o: %.c
	$(info Compiling C file $<)
	@if [ ! -d "$(@D)" ]; then mkdir -p $(@D); else exit 0; fi
	$(CC) -c $(CCFLAGS) $< -o $@

$(OBJDIR)/%.o: %.cpp
	$(info Compiling C++ file $<)
	@if [ ! -d "$(@D)" ]; then mkdir -p $(@D); else exit 0; fi
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJDIR)/%.o: %.S
	$(info Assembling $<)
	@if [ ! -d "$(@D)" ]; then mkdir -p $(@D); else exit 0; fi
	$(CXX) -xassembler-with-cpp -c $(CXXFLAGS) $< -o $@
