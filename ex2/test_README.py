#!/usr/bin/env python3

import os
import re
import sys
import glob

BAD_LOGINS = 0
BAD_README = 1


def findInterestingFiles(path):
    files = glob.glob('*.c*')


# if the login(s) correctly extracted they and the no_appeal error are printed 
def test_README(path):
    
    usernames = None
    readme_filename = os.path.join(os.getcwd(), path, 'README')

    print('Reading ' + readme_filename)
    README_file_obj = open(readme_filename,'r')
    error_codes = set()

    # first line in README: logins
    line = README_file_obj.readline()
    p=re.compile('^(([\w\.]+)\s*)((\,\s?([\w\.]+))\s*)?\n$')
    m = p.match(line)
    if not m:
        README_file_obj.close()
        print "bad logins in README"
        return 

    # parse the second line: names and ids
    r = re.compile('^(\w+\s+)(\w+\s?)(\(\d+\)\s*)(,\s+(\w+\s+)(\w+\s?)(\(\d+\)\s*))?\n$')
    line = README_file_obj.readline()
    m = r.match(line)
    if not m:
        print "bad names and ids in README"
        return

    # parse the third line: EX: 2
    r = re.compile('^(Ex: 2)\n', re.IGNORECASE)
    line = README_file_obj.readline()
    m = r.match(line)
    if not m:
        print "no line Ex: 2 in README"
        return

    # parse the 4th line: empty line
    r = re.compile('(\n)')
    line = README_file_obj.readline()
    m = r.match(line)
    if not m:
        print "4th line in README not empty"
        return   
            
    README_file_obj.close()
    return 

