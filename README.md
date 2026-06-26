# bloom filter cascade

Project from algorithm and datastructures 3 of 3rd bachelor computer science.
A cascade of [bloom filters](https://en.wikipedia.org/wiki/Bloom_filter) is used to categorize elements.

I got a 3.5/4 for this, pretty good :)

You can find my report in dutch [here](extra/verslag.pdf).

## install & usage

### Compile binary

```sh
make build
```

### Use binary

```
$ ./Bloom_filter_cascade --help
Usage: ./Bloom_filter_cascade [train|classify] <arguments>

train:
  Arguments:
    -o <file>    output file
    -a <int>     0: simple algorithm - 1: enhanced algorithm (default = 1)
    <file>       input file (last argument that isn't preceded by a flag)
  Examples:
    ./Bloom_filter_cascade train -o cascade.bfc -a 0 categories.txt
    ./Bloom_filter_cascade train categories.txt -o cascade.bfc

classify:
  Arguments:
    <file>       input file (first argument after 'classify')
  Example:
    ./Bloom_filter_cascade classify cascade.bfc
  Note: to end the classify execution, type :q
```


## make targets

Before running tests, you need to download the test library acutest.h from https://github.com/mity/acutest:
:

```sh
mkdir -p test/include && wget -O test/include/acutest.h https://raw.githubusercontent.com/mity/acutest/refs/heads/master/include/acutest.h
```

### Available targets

| Target                     | Description                                                  |
| -------------------------- | ------------------------------------------------------------ |
| `make`                     | `build` & `test`                        |
| `make debug`               | `build_debug` & `test`                     |
| `make build`               | Compiles the main executable (`Bloom_filter_cascade`)        |
| `make build_debug`         | Compiles main executable with debug flags (`-g`)             |
| `make build_test`          | Compiles the test executable (`run_tests`)              |
| `make test`                | `build_test` & runs tests                                   |
| `make build_test_large`    | Compiles large-scale test executable (`run_large_test`)      |
| `make test_large`          | `build_test_large` & runs large-scale tests                                       |
| `make train_large`         | `build` & trains bloom filter on large dataset      |
| `make train_large_simple`  | `train_large` with simple algorithm (`-a 0`) |
| `make memcheck`            | `test` under Valgrind Memcheck                      |
| `make massif`              | `test` under Valgrind Massif and opens visualizer   |
| `make massif_large`        | `train_large` under Valgrind Massif and opens visualizer               |
| `make massif_large_simple` | `massif_large` with simple algorithm (`-a 0`)                  |
| `make clean`               | Removes compiled binaries                     |