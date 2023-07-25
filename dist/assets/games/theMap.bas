s1 = "0"

class screen
	def draw(s)
		for l = 0 to 9
			for c = 0 to 19
				cx = c*16
				cy = l*16+40
				m = 0
				rect(cx,cy,16,16,1,1)
				'meta(m,cx,cy)
				text(formattext("%i",c),cx,cy,1,2)
			next
		next
	enddef
endclass

def drawUI()
	rect (0,0,320,40,1,2)
enddef

def draw()
	cls(0)
	drawUI()
	screen.draw(0)
	text(s1,0,0,1,2)
	print len (s1)
enddef