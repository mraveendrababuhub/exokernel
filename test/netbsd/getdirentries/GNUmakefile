
TOP = ../../..
PROG = getdirentries
MAINSRC = getdirentries.c
SRCFILES = $(MAINSRC) # nfs_getdirentries.c open.c namei.c

EXTRAINC = -I$(TOP)/lib/libc 
EXTRAINC += -I$(TOP)/lib/libc/fd/nfs
EXTRAINC += -I$(TOP)/lib/libc/fd

VPATH+= $(TOP)/lib/libc/fd/nfs
VPATH+= $(TOP)/lib/libc/fd

export DOINSTALL=yes
export INSTALLPREFIX=test

include $(TOP)/GNUmakefile.global

EXTRACLN += unix

unix: $(MAINSRC)
	gcc -o unix $(MAINSRC)
