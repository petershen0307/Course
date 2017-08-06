CC ?= gcc
obj = obj
bin = bin
flag = -std=c99

EXEC = $(bin)/server.out

all: $(EXEC)

$(bin)/%.out: $(obj)/%.o
	$(CC) -o $@ $^ -lpthread

$(obj)/%.o: %.c
	$(CC) -c $(flag) -o $@ $^

clean:
	rm $(EXEC)