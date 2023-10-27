
for c = 0 to 31
	savedata(c,c)
next
for c = 0 to 31
	print loaddata(c)
next
def draw()
	cls(0)
	text(str(frame),10,10,1,3)
	rect(0,0,320,200,3,6)
	line(mouse_x, 0, mouse_x,200,1,14)

	sprite(0,mouse_x+2,mouse_y+2,1)
	sprite(0,mouse_x,mouse_y,3)

text(str(loaddata(0)),10,20,1,2)
enddef

def close()
	print "bye"
	'quit()
enddef