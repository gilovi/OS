#!/usr/bin/env python3

import os
import tarfile
import test_README


def err(msg):
    if msg:
        print msg


os.system("make tar")
os.system("make clean")
files = tarfile.open("ex2.tar")
files.extractall(path=".", members=None)

#tar
required_input_files = {"README", "Makefile", "uthreads.h"}
sentFiles = files.getnames()

for r in required_input_files:
    if r not in sentFiles:
        err("Missing Input File: " + r)



#readme
if "README" in sentFiles:
    x = test_README.test_README(os.getcwd())
    err(x)

#make
os.system("make")
files_after_make = os.listdir(os.getcwd())

required_output_files = {"libuthreads.a"}
for r in required_output_files:
  if r not in files_after_make:
    err("Missing Output File: " + r)

os.system("make clean")
files.close()
