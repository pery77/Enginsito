'GET(list, position)        -> Returns the value at a specific index in a list, or the value with a specific key in a dictionary, 
'                  	            or a member of a class instance, overridable for referenced usertype and class instance.
'SET(list, position, value) -> Sets the value at a specific index in a list, or the value with a specific key in a dictionary,
'                               or a member variable of a class instance, overridable for referenced usertype and class instance.
'[
You can accessing elements in a list or dictionary using
brackets directly:
d = dict()
d(1) = 2
print d(1);
']

l = list("a" ,"b", 3, 4)
for n in l
	print n
next

print get(l,2)' print before set
set(l, 2, "c")' set new value
print get(l,2)' print after set

for n in l
	print n
next

d = dict()
set(d,0,"hello")
set(d,1,"world")

print get(d, 0)
print get(d, 1)