TARGET = skysphere
OBJS = skysphere.o main.o

#To build for custom firmware:
BUILD_PRX = 1
PSP_FW_VERSION=371

CFLAGS = -O2 -g -G0 -Wall -fverbose-asm -save-temps
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)
LIBDIR =

MYLIBS=
STDLIBS= -losl -lpng -lz \
         -lpsprtc -lpsphprm -lpspsdk -lpspctrl -lpspumd -lpsppower -lpspgum -lpspgu  -lpspaudiolib -lpspaudio -lpsphttp -lpspssl -lpspwlan \
         -lpspnet_adhocmatching -lpspnet_adhoc -lpspnet_adhocctl -lm -ljpeg
LIBS=$(STDLIBS) $(MYLIBS)

LDFLAGS =
EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Skysphere
#PSP_EBOOT_ICON = ICON0.PNG
PSPSDK=$(PSPDEV)/psp/sdk
include $(PSPSDK)/lib/build.mak
