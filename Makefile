.PHONY: all clean check export

all:
	mkdir -p build
	cd build && cmake ..
	$(MAKE) -C build

clean:
	$(RM) -r build

check: all
	cd check && ./run_tests.py

export:
	git archive HEAD --prefix=audebe_r-42sh/ \
	    | bzip2 > audebe_r-42sh.tar.bz2
