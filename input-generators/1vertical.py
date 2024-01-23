#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Jan 21 16:02:25 2024

@author: frida
"""
import sys

n= int(sys.argv[1])
 
print('horizontal segments ' + str(n))

for i in range(n):
    print('1 5 '+ str(i+2))


print('vertical segments 1')
    

print('6 1 '+str(n+2))