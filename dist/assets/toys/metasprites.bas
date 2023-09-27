flag = 0
def tick()
	flag = flag + MOUSE_WHEEL()
	flag = flag mod 4
	flag = flag + MOUSE_DOWN(0)*8	
	flag = flag + MOUSE_DOWN(1)*16
enddef

def draw()
	cls(0)
	rect(160,100,16,16,1,3)
	rect(220,100,32,8,1,3)
	text(str(flag),4,4,2,3)
	meta(0,160,100,flag)
	meta(1,220,100,flag)
	meta(2,220,150,flag)
		
	meta(3,80,80,flag)
	meta(4,80,100,flag)
		
	meta(0,mouse_X,mouse_y,flag)	
	meta(0,mouse_X,mouse_y,flag+1)
enddef