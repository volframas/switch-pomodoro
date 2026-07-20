# Patikriname devkitPro aplinką
ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment")
endif

include $(DEVKITPRO)/libnx/switch_rules

TARGET      := pomodoro
TITLE       := Pomodoro Timer
AUTHOR      := Volframas
VERSION     := 1.0.0

# Nurodome kompiliatoriui, kur ieškoti bibliotekų (suras switch.h)
INCLUDE     := -I$(DEVKITPRO)/libnx/include -I$(DEVKITPRO)/portlibs/switch/include
LIBDIRS     := -L$(DEVKITPRO)/libnx/lib -L$(DEVKITPRO)/portlibs/switch/lib

# Sujungiame grafines dalis
LIBS        := -lSDL2_gfx -lSDL2 -lswitch -lpng -ljpeg -lwebp -lz

export CFLAGS   := -g -Wall -O2 -march=armv8-a+crc+crypto -mtune=cortex-a57 -fPIE $(INCLUDE)
export CXXFLAGS := $(CFLAGS)
export LDFLAGS  := -specs=$(DEVKITPRO)/libnx/switch.specs -g -march=armv8-a+crc+crypto -mtune=cortex-a57 $(LIBDIRS)

.PHONY: all clean

all: $(TARGET).nro

$(TARGET).nro: $(TARGET).elf
	@elf2nro $< $@ --title="$(TITLE)" --author="$(AUTHOR)" --version="$(VERSION)"

$(TARGET).elf: main.o
	$(CXX) $(LDFLAGS) -PIE -Wl,--nx $^ $(LIBS) -o $@

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o *.elf *.nro *.map
