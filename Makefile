.PHONY: all clean check export doc

all:
	mkdir -p build
	cd build && cmake ..
	$(MAKE) -s -C build

clean:
	$(RM) -r build
	$(RM) -r doc/html doc/latex
	$(RM) -r tests/qdunittest/__pycache__
	$(RM) 42sh
	$(RM) audebe_r-42sh.tar.bz2

distclean: clean
	$(RM) common.cmake

check: all
	cd build/tests && ./run_tests.py

fastcheck: all
	cd build/tests && ./run_tests.py -m

coverage: all
	$(MAKE) -C build coverage

readlinecheck: all
	cd build/tests && ./test_readline

norm:
	tools/norm.sh src/

export:
	git archive HEAD --prefix=audebe_r-42sh/ \
	    | bzip2 > audebe_r-42sh.tar.bz2

doc:
	doxygen
