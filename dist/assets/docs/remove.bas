'[
REMOVE(list, position)   -> Removes the element at a specific index in a list, 
							 or the element with a specific key in a dictionary,
                             overridable for referenced usertype and class instance.
CLEAR(list)              -> Clears a list or a dictionary, 
							 overridable for referenced usertype and class instance.
CLONE(list)              ->  Clones a collection, or a referenced usertype
TO_ARRAY(list)           ->  Copies all elements from a list to an array
']

l = list(0,1,2,3,4,5,6,7,8,9)

remove(l, 3)

for n in l
	print n
next

l2 = clone(l)

clear(l)
print "length of l after clear"
print len(l)

print "length of l2"
print len(l2)

arr = to_array(l2)

print type(arr)
print arr(len(arr)-1)