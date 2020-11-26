SRCS = $(wildcard *.cmb)
PROGS = $(patsubst %.cmb,%.cmbx,$(SRCS))
all_tests: $(PROGS)
%.cmbx: %.cmb
	../assembler.py $< $@
	mv $@ ./build