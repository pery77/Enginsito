a=0
def tick()
	cls(8)
	print a
	a = a +3
enddef
def draw()
	cls(7)
	draw.text("X|O|X",8,8,3,3)	
	draw.text(inttotext("%02i",a),8,32,3,2)
	draw.text(inttotext("%02i",frame()),8,64,2,3)
enddef










