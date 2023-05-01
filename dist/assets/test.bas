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
	draw.rect(0,0,320,200,2,13)
enddef

for d = 0x440 to 0x820
	poke(d,0)
next



'asdfasdfasdf222233333
