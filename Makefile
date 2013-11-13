.PHONY: all clean check export doc

all:
	mkdir -p build
	cd build && cmake ..
	$(MAKE) -s -C build

clean:
	$(RM) -r build

check: all
	cd build/check && ./run_tests.py

fastcheck: all
	cd build/check && ./run_tests.py -m

norm:
	tools/norm.sh src/

export:
	git archive HEAD --prefix=audebe_r-42sh/ \
	    | bzip2 > audebe_r-42sh.tar.bz2

doc:
	doxygen
