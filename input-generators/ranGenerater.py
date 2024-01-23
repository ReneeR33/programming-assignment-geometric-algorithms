#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Jan 21 16:12:21 2024

@author: frida
"""
import random
import sys
n=int(sys.argv[1])
m = 1000
 
print('horizontal segments ' + str(n))

for i in range(n):
    a = random.randint(1, m-1)
    b = random.randint(a+1, m)
    c = random.randint(1, m)
    print(str(a)+ ' '+ str(b) + ' '+str(c))


print('vertical segments ' + str(n))
    
for i in range(n):
    a = random.randint(1, m-1)
    b = random.randint(a+1, m)
    c = random.randint(1, m)
    print(str(c)+ ' '+ str(a) + ' '+str(b))