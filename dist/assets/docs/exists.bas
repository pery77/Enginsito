'[
EXISTS(list, value)   -> Tells whether a list contains a 
						  specific value, or whether a dictionary contains  a specific key, 
						  overridable for referenced usertype and class instance.
INDEX_OF(list, value) -> Gets the index of a value in a list, 
						  overridable for referenced usertype and class instance.
']

l = list("a" ,"b", 3, 4, "x")

print exists(l, "a") ' return 1 because "a" exist in list l
print exists(l, "c") ' return 0 because "c" does not exist in list l

print index_of(l,"c") ' print UNKNOWN
print index_of(l,"x") ' print 4, the index of the element x