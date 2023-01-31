import "assets/lib/pico-palette.bas"
import "assets/lib/ui.bas"
ui.colorbase = 0
ui.colorBaseHover = 4
ui.colorHi = 8
ui.colorHiHover = 13
ui.buttonW = 90
def draw()
    cls(0)
    draw.TRIANGLE(mouse.x(), 32,mouse.x()+4, 24,mouse.x()-4, 24, 1, 12)
    draw.TRIANGLE(mouse.x(), 32,mouse.x()+4, 24,mouse.x()-4, 24, 0, 15)
    draw.rect(78,3,80,10,1, 15)
    draw.rect(79,4,78,8,0, floor(mouse.x()/20))
    draw.font(intToText("Color: %02i",floor(mouse.x()/20)), 2, 4, 8, 15)
    
    for i = 0 to 15
        x = i*20
        draw.rect(x,34,19,16,0,i)
        draw.font(intToText("%02i",i), x+4, 52, 8, 15)
    next

    IF ui.button (10,74,"Restore") THEN restorePalette() ENDIF
    IF ui.button (10,86,"PicoPal") THEN setPicoPalette() ENDIF
    ui.drawPalette()
    ui.drawmouse()
    
enddef