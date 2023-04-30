TARGET = PPi_by_pyroesp
OBJS = main.o PPi.o snprintf.o
USE_KERNEL_LIBC = 1
USE_KERNEL_LIBS = 1
INCDIR =
LDFLAGS =

CFLAGS = -DNOEXIT -DFPM_MIPS -O2 -G0 -Wall -fno-pic
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)
LIBDIR =
LIBS = -lpsprtc -lpspctrl -lpspdisplay
PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build_prx.mak
LIBS += -lpspge_driver -lpspgu