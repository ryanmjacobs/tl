## Options

# Name of executable
EXE?=tl

# Compiler statically? (default is dynamic libs)
STATIC?=no

# Installation prefix
PREFIX?=/usr

# Compiler Options
CC?=gcc
CFLAGS=-c -O2 -Wall -std=c99 -pedantic
DEFINES=-D_DEFAULT_SOURCE

MAN=$(EXE).1
SOURCES=$(shell find src/ -type f -name '*.c')
OBJECTS=$(SOURCES:.c=.o)

ifeq ($(STATIC),yes)
	DEFINES+=-D_TL_STATIC_BUILD="\"yes\""

	# Using a custom build of ffmpeg with:
	#
	#   > git clone git://source.ffmpeg.org/ffmpeg.git ffmpeg
	#   > git checkout release/2.7
	#   > ./configure --prefix=/home/vagrant/builds/usr --enable-libx264 --enable-gpl
	#                 --disable-libopus --disable-vaapi --enable-static --disable-shared
	#   > make && make install
	#
	CFLAGS+=-I/home/vagrant/builds/usr/include
	LDFLAGS=-L/home/vagrant/builds/usr/lib\
			-static -lX11 -lxcb -lXau -lXdmcp\
			/home/vagrant/builds/usr/lib/libavformat.a\
			/home/vagrant/builds/usr/lib/libavcodec.a\
			/home/vagrant/builds/usr/lib/libavfilter.a\
			/home/vagrant/builds/usr/lib/libavutil.a\
			/home/vagrant/builds/usr/lib/libswresample.a\
			/home/vagrant/builds/usr/lib/libswscale.a\
			-lx264 -lm -lz -ldl -lpthread -llzma\
			-static-libgcc

	# If only system libraries worked...
	#LDFLAGS=-static -lX11 -lxcb -lXau -lXdmcp\
		-lx264 -lm -lz -ldl -lpthread -llzma\
		-lavformat -lavcodec -lavfilter -lavutil -lswresample -lswscale\
		-static-libgcc
else
	DEFINES+=-D_TL_STATIC_BUILD="\"no\""

	LDFLAGS=-lX11 -lavformat -lavcodec -lavutil -lswscale -lm
endif

DEFINES+=\
	-D_TL_COMPILE_DATE="\"$(shell date '+%b %d, %Y - %T %Z')\""\
	-D_TL_COMMIT_SHA="\"$(shell git rev-parse --verify HEAD)\""\
	-D_TL_CFLAGS="\"$(CFLAGS)\""\
	-D_TL_LDFLAGS="\"$(LDFLAGS)\""

all: $(SOURCES) $(EXE)

$(EXE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.c.o:
	@echo $(CC) $(CFLAGS) $< -o $@
	@$(CC) $(CFLAGS) $(DEFINES) $< -o $@

install: all
	install -Dm 775 $(EXE) $(PREFIX)/bin/$(EXE)
	install -Dm 644 src/tl.1 $(PREFIX)/share/man/man1/$(MAN)

uninstall:
	@rm -vf $(PREFIX)/bin/$(EXE)
	@rm -vf $(PREFIX)/share/man/man1/$(MAN)

clean:
	@rm -vf $(EXE)
	@find src/ -type f -name '*.o' -exec rm -vf {} \;

.PHONY: clean
