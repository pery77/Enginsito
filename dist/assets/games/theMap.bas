s1 = dict()
for c = 0 to 19
s1(c) = 1
s1(c+180) = 1
next
s1(76) = 2

class screen

	def draw(s)
		st = 0
	
		while (st<200)
			cx = (st mod 20) * 16
			cy = floor(st/20) * 16 + 40
			cm = s(st)-1
			if cm > -1 then meta(cm,cx,cy)

			'rect(cx,cy,16,16,1,1)
			'text(formattext("%i",m),cx,cy,1,2)
			st = st + 1
			
		wend
	enddef
endclass

def drawUI()
	rect (0,0,320,40,0,0)
enddef

def draw()
	cls(1)
	drawUI()
	screen.draw(s1)
enddef