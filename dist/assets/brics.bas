def draw()
	cls(0)
	for a = 0 to 320 step 8
		for b = 0 to 200 step 8
			meta(0,a,b)
		next
	next
enddef