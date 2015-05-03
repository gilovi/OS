import os
from os import listdir
from os.path import isfile, join
from subprocess import check_call


path = "/cs/stud/moshemandel/OS/ex2/test2.2"
libary_location = "/cs/stud/moshemandel/OS/ex2"

make_command = "g++ -Wall -g -L {0} -I {0} {1} -luthreads -o {3}test{2} -std=c++11"

os.chdir("/")

testfiles = [ f for f in listdir(path) if isfile(join(path,f)) ]

i=0

for test in testfiles:

    if not test.endswith('.cpp'):
        continue

    command = make_command.format(libary_location, join(path, test), i, path)

	#    print(command)

    res = os.system(command)
    
    res = os.system(path + "test" + str(i))

    assert res == 0, "test failed for " + test

    i+=1

    # print(test)

print("ok")
