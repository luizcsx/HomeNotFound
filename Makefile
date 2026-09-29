# SPDX-License-Identifier: CC0-1.0
#
# Home Not Found - Makefile para BlocksDS (ARM9 + ARM7 padrão do SDK)

BLOCKSDS    ?= /opt/blocksds/core
BLOCKSDSEXT ?= /opt/blocksds/external

# --- Configuração do jogo ---------------------------------------------------

NAME          := home-not-found

GAME_TITLE    := Home Not Found
GAME_SUBTITLE := Get Lewis out.
GAME_AUTHOR   := Open-source homebrew
# GAME_ICON   := icon.bmp   # descomente quando tiver um ícone 32x32 (BMP)

# --- Pastas do projeto (devem ficar dentro deste diretório) -----------------

SOURCEDIRS  := source
INCLUDEDIRS := include
# GFXDIRS    := graphics     # descomente quando houver arte (PNG + .grit)
# AUDIODIRS  := audio        # descomente quando houver áudio (maxmod)
# NITROFSDIR := nitrofs      # descomente quando houver dados em runtime

# --- Makefile padrão do BlocksDS (ROM com ARM7 padrão) ----------------------
# O nome da pasta pode variar entre versões do SDK, então procuramos os
# candidatos conhecidos e usamos o primeiro que existir.

DEFAULT_MAKEFILE := $(firstword $(wildcard \
    $(BLOCKSDS)/sys/default_makefiles/rom_arm9only/Makefile \
    $(BLOCKSDS)/sys/default_makefiles/rom_arm9/Makefile))

ifeq ($(DEFAULT_MAKEFILE),)
$(error Makefile padrão do BlocksDS não encontrado em $(BLOCKSDS)/sys/default_makefiles. Verifique BLOCKSDS)
endif

include $(DEFAULT_MAKEFILE)
