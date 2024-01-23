#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Jan 21 16:09:52 2024

@author: frida
"""
import sys

n= int(sys.argv[1])
 
print('horizontal segments ' + str(n))

for i in range(n):
    print('1 '+str(n+2) + " " + str(i+2))


print('vertical segments ' + str(n))
    
for i in range(n):
    print(str(n+i+3) +' 1 '+str(n+2) )