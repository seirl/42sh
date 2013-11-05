.PHONY: all clean export

all:
	mkdir -p build
	cd build && cmake ..
	$(MAKE) -C build

clean:
	$(RM) -r build

export:
	git archive HEAD --prefix=audebe_r-42sh/ \
	    | bzip2 > audebe_r-42sh.tar.bz2
