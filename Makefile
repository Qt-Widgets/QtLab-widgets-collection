all: build

build: FORCE
	make -C build
	cp build/compile_commands.json ./

prepare:
	-mkdir build
	cd build && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..

run: FORCE
	cd build && ctest -V -R $(E)

Makefile:;

#%:: FORCE
#	make -C build $@

FORCE:

.PHONY: FORCE all
