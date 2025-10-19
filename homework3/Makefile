CC = gcc
CFLAGS = -Wall -Iinclude `pkg-config --cflags gtk+-3.0`
LIBS = `pkg-config --libs gtk+-3.0`
SRCS = src/scheduler.c src/algorithms.c src/metrics.c src/gui_gtk.c
TARGET = scheduler_gui

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) -o $(TARGET) $(SRCS) $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET) src/*.o
