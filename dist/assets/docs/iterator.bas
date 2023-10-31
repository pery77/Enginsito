'ITERATOR(list)      -> Gets an iterator of a list or a dictionary,
'                       overridablefor referenced usertype and class instance.
'MOVE_NEXT(iterator) -> Moves an iterator to next position over a list or a dictionary, 
'                       overridable for referenced user type and class instance.

d = dict(1, "One", 2, "Two")
set(d, 3, "Three")
set(d, 10, "Teen")

it = iterator(d)
while move_next(it)
	w = get(it)
	print formattext("%i -> ",get(it)) + d(w)
wend