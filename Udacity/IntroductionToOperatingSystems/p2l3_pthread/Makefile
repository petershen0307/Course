CC ?= gcc
obj = obj
bin = bin
flag = -std=c99

EXEC = $(bin)/quiz1.out $(bin)/quiz2.out $(bin)/quiz3.out $(bin)/p_c.out

all: $(EXEC)

$(bin)/%.out: $(obj)/%.o
	$(CC) -o $@ $^ -lpthread

$(obj)/%.o: %.c
	$(CC) -c $(flag) -o $@ $^

clean:
	rm $(EXEC)