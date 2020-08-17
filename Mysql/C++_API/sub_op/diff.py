f1 = open("log.txt",'r')
f2 = open("read.txt",'r')
#print(f1.read().split(" ")[-1] == f2.read().split(" ")[-1])

for i,j in zip(f1.read().split(" "),f2.read().split(" ")):
	print(i==j)
	
f1.close()
f2.close()
