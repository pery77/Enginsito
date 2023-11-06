let x = 160
let ex = 20
let ey = 60
let g = 1
def draw()
	let s = 32
	cls(0)
	for i = 0 to 4
		line(x+i*s, 200 + s, x + 200 + i*s, 0 ,s,i+4)
	next

text ("Enginsito", ex-g,ey,4,1)
text ("Enginsito", ex+g,ey,4,1)
text ("Enginsito", ex,ey-g,4,1)
text ("Enginsito", ex,ey+g,4,1)
text ("Enginsito", ex,ey,4,3)


enddef