'PUSH(list, value)  ->  Pushes a value to the tail of a list, overridable for
'                         referenced usertype and class instance.
'POP(list)          ->  Pops a value from the tail of a list, overridable for
'                         referenced usertype and class instance.
'BACK(list)         ->  Peeks the value at tail of a list, overridable for
'                         referenced usertype and class instance.

l = list()
print "pushing"
for c = 48 to 57
	push(l, chr(c))
next
for n in l
	print n
next

print "length"
print len(l)

print "popping"

for n = 0 to 4
	print pop(l)
next

print "length after popping 5 elements"
print len(l)

print "back"
for n = 0 to 2
	print back(l) ' get the last element 3 times
next