'FONTSPACE(space)
'Set font separation in pixels

def draw()
	cls(0)
	fontspace(0)
	text("This is normal space",10,10,1,4)
	fontspace(-2)
	text("This is -2 space",10,20,1,4)
	fontspace(6)
	text("And this is 6 space",10,30,1,4)
	
	fontspace(sin(frame*0.04)*18)
	text("Live Text",100,50,1,5)
enddef