TXTOBJS:=objs
TXTDEPS:=deps
OBJS_DIR:=./$(TXTOBJS)/
DEPS_DIR:=./$(TXTDEPS)/
SRCS:=$(wildcard *.cpp *.c *.s *.jpg)
OBJS:=$(SRCS:=.o)
DEPS:=$(SRCS:=.dep)
OBJS:=$(addprefix $(OBJS_DIR),$(OBJS))
DEPS:=$(addprefix $(DEPS_DIR),$(DEPS))
ifeq ($(DEBUG),yes)
COMPILE_FLAG = -g2
else
COMPILE_FLAG =
endif
all: $(DEPS) $(EXEC) 
$(DEPS_DIR)%.cpp.dep: %.cpp
	@set -e; \
	$(RM) -rf $@.tmp; \
	gcc -E -MM $^ > $@.tmp; \
 	sed 's,\(.*\)\.o[ :]*,'$(TXTOBJS)'/\1.cpp.o:,g' < $@.tmp > $@; \
	$(RM) $@.tmp
$(DEPS_DIR)%.c.dep: %.c
	@set -e; \
	$(RM) -rf $@.tmp; \
	gcc -E -MM $^ > $@.tmp; \
 	sed 's,\(.*\)\.o[ :]*,'$(TXTOBJS)'/\1.c.o:,g' < $@.tmp > $@; \
	$(RM) $@.tmp
$(DEPS_DIR)%.s.dep: %.s
	@set -e; \
	$(RM) -rf $@.tmp; \
	echo $@ : $^ > $@.tmp; \
	sed 's,\(.*\)\.s.dep[ :]*,\1.s.o:,g' < $@.tmp > $@; \
 	sed 's/'$(TXTDEPS)/$(TXTOBJS)'/g' < $@.tmp > $@; \
	$(RM) $@.tmp
$(DEPS_DIR)%.jpg.dep: %.jpg
	@set -e; \
	$(RM) -rf $@.tmp; \
	echo $@ : $^ > $@.tmp; \
	sed 's,\(.*\)\.jpg.dep[ :]*,\1.jpg.o:,g' < $@.tmp > $@; \
 	sed 's/'$(TXTDEPS)/$(TXTOBJS)'/g' < $@.tmp > $@; \
	$(RM) $@.tmp
-include $(DEPS)
$(EXEC): $(OBJS)
	$(CPP) $(FLAG) -o $@ $^ $(addprefix -l,$(LIBS))
$(addprefix $(OBJS_DIR),%.s.o): %.s
	$(AS) $< -o $@
$(addprefix $(OBJS_DIR),%.c.o): %.c
	$(CC) $(COMPILE_FLAG) -c $< -o $@
$(addprefix $(OBJS_DIR),%.cpp.o): %.cpp
	$(CPP) $(COMPILE_FLAG) -c $< -o $@
$(addprefix $(OBJS_DIR),%.jpg.o): %.jpg
	$(OBJCP) -I binary -O elf32-littlearm -B arm $< $@
.PHONY: clean
clean:
	$(RM) -rf $(addprefix $(OBJS_DIR),*.o) $(addprefix $(DEPS_DIR),*.dep) $(EXEC) 
