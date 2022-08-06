# base64
Unix command style base64 encoder/decoder

## License
This is free and unencumbered software released into the public domain.

## Usage
```
Usage: base64 [OPTION]... [FILE]...
Encode or decode base64 from each FILE to standard output.

With no FILE, or when FILE is -, read standard input.

  -e, --encode		encode data to base64
              		  This is the default if neither 
  -d, --decode		decode base64 to data
  -p, --padding		use padding when encoding data to base64
  -h, --help        show this help message
```

## Building, Cleaning, Testing
The script `run.sh` is used to build, clean, and test `base64`.

### Usage:

```
[ENV_VARIABLES] ./run.sh [SUBCOMMAND]...
```

#### Environment Variables
`run.sh` uses specific environment variables to control certain options:

- `BUILD_DIR`: The output build directory for building and test logs
- `MAKE`: whether to use `cmake` or `make`

#### Subcommands

Subcommands are executed in order

- build: build the project
- clean: clean
- bear: clean and generate `compile_commands.json` with `bear`
- test: test using the test script `test/run_tests.py` on tests in `test/files/**`

## The `run_tests.py` Test Script
Run tests contained in a `files` folder in the same directory as `run_tests.py`.
Each test file has 3 sections, separated by lines with only two semicolons:
The first section are the command line options to pass to `base64`.
The second and third sections are the test input and expected output, respectively.
Note that leading and trailing whitespace is ignored for each line.

For example, to create an encoding test, with padding for the string "Man":


```
-en
;;
Man
;;
TWFu
```
