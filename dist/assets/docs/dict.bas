'DICT         -> ' Creates a dictionary.

d = dict()
d(0)  = "cero"   
d(10) = "diez"
d(5) = "xxx"
d(6) = list(1,"wow",3)

for i in d
	print d(i)
next

print d(2) 'print Unknown
l =  d(6)
print l(1) 'print the second element of list stored in d(6)


d2 = dict(0, "key", 1, "door", 2, "chest")
for i in d2
	print str(i) + ">" + d2(i)
next