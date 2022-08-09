MAKEFLAGS=--warn-undefined-variables

# -------------------------------------------------------------------------------------------------
.PHONY: all
all: bin/shivver

.PHONY: clean
clean:
	@rm -Rf bin build

FLAGS_LANG	= -std=c11
FLAGS_WARN	= -Wall -Werror
FLAGS_DEBUG	= -g -fsanitize=address
FLAGS_OPT	= -O1
FLAGS_COMPILE	= ${FLAGS_LANG} ${FLAGS_WARN} ${FLAGS_OPT} ${FLAGS_DEBUG}
# FLAGS_DEBUG	=

# -------------------------------------------------------------------------------------------------
shivver_c	= $(shell find main/shivver -name "*.c") $(shell find main/sv -name "*.c")
shivver_h	= $(shell find main/shivver -name "*.h") $(shell find main/sv -name "*.h")
shivver_o	= $(patsubst %.c,build/x86_64/%.o,${shivver_c})

main_c		= $(shell find main/tools/shivver -name "*.c")
main_o		= $(patsubst %.c,build/x86_64/%.o,${main_c})

build/x86_64/main/%.o : main/%.c ${shivver_h}
	@mkdir -p $(dir $@)
	clang ${FLAGS_COMPILE} -Imain -c $< -o $@

bin/shivver: ${main_o} ${shivver_o}
	@mkdir -p $(dir $@)
	clang ${FLAGS_COMPILE} $^ -o $@


# -------------------------------------------------------------------------------------------------
# test sources
test_c			= $(shell find test -name "main.c")
test_exe		= ${patsubst %.c,build/x86_64/%,${test_c}}
test_stdout		= ${patsubst %.c,build/x86_64/%.stdout,${test_c}}
test_stdout_expect	= ${shell find test -name "main.stdout-expect"}
test_stdout_diff	= ${patsubst %.stdout-expect,build/x86_64/%.stdout-diff,${test_stdout_expect}}

# test executables
build/x86_64/test/%: test/%.c ${shivver_o}
	@echo "\n* linking $@"
	@mkdir -p $(dir $@)
	clang ${FLAGS_COMPILE} -Imain ${shivver_o} $< -o $@
	@echo


# run test and save output
build/x86_64/test/%.stdout: build/x86_64/test/%
	@echo "* test $<"
	@$< > $@


# compute diff between actual output and expected
build/x86_64/test/%.stdout-diff: build/x86_64/test/%.stdout
	$(eval RESULT = $(shell diff test/$*.stdout-expect $< > $@; echo $$?))

	@if test ${RESULT} -eq 1; then \
		echo "! failed: test/$*.stdout-expect"; \
		cat $@; \
		rm $@; \
	fi


# update expected test output
test/%.stdout-expect: build/x86_64/test/%.stdout
	cp $< $@


.PHONY: test
test: ${test_exe} ${test_stdout} ${test_stdout_diff}
