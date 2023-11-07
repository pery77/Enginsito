'[
Based in this video
https://www.youtube.com/watch?v=N60lBZDEwJ8&list=PLB_ibvUSN7mzUffhiay5g5GUHyJRO4DYr
']
let road = 2000
let segL = 200
let camD = 0.84

class linea
	var x = 0
	var y = 0
	var z = 0
	var xS = 0
	var yS = 0
	var WS = 0
	var scale = 1
endclass



def drawQuad(col, x1 ,y1, w1, x2, y2, w2)
	p1x = x1-w1
	p1y = y1
	p2x = x2-w2
	p2y = y2
	p3x = x2+w2
	p3y = y2
	p4x = x1+w1
	p4y = y1
	line(p1x, p1y, p2x, p2y, 1 ,col)
	line(p2x, p2y, p3x, p3y, 1 ,col)
	line(p3x, p3y, p4x, p4y, 1 ,col)
	line(p4x, p4y, p1x, p1y, 1 ,col)



enddef

def draw()
	cls (0)
	drawQuad(14,mouse_X,100,50,160,40,20)
enddef