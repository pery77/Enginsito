def draw()

	cls(8)
	rect(0,0,319,199,2,4,0)
	text("Fxking LEGEND",4,4,2,11)
    triangle(mouse.x(), mouse.y(), mouse.x(), mouse.y()+10, mouse.x()+7, mouse.y()+7, 0, 0)
    triangle(mouse.x(), mouse.y(), mouse.x(), mouse.y()+9, mouse.x()+7, mouse.y()+7, 1, 10)

enddef


