CC ?= gcc
obj = obj
bin = bin

EXEC = $(bin)/quiz1.out $(bin)/quiz2.out $(bin)/quiz3.out

all: $(EXEC)

$(bin)/%.out: $(obj)/%.o
	$(CC) -o $@ $^ -lpthread

$(obj)/%.o: %.c
	$(CC) -c -o $@ $^

clean:
	rm $(EXEC)