def draw()
	cls(0)
	for x = 0 to 39
		sprite(0,x*8,0,14)
		sprite(0,x*8,192,15)
	next
	sprite (2,100, 184,3)
	text ("a",(sin(frame*0.01) * 140)+150,20,4,6)
enddef