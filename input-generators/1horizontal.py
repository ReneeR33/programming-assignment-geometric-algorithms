#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Jan 21 16:06:05 2024

@author: frida
"""
import sys
n= int(sys.argv[1])
 
print('horizontal segments 1')

print('1 '+str(n+2) + ' 6')

print('vertical segments '+ str(n))
    
for i in range(n):
    print(str(i+2)+' 1 5')
