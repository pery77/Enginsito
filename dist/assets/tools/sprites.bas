import "assets/lib/ui.bas"

pixelSize = 8   
selectedColor = 0
d = list(0 to 500)
for i=0 to 500
	d(i) = 0
next

DEF paletteButton(x,y,color)
    size = 11
    x=x+size*color
    border = 12
    hover = isHover(x,y,size,8)
    IF hover THEN
        border = 15
    ENDIF 

    IF hover AND mouse.released(0) THEN
        selectedColor = color
    ENDIF

    IF color = selectedColor THEN
        draw.rect(x,y+8,size,2,1,15)
    ENDIF

    draw.rect(x,y,size,8,0,color)
    draw.rect(x,y,size,8,1,border)
ENDDEF

DEF drawPal(x,y)
    draw.rect(x-2,y-2,179,12,2,1)
    FOR i = 0 to 15
        paletteButton(x,y,i)
    NEXT

ENDDEF
selected = 0
DEF pixelCanvas(x,y)
    color = 1
    id=x/pixelSize+(y/pixelSize*16)

    hover = isHover(x,y,pixelSize,pixelSize)
    IF hover THEN
        color = 12
        IF mouse.pressed(0) THEN
            selected = id
        ENDIF 
    ENDIF

    IF mouse.down(0) and hover THEN
        draw.rect(x,y,pixelSize,pixelSize,0,selectedColor)
        draw.text(intToText("%i",id),1,1,8,11)
        d(id) = selectedColor
    ENDIF

    draw.rect(x,y,pixelSize,pixelSize,0,d(id))
    draw.rect(x,y,pixelSize,pixelSize,1,color)

ENDDEF

DEF drawCanvas(x,y)
    draw.rect(x,y,pixelSize * 16 ,pixelSize*16,0,0)
    FOR i = 0 to 15
        FOR j = 0 to 15
            pixelCanvas(x+j*pixelSize,y+i*pixelSize)
        NEXT
    NEXT
ENDDEF

DEF draw()

    cls(0)

    drawCanvas(4,20)
    drawPal(140,4)

    draw.rect(140,20,128,128,0,0)
IF mouse.down(1) THEN  drawPalette(180) ENDIF
    drawmouse()

ENDDEF