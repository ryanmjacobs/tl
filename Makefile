CC=gcc
CFLAGS=-c -O2 -Wall -std=c99 -pedantic
STATIC=yes

ifeq ($(STATIC),yes)
	# Build ffmpeg w/
	#./configure --prefix=/home/vagrant/builds/usr --enable-libx264 --enable-gpl
	#            --disable-libopus --disable-vaapi --enable-static --disable-shared
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
else
	LDFLAGS=-lX11 -lavformat -lavcodec -lavutil -lswscale -lm
endif

SOURCES=$(shell find src/ -type f -name '*.c')
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=tl
all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(EXECUTABLE)
	@find src/ -type f -name '*.o' -exec rm -vf {} \;

.PHONY: clean
