TARGET = psp-post-it
OBJS = main.o blit.o post_it.o font.o ./cJSON/cJSON.o

BUILD_PRX = 1
PRX_EXPORTS = exports.exp

INCDIR = 
CFLAGS = -O2 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR =

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak