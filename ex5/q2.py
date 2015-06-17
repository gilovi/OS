
import timeit
import subprocess


def mesure(fileto, times):
	program = "/tmp/clftp"
	T = 0
	for x in range(times):
		start = timeit.timeit()
		subprocess.call([program, "3333","nan-41",fileto,fileto[5:]])
		end = timeit.timeit()
		T += end-start
	return T/ times
	
def main():
	a=[]
	files = ["/tmp/test","/tmp/noflash.CR2" ,"/tmp/gray.RW2","/tmp/record.rar","/tmp/out.mp4"]
	for file in files :
		a.append(mesure(file, 1))
	
	print a
if __name__ == "__main__":
    main()
