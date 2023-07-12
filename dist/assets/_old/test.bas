

int = formatText("(33) Int int   %06i",32)
float= formatText("(55.2) Float float %.6f ",55.229)

print formatText("(no)Int float %i ",55.222)
print formatText("(no)Float int   %f ",33)

print formatText("Multi  %i,%i,%i,%i,%i,%i,",1,2,3,4.6,5,6,7)
a = list(0,10000)
def draw()
	cls(0)
	rect(0,0,320,200,2,3)
	mousePos = formatText("%i",mem)
	text(int,4,4,1,2)
	text(float,4,12,1,2)
		text(mousePos,4,22,1,2)
	
enddef
