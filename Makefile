MKFILE_PATH=$(dir $(realpath $(firstword $(MAKEFILE_LIST))))

CLANG=${LLVM_BUILD_DIR}/bin/clang
LLVMDIS=${LLVM_BUILD_DIR}/bin/llvm-dis

ERRORLIBS=error_lib_1.bc error_lib_empty.bc
EXAMPLES=npo2_with_checks_1.bc npo2_with_checks_2.bc tcas_without_checks.bc tcas_with_checks.bc

all : $(ERRORLIBS) $(EXAMPLES)

%.bc : error_lib/%.c
	${CLANG} -emit-llvm -c -g -Iklee/include/klee -I$(MKFILE_PATH)/error_lib $? -o $@

%.bc : examples/%.c
	${CLANG} -emit-llvm -c -g -Iklee/include/klee -I$(MKFILE_PATH)/error_lib $? -o $@

dis : all
	for f in *.bc; do ${LLVMDIS} $$f; done

locs : all
	for f in *.bc; do ${LLVMDIS} $$f; done
	for f in *.ll; do wc -l $$f; done
	for f in examples/*.c; do wc -l $$f; done

clean:
	rm -rf *.bc *.ll klee-out-* klee-last

