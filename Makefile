.PHONY: all clean check export doc

all:
	mkdir -p build
	cd build && cmake ..
	$(MAKE) -s -C build

clean:
	$(RM) -r build
	$(RM) common.cmake
	$(RM) -r doc/html doc/latex

check: all
	cd build/check && ./run_tests.py

fastcheck: all
	cd build/check && ./run_tests.py -m

readlinecheck: all
	cd build/check && ./run_readline_test "Test.$$ "

norm:
	tools/norm.sh src/

export:
	git archive HEAD --prefix=audebe_r-42sh/ \
	    | bzip2 > audebe_r-42sh.tar.bz2

doc:
	doxygen
