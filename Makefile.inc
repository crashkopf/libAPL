%.o: %.c *.h
	$(CC) $(CFLAGS) -o $@ $<

all: $(OBJS)
	
.PHONY: clean

clean:
	rm -f $(OBJS)
