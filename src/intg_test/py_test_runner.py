# Copyright 2007 Baptiste Lepilleur and The JsonCpp Authors
# Distributed under MIT license, or public domain if desired and
# recognized in your jurisdiction.
# See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE

"""Simple implementation of a json test runner to run the test against
json-py."""

import sys
import os.path
import json
import types

if len(sys.argv) != 2:
    print("Usage: %s input-json-file", sys.argv[0])
    sys.exit(3)

input_path = sys.argv[1]
base_path = os.path.splitext(input_path)[0]
actual_path = base_path + '.actual'
rewrite_path = base_path + '.rewrite'
rewrite_actual_path = base_path + '.actual-rewrite'

def valueTreeToString(fout, value, path = '.'):
    ty = type(value)
    if ty  is types.DictType:
        fout.write('%s={}\n' % path)
        suffix = path[-1] != '.' and '.' or ''
        names = value.keys()
        names.sort()
        for name in names:
            valueTreeToString(fout, value[name], path + suffix + name)
    elif ty is types.ListType:
        fout.write('%s=[]\n' % path)
        for index, childValue in enumerate(value):
            valueTreeToString(fout, childValue, path + '[%d]' % index)
    elif ty is types.StringType:
        fout.write('%s="%s"\n' % (path,value))
    elif ty is types.IntType:
        fout.write('%s=%d\n' % (path,value))
    elif ty is types.FloatType:
        fout.write('%s=%.16g\n' % (path,value))
    elif value is True:
        fout.write('%s=true\n' % path)
    elif value is False:
        fout.write('%s=false\n' % path)
    elif value is None:
        fout.write('%s=null\n' % path)
    else:
        assert False and "Unexpected value type"

def parseAndSaveValueTree(input, actual_path):
    root = json.loads(input)
    with open(actual_path, 'w') as fout:
        valueTreeToString(fout, root)
    return root

def rewriteValueTree(value, rewrite_path):
    rewrite = json.dumps(value)
    #rewrite = rewrite[1:-1]  # Somehow the string is quoted ! jsonpy bug ?
    with open(rewrite_path, 'w') as fout:
        print(rewrite, file=fout)
    return rewrite

with open(input_path, 'r') as f:
    input = f.read()
root = parseAndSaveValueTree(input, actual_path)
rewrite = rewriteValueTree(json.write(root), rewrite_path)
rewrite_root = parseAndSaveValueTree(rewrite, rewrite_actual_path)

sys.exit(0)
