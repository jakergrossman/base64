#!/usr/bin/env python3

import subprocess
import os
import glob
import sys
import logging
from time import strftime, gmtime

def parse_input(file):
    with open(file) as f:
        data = [l.strip() for l in f.read().split(';;')]

        return ( data[0].split('\\s'), data[1], data[2] )

def run_test(options, input, output):
    test = parse_input(file)

    result = subprocess.run([os.path.join(script_dir, '..', 'build', 'base64')]+options, stdout=subprocess.PIPE,
                            input=bytes(input, encoding='utf8'))

    decoded = result.stdout.decode('utf8')

    return (decoded == output, decoded)

def log(string, print_file=sys.stdout):
    print(string, file=print_file)
    if print_file == sys.stdout:
        logging.info(string)
    elif print_file == sys.stderr:
        logging.error(string)

script_dir = os.path.dirname(os.path.relpath(__file__))
build_dir  = os.environ.get('BUILD_DIR')
log_dir    = build_dir if build_dir else os.path.join(script_dir, '..', 'build')

timestamp  = strftime('%d-%m-%Y_%H:%M:%S', gmtime())
log_file   = os.path.join(log_dir, f'test_log_{timestamp}.txt')

test_files = glob.glob("files/**/*.txt", recursive=True, root_dir=script_dir)
test_files = [os.path.join(script_dir, file) for file in test_files]

logging.basicConfig(filename=log_file, format='%(levelname)s:%(asctime)s %(message)s', level=logging.DEBUG)

for file in test_files:
    options, test_input, expected_output = parse_input(file)
    pass_result, test_output = run_test(options, test_input, expected_output)

    basename  = os.path.basename(file)
    padding   = 40 - len(basename)
    indicator = ('SUCCESS' if pass_result else 'FAIL').rjust(padding)
    log(f'{file}:{indicator}')

    if not pass_result:
        log('EXPECTED:', print_file=sys.stderr)

        for line in test_output.splitlines():
            log(f'\t"{line}"', print_file=sys.stderr)

        log('ACTUAL:', print_file=sys.stderr)

        for line in test_output.splitlines():
            log(f'\t"{line}"', print_file=sys.stderr)

        log('', print_file=sys.stderr)

