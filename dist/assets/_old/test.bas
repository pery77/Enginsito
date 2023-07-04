a=0
def init()
cls(0)
enddef

def draw()
	'cls(0)
	rect(0,0,320,200,1,50)
	pixel(mouse.x(), mouse.y(),a)
	a=mouse.x()/16
enddef
