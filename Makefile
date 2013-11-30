CPPCHECK=$(shell hash cppcheck 1>&2 2>/dev/null; echo $$?)
.PHONY: all clean check export doc cppcheck

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
	$(RM) doc/README.md

distclean: clean
	$(RM) common.cmake

check: all
	cd build/tests && ./run_tests.py

fastcheck: all
	cd build/tests && ./run_tests.py -m

fastfail: all
	cd build/tests && ./run_tests.py -m --failfast

coverage: all
	$(MAKE) -C build coverage

readlinecheck: all
	cd build/tests && ./test_readline

norm:
	tools/norm.sh src/

cppcheck:
ifeq (${CPPCHECK}, 0)
	mkdir -p build
	cppcheck --enable=all --xml src/ 2> build/cppcheck.xml
endif

export:
	git archive HEAD --prefix=audebe_r-42sh/ \
	    | bzip2 > audebe_r-42sh.tar.bz2

doc:
	@ln README doc/README.md
	doxygen
