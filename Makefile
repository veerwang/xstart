export PROJ     :=
export DEBUG    :=
export EXEC     := main_$(PROJ)
###########################################################
#     Select which cross compiler tools to be used
#     Selection: armv41,armv42,myarm or x86; 
#     default value is x86
export COMPILE  := x86
ifeq ($(COMPILE),armv41)
	CROSS := /opt/host/armv3l/bin/armv4l-unknown-linux-
	EXOBJS := etio-4.0.1.o
endif
ifeq ($(COMPILE),armv42)
	CROSS := /opt/host/arm40_new/bin/arm-unknown-linux-gnu-
	EXOBJS := etio-4.0.1.o
endif
ifeq ($(COMPILE),myarm)
	CROSS := /opt/host/arm-kevin-linux-gnueabi/bin/arm-kevin-linux-gnueabi-
	EXOBJS := etio-4.3.2.o
endif
ifeq ($(COMPILE),x86)
	CROSS ?=
endif
CROSS ?=
###########################################################
export MAKE_DIR := $(shell pwd)
export PARENTS_DIR := $(dir $(MAKE_DIR))
export DIR_NAME    := $(notdir $(MAKE_DIR))
export INST_DIR     := /media/disk/et/
export INST_NAME    := main 
export EXOBJS
###########################################################
export CROSS
export CC := $(CROSS)gcc
export CPP := $(CROSS)g++
export AS := $(CROSS)as
export RM := rm
export AR := $(CROSS)ar
export OBJCP := $(CROSS)objcopy
export STRIP := $(CROSS)strip
export FLAG := -Wall
export LIBS := pthread dl
###########################################################
MAKEFLAG := -s -j2
.PHONY : all
all:
ifeq ($(PROJ),)
	$(make_msg)
else
	@cd $(MAKE_DIR)/src;$(MAKE) $(MAKEFLAG) $(MAKECMDGOALS)
	@cp $(MAKE_DIR)/src/$(PROJ)/$(EXEC) $(MAKE_DIR)/bin/$(EXEC)
ifeq ($(DEBUG),yes)
else
	@$(STRIP) $(MAKE_DIR)/bin/$(EXEC)
endif

endif
.PHONY : clean 
clean:
	@set -e;cd src;$(MAKE) $(MAKEFLAG) $(MAKECMDGOALS)
	@$(RM) -f $(MAKE_DIR)/bin/$(EXEC)
.PHONY : install 
install: 
	@echo Copy  $(MAKE_DIR)/bin/$(EXEC) to $(INST_DIR)$(INST_NAME)
	@cp $(MAKE_DIR)/bin/$(EXEC) $(INST_DIR)$(INST_NAME)
.PHONY : tar 
tar: 
	@echo tar the Smart serial project 
	@cd $(PARENTS_DIR);tar -cjf $(DIR_NAME).tar.bz2 $(DIR_NAME);cd $(MAKE_DIR) 
###########################################################
define make_msg
@echo " "
@echo Usage:
@echo "        make PROJ=project COMPILE=tool DEBUG=yes/(others)"
@echo "        make PROJ=project clean"
@echo " "
endef
