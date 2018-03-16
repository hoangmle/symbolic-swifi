Symbolic Software-Implemented Fault Injection

# Build instructions

1. Clone the repository recursively with `git clone --recursive`, otherwise you need to execute `git submodule update --init` after cloning to get our extended version of KLEE in the subfolder `klee`

2. Build the extended version of KLEE following the instructions at http://klee.github.io/build-llvm34

3. Set the environment variable `LLVM_BUILD_DIR` to the LLVM build used to build KLEE

4. Execute `make` to build the LLVM bitcode of the error lib and the examples (e.g. `tcas_with_checks.bc`)

# KLEE options for experiments

In the following, it is assumed that the KLEE executables `klee` and `ktest-tool` can be found in your PATH.
They are typically in the subfolder `klee/build/bin`.

## Error-free execution

`klee -only-replay-seeds=0 -optimize=0 -link-llvm-lib=error_lib_empty.bc -inject-bit-error -output-module -emit-all-errors=1 -output-global-vars=error_counter ${bitcode-file}`

Each produced ktest file will include the value of error_counter (= number of injection locations on that path).
Add option `-output-dir=<string>` to generate ktest files in the given directory (enable reuse with symbolic fault injection).
Without that option, the ktest files and some other KLEE stats can be found in the subfolder `klee-last`.

## Execution with symbolic fault injection

### Basic command
`klee -only-replay-seeds=0 -optimize=0 -link-llvm-lib=error_lib_1.bc -inject-bit-error -output-module -emit-all-errors=1 -no-output-on-peaceful-exit ${bitcode-file}`

### Seeding mode
`klee -seed-out=${ktest-file} -only-replay-seeds=1 -named-seed-matching=strict -allow-seed-extension -allow-seed-patching=0 -optimize=0 -link-llvm-lib=error_lib_1.bc -inject-bit-error -output-module -emit-all-errors=1 -seed-forcing-mode=cex -no-output-on-peaceful-exit ${bitcode-file}`

### Additional options
`-silent-klee-assume`, `-check-overshift=0`, `-silent-out-of-bound-pointer-error` to enable KLEE-builtin checks

## Print ktest files

`for t in klee-last/*.ktest; do ktest-tool --write-ints --single-line-object $t; done`
