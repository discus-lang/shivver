MAKEFLAGS=--warn-undefined-variables

# -------------------------------------------------------------------------------------------------
.PHONY: all
all: bin/shivver

.PHONY: clean
clean:
	@rm -Rf bin build

FLAGS_LANG	= -std=c11
FLAGS_WARN	= -Wall -Werror
FLAGS_OPT	= -O1
FLAGS_DEBUG	= -g -fsanitize=address
# FLAGS_DEBUG	=
FLAGS_COMPILE	= ${FLAGS_LANG} ${FLAGS_WARN} ${FLAGS_OPT} ${FLAGS_DEBUG}

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
test_stdout_expected	= ${shell find test -name "main.stdout-expected"}
test_stdout_diff	= ${patsubst %.stdout-expect,build/x86_64/%.stdout-diff,${test_stdout_expected}}

# test executables
build/x86_64/test/%: test/%.c ${shivver_o}
	@mkdir -p $(dir $@)
	@rm -f $@.stdout
	@rm -f $@.stdout-diff
	clang ${FLAGS_COMPILE} -Imain ${shivver_o} $< -o $@


# run test and save output
build/x86_64/test/%.stdout: build/x86_64/test/%
	@echo "* test:       $<"
	$(eval RESULT = $(shell $< > $@ 2> $<.stderr; echo $$?))

	@if test ${RESULT} -ne 0; then \
		echo "! failed $<\n"; \
		echo "-- stderr ---------------------------"; \
		cat $<.stderr; \
		echo "-------------------------------------"; \
		rm -f $@; \
		rm -f $@-diff; \
		exit 1; \
	fi


# compute diff between actual output and expected
build/x86_64/test/%.stdout-diff: build/x86_64/test/%.stdout
	$(eval RESULT = $(shell diff test/$*.stdout-expected $< > $@; echo $$?))

	@if test ${RESULT} -eq 1; then \
		echo "! unexpected output"; \
		echo "  expected: test/$*.stdout-expected"; \
		echo "  actual:   $<"; \
		echo "  diff :    $@"; \
		echo ""; \
		cat $@; \
		rm $@; \
	fi


.PHONY: test
test: ${test_exe} ${test_stdout} ${test_stdout_diff}
