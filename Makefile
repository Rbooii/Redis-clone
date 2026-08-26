.PHONY: all full
all:
	cmake --build build
full:
	cmake -B build
	cmake --build build