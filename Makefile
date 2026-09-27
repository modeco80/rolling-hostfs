TOP := $(shell pwd)
include mk/ports.mk

.PHONY: all clean

# This contains all the things needed to build a subproject in the tree
SUBMAKE = $(MAKE) V=$(V) TOP=$(TOP) GAME=$(GAME) VERSION=$(VERSION)

define submake
$(info Making project $(1))
$(SUBMAKE) -C $(1) $(2)
endef

define makeall
$(call submake,src/ml/lib/hw,$(1))
$(call submake,src/ml/lib/core,$(1))
$(call submake,src/ml/lib/cxx,$(1))
$(call submake,src/utils,$(1))
$(call submake,src/hostfs,$(1))
endef

# Forwarders
all:
	$(call makeall,)
clean:
	$(call makeall,clean)

$V.SILENT:
