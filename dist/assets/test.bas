def tick()

enddef

def draw()
	cls(0)
	text(str(frame),10,10,1,3)
	rect(0,0,320,200,3,6)
	line(mouse_x, 0, mouse_x,200,1,14)

	sprite(0,mouse_x+2,mouse_y+2,1)
	sprite(0,mouse_x,mouse_y,3)
	print frame
enddef