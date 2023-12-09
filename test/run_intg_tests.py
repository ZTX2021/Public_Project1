# Copyright 2007 Baptiste Lepilleur and The JsonCpp Authors
# Distributed under MIT license, or public domain if desired and
# recognized in your jurisdiction.
# See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE

from io import open
import sys
import os

VALGRIND_CMD = 'valgrind --tool=memcheck --leak-check=yes --undef-value-errors=yes '

def getStatusOutput(cmd):
    """
    Return int, unicode (for both Python 2 and 3).
    Note: os.popen().close() would return None for 0.
    """
    print(cmd, file=sys.stderr)
    pipe = os.popen(cmd)
    process_output = pipe.read()
    try:
        # We have been using os.popen(). When we read() the result
        # we get 'str' (bytes) in py2, and 'str' (unicode) in py3.
        # Ugh! There must be a better way to handle this.
        process_output = process_output.decode('utf-8')
    except AttributeError:
        pass  # python3
    status = pipe.close()
    return status, process_output
def compareOutputs(expected, actual, message):
    expected = expected.strip().replace('\r','').split('\n')
    actual = actual.strip().replace('\r','').split('\n')
    diff_line = 0
    max_line_to_compare = min(len(expected), len(actual))
    for index in range(0,max_line_to_compare):
        if expected[index].strip() != actual[index].strip():
            diff_line = index + 1
            break
    if diff_line == 0 and len(expected) != len(actual):
        diff_line = max_line_to_compare+1
    if diff_line == 0:
        return None
    def safeGetLine(lines, index):
        index += -1
        if index >= len(lines):
            return ''
        return lines[index].strip()
    return """  Difference in %s at line %d:
  Expected: '%s'
  Actual:   '%s'
""" % (message, diff_line,
       safeGetLine(expected,diff_line),
       safeGetLine(actual,diff_line))

def safeReadFile(path):
    try:
        with open(path, 'rt', encoding = 'utf-8') as f:
            return f.read()
    except IOError as e:
        return '<File "%s" is missing: %s>' % (path,e)

class FailError(Exception):
    def __init__(self, msg):
        super(Exception, self).__init__(msg)

def runAllTests(jsontest_executable_path, input_dir = None,
                 use_valgrind=False, with_json_checker=False,
                 ):
    tests = list(input_dir.glob('*.json'))
    if with_json_checker:
        all_tests = (input_dir / '../jsonchecker').glob('*.json')
        # These tests fail with strict json support, but pass with JsonCPP's
        # extra leniency features. When adding a new exclusion to this list,
        # remember to add the test's number and reasoning here:
        known = ["fail{}.json".format(n) for n in [
            4, 9, # fail because we allow trailing commas
            7,    # fails because we allow commas after close
            8,    # fails because we allow extra close
            10,   # fails because we allow extra values after close
            13,   # fails because we allow leading zeroes in numbers
            18,   # fails because we allow deeply nested values
            25,   # fails because we allow tab characters in strings
            27,   # fails because we allow string line breaks
        ]]
        test_jsonchecker = [ test for test in all_tests
                             if test.name not in known]

    else:
        test_jsonchecker = []

    failed_tests = []
    valgrind_path = use_valgrind and VALGRIND_CMD or ''
    tests += test_jsonchecker
    for input_path in tests:
        expect_failure = input_path.name.startswith('fail')
        is_json_checker_test = (input_path in test_jsonchecker) or expect_failure
        print('TESTING:', input_path, end=' ')
        options = is_json_checker_test and '--json-checker' or ''
        cmd = '%s%s %s "%s"' % (valgrind_path, jsontest_executable_path, options, input_path.resolve())
        status, process_output = getStatusOutput(cmd)
        if is_json_checker_test:
            if expect_failure:
                if not status:
                    print('FAILED')
                    failed_tests.append((input_path, 'Parsing should have failed:\n%s' %
                                          safeReadFile(input_path)))
                else:
                    print('OK')
            else:
                if status:
                    print('FAILED')
                    failed_tests.append((input_path, 'Parsing failed:\n' + process_output))
                else:
                    print('OK')
        else:
            actual_output = safeReadFile(input_path.with_suffix('.actual'))
            actual_rewrite_output = safeReadFile(input_path.with_suffix('.actual-rewrite'))
            with open(input_path.with_suffix('.process-output'), 'wt') as f:
                f.write(process_output)
            if status:
                print('parsing failed')
                failed_tests.append((input_path, 'Parsing failed:\n' + process_output))
            else:
                with open(input_path.with_suffix('.expected'), 'rt') as f:
                    expected_output = f.read()
                detail = (compareOutputs(expected_output, actual_output, 'input')
                            or compareOutputs(expected_output, actual_rewrite_output, 'rewrite'))
                if detail:
                    print('FAILED')
                    failed_tests.append((input_path, detail))
                else:
                    print('OK')

    if failed_tests:
        print()
        print('Failure details:')
        for failed_test in failed_tests:
            print('* Test', failed_test[0])
            print(failed_test[1])
            print()
        print('Test results: %d passed, %d failed.' % (len(tests)-len(failed_tests),
                                                       len(failed_tests)))
        raise FailError(repr(failed_tests))
    else:
        print('All %d tests passed.' % len(tests))

def get_parser():
    from argparse import ArgumentParser
    parser = ArgumentParser()
    parser.add_argument('--valgrind', action='store_true', default=False, help='run all the tests using valgrind to detect memory leaks')
    # parser.add_argument('-c', '--with-json-checker', action='store_true', default=False, help='run all the tests from the official JSONChecker test suite of json.org')
    from pathlib import Path
    parser.add_argument('executable_path', type=Path, help='path to jsontestrunner executable')
    return parser

def main():
    options = get_parser().parse_args()

    from pathlib import Path
    test_case_directory = Path(__file__).parent / 'data'
    runAllTests(options.executable_path, test_case_directory,
                         use_valgrind=options.valgrind,
                         with_json_checker=True,
                         )

if __name__ == '__main__':
    try:
        main()
    except FailError:
        sys.exit(1)
