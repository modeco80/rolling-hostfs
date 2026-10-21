TOP := $(shell pwd)
PATCHSDK := $(TOP)/patchsdk
include mk/ports.mk

.PHONY: all clean

# This contains all the things needed to build a subproject in the tree
SUBMAKE = $(MAKE) V=$(V) TOP=$(TOP) GAME=$(GAME) VERSION=$(VERSION) PATCHSDK=$(PATCHSDK)

define submake
$(info Making project $(1))
$(SUBMAKE) -C $(1) $(2)
endef

define makeall
$(call submake,patchsdk,$(1))
$(call submake,src,$(1))
endef

# Forwarders
all:
	$(call makeall,)
clean:
	$(call makeall,clean)

$V.SILENT:
