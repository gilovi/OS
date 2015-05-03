import re

f = open("test2.out")

print ("   **start test:**")

try:
	for i in range(1,100):
	    line = next(f)
	    if line != (str(i)+"\n"):
		print ("error in line " +str(i)+ ". should be " + str(i) + " instead of " + line)

	#match error
	line = next(f)
	error_ptr = re.compile('thread library error:(.*)\n')
	error_mcth = re.match(error_ptr, line)
	if not (error_mcth):
	    print ("too many threads. should be an error")
	line = next(f)
	if line != ("-1\n"):
	    print ("should be an error and print -1")


	#match error
	line = next(f)
	error_mcth = re.match(error_ptr, line)
	if not (error_mcth):
	    print ("too many threads. should be an error")
	line = next(f)
	if line != ("-1\n"):
	    print ("should be an error and print -1")

	line = next(f)
	if line != (str(5)+"\n"):
	    print ("error. should be " + str(5) + " instead of " + line)

	#match error
	line = next(f)
	error_mcth = re.match(error_ptr, line)
	if not (error_mcth):
	    print ("too many threads. should be an error")
	line = next(f)
	if line != ("-1\n"):
	    print ("should be an error and print -1")


	for i in range(15,86,10):
	    line = next(f)
	    if line != (str(i)+"\n"):
		print ("error in line " +str(i)+ ". should be " + str(i) + " instead of " + line)

	#match error
	line = next(f)
	error_mcth = re.match(error_ptr, line)
	if not (error_mcth):
	    print ("too many threads. should be an error")
	line = next(f)
	if line != ("-1\n"):
	    print ("should be an error and print -1")

except:
	print ("   **test fail**")
else:
	try:
		line = next(f)
	except:
		print ("   **test success**")
	else:
		print ("   **test fail**")

