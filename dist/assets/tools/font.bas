import "assets/lib/ui.bas"

pixelSize = 16
pixelList = list(0 to 63)
for i = 0 to 63
	pixelList(i) = 0
next
fontList = list(0 to 255)
for i = 0 to 255
	fontList(i) = 0
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
def setBinary(lastFontChar)
    for b = 0 to 7
        g = getFontByte(lastFontChar,b)
    while g > 1
        if g MOD 2 <> 0 then print 1 else print 0 endif
        g = g / 2
    wend
    print;
    next
        print(intToText("%i,",lastFontChar));
enddef
DEF pixelCanvas(x, y, offSetX, offsetY)
    color = 1
    id=offSetX+(offSetY*8)

    x = x + offSetX*pixelSize;
    y = y + offSetY*pixelSize

    hover = isHover(x, y, pixelSize, pixelSize)
    IF hover THEN
        color = 11
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

lastFontChar = 33
fontList(lastFontChar) = 1
def fontChar(x,y,offSetX,offSetY)
    color = 8
    char = offSetX+(16*offSetY)+33

    x = x + offSetX*8;
    y = y + offSetY*8

    hover = isHover(x, y, 8, 8)
    if hover then
        draw.rect(x,y,8,8,0,1)
        color = 13
    endif

    IF mouse.down(0) and hover THEN
        fontList(lastFontChar) = 0
        fontList(char) = 1
        lastFontChar = char
    ENDIF

    IF fontList(char) = 1 THEN
        color = 15
    endif

    draw.font(getChar(char), c*8+x, y+d*8, 8, color)

    if mouse.down(2) and hover then
        draw.rect(172,132,22,11,0,1)
        draw.rect(172,132,22,11,1,11)
        draw.text(inttotext("%03i",char),174,133,8,18)
    endif
enddef

def fontCanvas(x,y)
    'quote = getChar(34)
    'draw.font("!"+quote+"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHI",x,y,8,15)
    'draw.font("!"+ getChar(mouse.x())+ "Ññª!",x,y+8,8,14)
    'draw.font("JKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmnopqrstuvwxyz{|}~",0,8,8,7)
    draw.font("0 1 2 3 4 5 6 7 8 9 ñ Ñ",0,160,8,3)
    for d = 0 to 5
        for c = 0 to 15
            fontChar(x,y,c,d)
        next
    next

enddef
def draw()
    cls(0)
    fontCanvas(184,8)
    drawCanvas(4,4)
for b = 0 to 7
    g = getFontByte(lastFontChar,b)
    draw.text(intToText("%i",g),4+b*20,150,8,15)
next

    buttonW = 20
    if button(160,32, "<<<") then
        setBinary(lastFontChar)
    endif
    if button(160,48, ">>>") then
        setFontChar(lastFontChar, getBinary(0),getBinary(1),getBinary(2),getBinary(3),getBinary(4),getBinary(5),getBinary(6),getBinary(7))
    endif

    buttonW = 50
    IF mouse.down(1) THEN  drawPalette(180) ENDIF

    ccc = slider(ccc,172,160,100,100)
    draw.rect(172,132,22,11,0,1)
    draw.rect(172,132,22,11,1,11)
    draw.text(inttotext("%03i",lastFontChar),174,133,8,18)
    draw.font(getchar(ccc),210,133,8,18)
    drawmouse()


enddef