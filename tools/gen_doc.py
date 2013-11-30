#!/usr/bin/env python3
# -*- encoding: utf-8 -*-

import re

def gen_doc(s):
    m = re.search('([a-zA-Z_]+)\(', s)
    if m:
        t = m.group(1).split('_')
        print('//! @brief {} a {}'.format(' '.join(t[1:]), t[0]))
