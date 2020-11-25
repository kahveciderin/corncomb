build: main
	
.c:
	$(CC) $(CFLAGS) $@.c $(LDFLAGS) -o $@
	mv $@ ./build/