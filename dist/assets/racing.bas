'[
Based in this video
https://www.youtube.com/watch?v=N60lBZDEwJ8&list=PLB_ibvUSN7mzUffhiay5g5GUHyJRO4DYr
']
let road = 1500
let segL = 100
let camD = 0.84

class linea

	var x = 0
	var y = 0
	var z = 0
	var xS = 0
	var yS = 0
	var WS = 0
	var scale = 0
	var curve = 0
	
	def project(camX, camY, camZ)
		scale = camD/(z-camZ)
		xS = (1 + scale*(x - camX)) * 160
		yS = (1 - scale*(y - camY)) * 99
		WS = scale * road * 160
	enddef
	
endclass

def drawQuad(col, x1 ,y1, w1, x2, y2, w2)

	if w1 < w2 or y1 < y2 then return
	
	p1x = x1-w1
	p1y = y1
	p2x = x2-w2
	p2y = y2
	p3x = x2+w2
	p3y = y2
	p4x = x1+w1
	p4y = y1
	
	triangle(p4x, p4y, p2x, p2y, p1x, p1y, 0, col)
	triangle(p3x, p3y, p2x, p2y, p4x, p4y, 0, col)
	line(p1x, p1y, p2x, p2y, 1 ,7)
	line(p3x, p3y, p4x, p4y, 1 ,7)
	
	'line(p2x, p2y, p3x, p3y, 1 ,col)
	'line(p4x, p4y, p1x, p1y, 1 ,7)

enddef

let lines = list()
let maxRoad = 2000
for i = 0 to maxRoad

	l = new(linea)
	l.z = i * segL
	
	if i > 100 and i < 170 then l.curve = 1
	if i > 300 and i < 400 then l.curve = -2
	
	l.y = sin(i/30) * 1000
	
	push(lines, l)
	
next

let nL = len(lines)
let pos = 0
let playerX = 0

def tick()

	if keydown(key_w) then pos = pos + 100
	if keydown(key_s) then pos = pos - 100
	if keydown(key_d) then playerX = playerX + 30
	if keydown(key_a) then playerX = playerX - 30
	
	if pos/segL > maxRoad-1 then pos = (maxRoad-1) * segL
	if pos < 0 then pos = 0
	
enddef

def draw()

	cls (10)
	'rect(0,100,320,200,0,14)
	
	startPos = pos / segL
	cx = 0 
	dx = 0
	l = get(lines, startPos)
	camH = 1500 + l.y
	maxY = 200
	
	endN = startPos + 200
t = 0
	for n = startPos to endN

		l = get(lines, n mod nL)
		l.project(playerX-cx,camH,pos)
		p = get(lines, n-1 mod nL)
		cx = cx + dx
		dx = dx + l.curve
		
		h = p.yS - l.yS
		

		if l.yS <= maxY and l.yS > 105 then

			maxY = l.yS
			grassCol = 13
			rumbleCol = 3
			roadCol = 1
			if n/3 mod 2 then 
				roadCol = 2
				rumbleCol = 4
				grassCol = 14
			endif
			drawQuad(grassCol,0,p.yS,320,0,l.yS,320)
			drawQuad(rumbleCol,p.xS,p.yS,p.wS*1.2,l.xS,l.yS,l.wS*1.2)
			drawQuad(roadCol,p.xS,p.yS,p.wS,l.xS,l.yS,l.wS)
			t = t +1
		
		endif

	next

	text(str(pos/segL),20,20,1,3)
	text(str(t),20,30,1,3)

enddef