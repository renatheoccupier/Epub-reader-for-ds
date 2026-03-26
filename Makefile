#-------------------------------------------------------------------------------
.SUFFIXES:
#-------------------------------------------------------------------------------
ifeq ($(strip $(DEVKITARM)),)
$(error "Set DEVKITARM in your environment.")
endif

include $(DEVKITARM)/ds_rules

export TARGET		:=	$(shell basename $(CURDIR))
export TOPDIR		:=	$(CURDIR)
export PATH			:=	$(DEVKITARM)/bin:$(PATH)

GAME_TITLE := Rena
GAME_SUBTITLE1 := App to read epub file with image
GAME_SUBTITLE2 := SSdtIHJlbmEsIGJ0dyBhcmUgeW91IHVuZW1wbG95ZWQ/
GAME_ICON := data/icon-nds.bmp

#-------------------------------------------------------------------------------
# path to tools
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Platform overrides.
#-------------------------------------------------------------------------------
##include Makefile.$(shell uname)

.PHONY: checkarm7 checkarm9 clean

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
all: $(TARGET).nds

#---------------------------------------------------------------------------------
checkarm7:
	@true

#---------------------------------------------------------------------------------
checkarm9:
	$(MAKE) -C arm9

#-------------------------------------------------------------------------------
$(TARGET).nds: arm9/$(TARGET).elf
	$(SILENTCMD)ndstool -c sandbox/$(TARGET).nds -7 $(CALICO)/bin/ds7_maine.elf -9 arm9/arm9.elf \
		-b $(GAME_ICON) "$(GAME_TITLE);$(GAME_SUBTITLE1);$(GAME_SUBTITLE2)"

	$(SILENTCMD)ndstool -c sandbox/$(TARGET).dsi -7 $(CALICO)/bin/ds7_maine.elf -9 arm9/arm9.elf \
		-b $(GAME_ICON) "$(GAME_TITLE);$(GAME_SUBTITLE1);$(GAME_SUBTITLE2)" \
		-g IKUR 01 "IKUREADER" $(VERSION) -z 80040000 -u 00030004 

	echo built ... $(notdir $@)

#-------------------------------------------------------------------------------
#(TARGET).arm7		: arm7/$(TARGET).elf
#$(TARGET).arm9		: arm9/$(TARGET).elf

#-------------------------------------------------------------------------------
arm7/$(TARGET).elf:
	@true

#-------------------------------------------------------------------------------
arm9/$(TARGET).elf:
	$(MAKE) -C arm9

#-------------------------------------------------------------------------------
clean:
	$(MAKE) -C arm9 clean
	rm -f sandbox/$(TARGET).nds
	rm -f sandbox/$(TARGET).dsi
