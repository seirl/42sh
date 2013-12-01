#!/usr/bin/env python3
# -*- encoding: utf-8 -*-

import random
import subprocess

binary = ['+', '-', '*', '>>', '<<', '|', '&', '||', '&&', '^']
unary = ['+', '-', '!', '~']

def gen_arithm(depth):
    if not random.randint(0, depth):
        return str(random.randint(1, 1000))
    if not random.randint(0, 4):
        return '({})'.format(gen_arithm(depth - 1))
    if not random.randint(0, 20):
        return '{}{}'.format(random.choice(unary), gen_arithm(depth - 1))
    return '{} {} {}'.format(gen_arithm(depth - 1), random.choice(binary),
                                 gen_arithm(depth - 1))

def gen_arithm_test():
    random.seed()
    i = gen_arithm(10)
    r = subprocess.check_output(["echo $(( {} ))".format(i)], shell=True)
    r = int(r.strip())

    return {
        "desc": "Random arithmetic",
        "type": "diff",
        "input": "echo $(( {} ))".format(i),
    }
