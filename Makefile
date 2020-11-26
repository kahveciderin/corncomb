build: main
	
tests_build:
	make -C tests
.c:
	$(CC) $(CFLAGS) $@.c $(LDFLAGS) -o $@
	mv $@ ./build/