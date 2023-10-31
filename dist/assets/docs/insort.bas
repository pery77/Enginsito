'INSERT(list, position, value)  -> Inserts a value at a specific position of a list,
'                                   overridable for referenced usertype and class instance.
'SORT(list)                     -> Sorts a list increasingly,  
'                                   overridable for referenced usertype and class instance.


'init a list with a pair of values
l = list("f" ,"h")

'push some random number in a list
for c = 0 to 9
	push(l, rnd(0,9))
next

'insert an element "a" in position 4
insert(l,4,"a")

print "print the list"
for n in l
	print n
next

'sort list
sort(l)
print "print sorted"
for n in l
	print n
next