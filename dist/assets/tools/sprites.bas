import "assets/lib/ui.bas"

pixelSize = 8   

DEF paletteButton(x,y,color)
    size = 11
    x=x+size*color
    border = 1
    hover = isHover(x,y,size,8)
    IF hover THEN
        border = 15
    ENDIF
    draw.rect(x,y,size,8,0,color)
    draw.rect(x,y,size,8,1,border)
ENDDEF

DEF drawPal(x,y)
    draw.rect(x-2,y-2,179,12,2,0)
    FOR i = 0 to 15
        paletteButton(x,y,i)
    NEXT

ENDDEF
 
DEF pixelCanvas(x,y)
    color = 0
    hover = isHover(x,y,pixelSize,pixelSize)
    IF hover THEN
        color = 12
    ENDIF
    draw.rect(x,y,pixelSize,pixelSize,0,color)
ENDDEF

DEF drawCanvas(x,y)
    FOR i = 0 to 15
        FOR j = 0 to 15
            pixelCanvas(x+j*pixelSize,y+i*pixelSize)
        NEXT
    NEXT
ENDDEF

DEF draw()

    cls(1)

    drawCanvas(4,4)
    drawPal(140,4)

    draw.text(floatToText("%f",mouse.x()/8.0),4,180,8,10)
    draw.text(intToText("%i",mouse.y()),4,190,8,11)
    draw.text(intToText("%03ix%03i",mouse.x(),mouse.y()),40,170,8,11)
    'draw.text(textformat("%03.02f x %03.02f",mouse.x(),mouse.y()),40,190,8,11)
    'draw.text(textformat("%.f : %.f : %.f :%i ",55.0,1,1,66),180,190,8,11)

    draw.rect(140,20,128,128,0,0)
    if (mouse.down(0)) then mouse.setpos(160, 100) endif

    drawmouse()

ENDDEF