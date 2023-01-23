import "assets/lib/pico-palette.bas"
import "assets/lib/ui.bas"

def draw()
    cls(0)
    draw.TRIANGLE(mouse.x(), 32,mouse.x()+4, 24,mouse.x()-4, 24, 1, 12)
    draw.TRIANGLE(mouse.x(), 32,mouse.x()+4, 24,mouse.x()-4, 24, 0, 15)
    draw.rect(99,7,182,14,1, 15)
    draw.rect(100,8,180,12,0, floor(mouse.x()/20))
    draw.text(textformat("Color: %02i",floor(mouse.x()/20)), 2, 4, 20, 15)
    
    for i = 0 to 15
        x = i*20
        draw.rect(x,34,19,100,0,i)
        draw.text(textformat("%02i",i), x+4, 134, 8, 15)
    next

    IF button (10,150,"Restore") THEN restorePalette() ENDIF
    IF button (10,165,"PicoPal") THEN setPicoPalette() ENDIF
    
    drawmouse()
    
enddef