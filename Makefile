# Programos nustatymai
TARGET       := pomodoro
TITLE        := Pomodoro Timer
AUTHOR       := Volframas
VERSION      := 1.0.0

# Aplankai
export DEVKITPRO ?= /opt/devkitpro
include $(DEVKITPRO)/libnx/switch_rules

# Kompiliatoriaus nustatymai
CXX      := aarch64-none-elf-g++
CXXFLAGS := -g -Wall -O2 -march=armv8-a+crc+crypto -mtune=cortex-a57 -fPIE $(INCLUDE) -I$(TARGET) -I$(DEVKITPRO)/portlibs/switch/include
LDFLAGS  := -specs=$(DEVKITPRO)/libnx/switch.specs -g -march=armv8-a+crc+crypto -mtune=cortex-a57 -Wl,-Map,$(TARGET).map

# Grafinės bibliotekos
LIBS     := -lSDL2_gfx -lSDL2 -lswitch -lpng -ljpeg -lwebp -lz

# Pagrindinės užduotys
all: $(TARGET).nro

$(TARGET).nro: $(TARGET).elf
	@elf2nro $< $@ --title="$(TITLE)" --author="$(AUTHOR)" --version="$(VERSION)"

$(TARGET).elf: main.o
	$(CXX) $(LDFLAGS) -PIE -Wl,--nx $^ $(LIBS) -o $@

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o *.elf *.nro *.map
