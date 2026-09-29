BLOCKSDS    ?= /opt/blocksds/core
BLOCKSDSEXT ?= /opt/blocksds/external


NAME          := home-not-found

GAME_TITLE    := HOME NOT FOUND
GAME_SUBTITLE := Luiz Miguel
GAME_AUTHOR   := Open-source homebrew
# GAME_ICON   := icon.bmp


SOURCEDIRS  := source
INCLUDEDIRS := include
BINDIRS     := data
# GFXDIRS    := graphics 
# AUDIODIRS  := audio
# NITROFSDIR := nitrofs

DEFAULT_MAKEFILE := $(firstword $(wildcard \
    $(BLOCKSDS)/sys/default_makefiles/rom_arm9only/Makefile \
    $(BLOCKSDS)/sys/default_makefiles/rom_arm9_only/Makefile \
    $(BLOCKSDS)/sys/default_makefiles/rom_arm9/Makefile))

ifeq ($(DEFAULT_MAKEFILE),)
$(error Makefile padrão do BlocksDS não encontrado em $(BLOCKSDS)/sys/default_makefiles. Verifique BLOCKSDS)
endif

include $(DEFAULT_MAKEFILE)
