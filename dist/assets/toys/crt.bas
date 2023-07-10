def tick()
	b = (sin(frame * 0.1)*128)+128
	poke(0xd37,b)	
	poke(0xff0,b)
enddef

def draw()
	cls(0)
	t = t + 1
	h =  mouse_y
	if h < 100 then h = 100
	if h > 180 then h = 180
	
	line(0,h,320,h,1,10)
	for g = h to 200 step 16
		line(0,g+t,320,g+t,1,10)
	next
	if t > 10 then t = 0
	
	for f = 0 to 320 step 16
		line(160 + (f - 160),h, 160 + (f - 160)*2, 200,1,10)
	next
	
enddef