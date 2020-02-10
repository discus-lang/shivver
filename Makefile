MAKEFLAGS=--warn-undefined-variables

.PHONY: all
all: bin/shivver

.PHONY: clean
clean:
	rm -Rf bin build


sources_shivver_c	= $(shell find src/shivver -name "*.c")
sources_shivver_h	= $(shell find src/shivver -name "*.h")

sources_main_c		= $(shell find src/tools -name "*.c")

objects_shivver		= $(patsubst %.c,build/x86_64/%.o,${sources_shivver_c})
objects_main		= $(patsubst %.c,build/x86_64/%.o,${sources_main_c})

build/x86_64/%.o : %.c ${sources_shivver_h}
	@mkdir -p $(dir $@)
	clang -std=c99 -Wall -g -Isrc -c $< -o $@

bin/shivver: ${objects_main} ${objects_shivver}
	@mkdir -p $(dir $@)
	clang -std=c99 $^ -o $@

