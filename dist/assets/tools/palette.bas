import "assets/lib/pico-palette.bas"
import "assets/lib/ui.bas"

ui.colorbase = 1
ui.colorBaseHover = 5
ui.colorHi = 15
ui.colorHiHover = 14
ui.buttonW = 80

selectedColor = 0

redK = 0
greenK = 0
blueK = 0

def colorBox(id)
    x=id*20
    y = 24
    hover = ui.isHover(x,y,19,16)
    IF hover AND mouse.pressed(0) THEN
        selectedColor = id
        redK = getColor(id,0)
        greenK = getColor(id,1)
        blueK = getColor(id,2)
    ENDIF
    
    draw.rect(x,y,19,16,0,id)

    IF id = 0 THEN
        draw.rect(x,y,19,16,1,1)
    ENDIF
    
    IF selectedColor = id THEN
        draw.rect(x,y,19,16,1,15)
    ENDIF


    draw.font(intToText("%02i",id), x+4, y+20, 8, 15)
enddef

def colorBoxSelector(y)

    draw.font(intToText("Color: %02i",selectedColor), 2, y, 8, 15)
    draw.rect(78,y,80,10,1, 15)
    draw.rect(79,y+1,78,8,0, selectedColor)
    
    for id = 0 to 15
        colorBox(id)
    next
enddef
    redK = 18

def draw()
    cls(0)
    colorBoxSelector(4)

    IF ui.button (168,3,"Restore") THEN restorePalette() ENDIF
    IF ui.button (4,62,"PicoPal") THEN setPicoPalette() ENDIF

    redK = ui.knob(redK,130,88, 0,255)
    greenK = ui.knob(greenK,175,88, 0,255)
    blueK = ui.knob(blueK,220,88,0,255)

    IF mouse.down(0) AND ui.mouseWorking THEN
        setColor(selectedColor, redK, greenK,blueK)
    ENDIF
    
    IF mouse.down(2) THEN ui.drawPalette() ENDIF
    ui.drawmouse()
    
enddef