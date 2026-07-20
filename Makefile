TARGET := pomodoro
BUILD := build
SOURCES := .
INCLUDES := .

include $(DEVKITPRO)/libnx/switch_rules

CFLAGS := -g -Wall -O2 -march=armv8-a+crc+crypto -mtune=cortex-a57 -mtp=soft -fPIE

# Iš čia pašalintas -lSDL2_ttf ir kiti nereikalingi šrifto failai
LIBS := -lswitch -lSDL2_gfx -lSDL2 -lpng -ljpeg -lwebp -lz

export OUTPUT := $(CURDIR)/$(TARGET)
export TOPDIR := $(CURDIR)

SUFX := .nro

all: $(OUTPUT)$(SUFX)

$(OUTPUT).nro: $(OUTPUT).elf
$(OUTPUT).elf: main.o
	$(CC) -g $(LDFLAGS) -PIE -Wl,--nx -mthumb $^ $(LIBS) -o $@
