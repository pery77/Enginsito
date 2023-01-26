import "assets/lib/ui.bas"

pixelSize = 16
pixelList = list(0 to 63)
for i = 0 to 63
	pixelList(i) = 0
next

def getBinary(row)
    result = 0;
    counter = 7;
    for i = 0 to 7
        bit = 0
        IF pixelList(i+row*8) <> 0 THEN
            bit = 1
        ENDIF
        result = result + (bit * (2^counter))
        counter = counter - 1
    next
    return result
enddef

DEF pixelCanvas(x, y, offSetX, offsetY)
    color = 1
    id=offSetX+(offSetY*8)

    x = x + offSetX*pixelSize;
    y = y + offSetY*pixelSize

    hover = isHover(x, y, pixelSize, pixelSize)
    IF hover THEN
        color = 11
        IF mouse.pressed(0) THEN
            selected = id
        ENDIF 
    ENDIF

    IF mouse.down(0) and hover THEN
        draw.rect(x,y,pixelSize,pixelSize,0,selectedColor)
        pixelList(id) = 15
    ENDIF
    IF mouse.down(1) and hover THEN
        draw.rect(x,y,pixelSize,pixelSize,0,selectedColor)
        pixelList(id) = 0
    ENDIF

    draw.rect(x,y,pixelSize,pixelSize,0,pixelList(id))
    draw.rect(x,y,pixelSize,pixelSize,1,color)

    IF mouse.down(2) THEN
        draw.text(intToText("%i",id),x+2,y+2,8,7)
    ENDIF


ENDDEF

DEF drawCanvas(x,y)
    draw.rect(x-2, y-2, pixelSize * 8+4, pixelSize*8+4, 2, 4)
    FOR i = 0 to 7
        FOR j = 0 to 7
            pixelCanvas(x, y, j, i)
        NEXT
    NEXT

    FOR j = 0 to 7
        draw.text(intToText("%03i",getBinary(j)),x+132,4 + y + j * 16,8,11)
    NEXT

ENDDEF

def testFont(x,y)
    draw.font("!#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHI",x,y,8,15)
    draw.font("!"+ getChar(mouse.x())+ " !",x,y+8,8,14)
    'draw.font("JKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmnopqrstuvwxyz{|}~",0,8,8,7)
    'draw.font("0 1 2 3 4 5 6 7 8 9",0,16,8,3)
enddef
def draw()
    cls(0)
    
    testFont(180,4)
    drawCanvas(4,4)

    IF mouse.down(1) THEN  drawPalette(180) ENDIF
    drawmouse()

enddef