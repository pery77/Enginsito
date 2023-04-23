def draw()

	cls(0)
	draw.rect(0,0,319,199,2,4,0)
	draw.text("Ole mis wevos",4,4,2,11)
    draw.triangle(mouse.x(), mouse.y(), mouse.x(), mouse.y()+10, mouse.x()+7, mouse.y()+7, 0, 1)
    draw.triangle(mouse.x(), mouse.y(), mouse.x(), mouse.y()+9, mouse.x()+7, mouse.y()+7, 1, 10)

enddef


