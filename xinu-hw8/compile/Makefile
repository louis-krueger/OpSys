#
#  Makefile for MIPS(EL) XINU.
#

include makeVars

BOOTIMAGE = xinu.boot

LIB       = ../lib

LDFLAGS   = -static --gc-sections -no-warn-mismatch -T ld.script

$(VERBOSE).SILENT:

INDENT = indent
INDENT_TYPES = -T uchar -T ushort -T uint -T ulong -T bool -T message    \
	       -T irqmask -T semaphore -T mailbox -T qid_type -T tid_typ \
	       -T syscall -T devcall -T shellcmd -T thread -T interrupt

INDENT_ARGS = -npro -kr -bl -blf -bls -bli0 -nce -ncs -nlps -l74 -lc74 -nut
INDENT_FILES = $(shell find ../ -name '*.[ch]')

#-----------------------------------------------------------------------
# source files
#-----------------------------------------------------------------------

# SHELL
SHLL = xsh_cat.c xsh_delete.c xsh_diskstat.c xsh_test.c shell-commands.c
NET  = netInit.c

# FILE system
FSYS = fileClose.c   fileGetChar.c  fileSeek.c fileCreate.c  fileOpen.c \
	sbFreeBlock.c  sbInit.c fileDelete.c  filePutChar.c  sbGetBlock.c   \
		sbOpen.c

SRC =	${SHLL:%.c=../shell/%.c} \
	${FSYS:%.c=../file/%.c} \
	${NET:%.c=../network/%.c}

KRNOBJ = ${SRC:%.c=%.o}

#-----------------------------------------------------------------------
# make targets
#-----------------------------------------------------------------------
all: ${BOOTIMAGE} 

${BOOTIMAGE}: ${LIB}/xinu-os.a ${KRNOBJ} ld.script ${LIB}/libxc.a 
	@echo -e "\tLinking" $@
	${LD} -o $@ ${LDFLAGS} --whole-archive ${LIB}/xinu-os.a ${KRNOBJ} --whole-archive ${LIB}/libxc.a

xinu-disk: ../device/disk/xinu-disk.c
	@echo -e "\tCompiling" $@
	gcc -o xinu-disk ../device/disk/xinu-disk.c


%.o: %.c
	@echo -e "\tCompiling" $@
	$(CC) $(CFLAGS) -o $@ $<

%.o: %.S
	@echo -e "\tAssembling" $@
	$(CC) $(CFLAGS) -o $@ $<

clean:
	@echo -e "\tCleaning all objects"
	rm -f *.o ${KRNOBJ} ${BOOTIMAGE}

depclean:
	@echo -e "\tCleaning dependencies"
	rm -f .deps
	touch .deps

realclean: clean depclean
	@echo -e "\tCleaning EVERYTHING"
	rm -f vn version
	rm -f Makefile.bak
	rm -f xinu-disk

indent:
	${INDENT} ${INDENT_ARGS} ${INDENT_TYPES} ${INDENT_FILES}
	rm -f ${INDENT_FILES:%=%~}

depend:
	@echo -e "\tBuilding dependencies"
	${MAKEDEP} -I../include $(LDRASM) $(SYSASM) $(SRC) > .deps

#-----------------------------------------------------------------------
# targets for files outside of this directory
#-----------------------------------------------------------------------

include .deps
