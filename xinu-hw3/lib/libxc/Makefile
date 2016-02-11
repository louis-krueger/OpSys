#
#  Make the Xinu version of the C run-time support library
#

include ../../compile/makeVars

INCLUDE	=	-I../../include

CFLAGS   = -Os -Wall -nostdinc -fno-builtin -Wstrict-prototypes \
	   -Wno-trigraphs -fno-strict-aliasing -fno-common \
	   -fomit-frame-pointer -G 0 -mno-abicalls -fno-pic \
	   -ffunction-sections -mlong-calls \
	   -mabi=32 -march=mips32 -Wa,-32 -Wa,-march=mips32 \
	   -Wa,-mips32 -Wa,--trap -c ${INCLUDE}

LIBDIR	=	..


.s.o:
		$(AS) -o $@ $(AFLAGS) $<

.c.o:
		$(CC) $(CFLAGS) $<

CFILES	=	abs.c atoi.c atol.c bzero.c ctype_.c doprnt.c doscan.c \
		fgets.c fprintf.c fputs.c fscanf.c 	memcmp.c memcpy.c qsort.c \
		rand.c sprintf.c sscanf.c strcat.c strchr.c strcmp.c strcpy.c \
		strlen.c strncat.c strncmp.c strncpy.c

OFILE2 = ${CFILES:%.c=%.o}
OFILES = ${OFILE2:%.s=%.o}

all:		libxc.a

libxc.a:	${OFILES}
		rm -f libxc.a
		${AR} cr libxc.a $(OFILES)

install:	libxc.a
		cp libxc.a $(LIBDIR)/libxc.a

clean:
		rm -f ${OFILES} libxc.a $(LIBDIR)/libxc.a
