def r(x,d)
	print x
	if d > 0 then
		if x<10 then
			r(x+1,1)
		endif
	endif
	if d < 0 then
		if x>0 then
			r(x-1,-1)
		endif
	endif

enddef

'r(5,1)
'r(5,-1)