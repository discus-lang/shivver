
MAKEFLAGS=--warn-undefined-variables

.PHONY: all
all: bin/shivver

.PHONY: clean
clean:
	rm -Rf bin build

FLAGS_LANG	= -std=c11
FLAGS_WARN	= -Wall -Werror
FLAGS_DEBUG	= -g -fsanitize=address
FLAGS_OPT	= -O1
FLAGS_COMPILE	= ${FLAGS_LANG} ${FLAGS_WARN} ${FLAGS_OPT} ${FLAGS_DEBUG}
# FLAGS_DEBUG	=

# -------------------------------------------------------------------------------------------------
shivver_c	= $(shell find main/shivver -name "*.c") $(shell find main/sv -name "*.c")
shivver_h	= $(shell find main/shivver -name "*.h") $(shell find main/sv -name "*.h")
shivver_o	= $(patsubst %.c,build/main/x86_64/%.o,${shivver_c})

main_c		= $(shell find main/tools/shivver -name "*.c")
main_o		= $(patsubst %.c,build/main/x86_64/%.o,${main_c})

build/main/x86_64/%.o : %.c ${shivver_h}
	@mkdir -p $(dir $@)
	clang ${FLAGS_COMPILE} -Imain -c $< -o $@

bin/shivver: ${main_o} ${shivver_o}
	@mkdir -p $(dir $@)
	clang ${FLAGS_COMPILE} $^ -o $@


# -------------------------------------------------------------------------------------------------
# test sources
test_c		= $(shell find test -name "main.c")
test_exe	= ${patsubst %.c,build/test/x86_64/%,${test_c}}
test_stdout	= ${patsubst %.c,build/test/x86_64/%.stdout,${test_c}}

# test executables
build/test/x86_64/%: %.c ${shivver_o}
	@mkdir -p $(dir $@)
	clang ${FLAGS_COMPILE} -Imain ${shivver_o} $< -o $@

# test outputs
build/test/x86_64/%.stdout: build/test/x86_64/%
	$< > $@

.PHONY: test
test: ${test_stdout} ${test_exe}
