def draw()
	cls(0)
	text(inttotext("Frame %i", frame), 10,10,2,3)	
	text(inttotext("Time %i ms", delta), 10,30,2,4)
	text(inttotext("Mouse %i,%i", mouse_x, mouse_y), 10,50,2,13)
enddef