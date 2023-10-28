def draw()
	cls(0)
	text(formatText("Frame %i", frame), 10,10,2,3)	
	text(formatText("Time %f ms", delta*1000), 10,30,2,4)
	text(formatText("Mouse %i,%i", mouse_x, mouse_y), 10,50,2,13)
enddef