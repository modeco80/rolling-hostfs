GAME := rolling
REGION ?= pal

GCC_VERSION ?= 2.96-ee-001003-1

# US version
GAME_rolling_pal_VALID = yes

# sanity check
ifneq ($(GAME_$(GAME)_$(REGION)_VALID),yes)
$(error The game/version combination $(GAME)/$(REGION) is currently not supported.)
endif
