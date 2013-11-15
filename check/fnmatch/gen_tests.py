#!/usr/bin/env python3
# -*- encoding: utf-8 -*-

def make_test(pattern, string, i, output):
    print(pattern, string)
    print('Test name ? ', end='')
    desc = input()
    print()
    name = '_'.join(['%02i' % i] + desc.lower().split()) + '.test'
    r = '''{
    "type": "fnmatch",
    "desc": "%s",
    "string": "%s",
    "pattern": "%s",
    "output": "%s"
}''' % (desc, string, pattern, output)
    print(r, file=open(name, 'w'))


tests = filter(bool, open('tests').read().split('\n'))
for i, t in enumerate(tests):
    output, pattern, string = t.split(',')
    make_test(pattern.strip("'"), string.strip("'"), i, output)
