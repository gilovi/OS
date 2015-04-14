import re

f = open("test3.out")

print ("   **start test:**")

try:
	error_ptr = re.compile('thread library error:(.*)\n')
	for i in range(15):
		line = next(f)
		error_mcth = re.match(error_ptr, line)
		if not (error_mcth):
			print ("should be an error")
except:
	print ("   **test fail**")
else:
	try:
		line = next(f)
	except:
		print ("   **test success**")
	else:
		print ("   **test fail**")

