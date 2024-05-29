CC = gcc
CFLAGS = -Wall -Wextra -std=c99
GTKFLAGS = `pkg-config --cflags --libs gtk+-3.0`

SRCS = src/main.c src/orders.c src/display_orders.c src/login.c
OBJS = $(SRCS:.c=.o)
TARGET = src/order_management

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(GTKFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< $(GTKFLAGS)

clean:
	rm -f $(OBJS) $(TARGET)
