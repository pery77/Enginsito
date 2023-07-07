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
    for l = 0 to 7
        g = getSpriteByte(lastFontChar,l)
        while g > 0
            pixelList((7 - counter) + l*8) = 0
            color = 0
            if (g MOD 2) <> 0 then color = 15 endif
            pixelList((7 - counter) + l*8) = color
            g = floor(g / 2)
            counter = counter + 1
        wend
        while counter < 8
             pixelList((7 - counter) + l*8) = 0
            counter = counter + 1
        wend
        counter = 0
    next
enddef
DEF pixelCanvas(x, y, offSetX, offsetY)
    color = 1
    id=offSetX+(offSetY*8)

    x = x + offSetX*pixelSize;
    y = y + offSetY*pixelSize

    hover = ui.isHover(x, y, pixelSize, pixelSize)
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
        draw.text(intToText("%i",id),x+2,y+2,1,7)
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
        draw.text(intToText("%03i",getBinary(j)),x+132,4 + y + j * 16,1,11)
    NEXT

ENDDEF

lastFontChar = 33
fontList(lastFontChar) = 1
def fontChar(x,y,ch)
    color = 8
    char = ch
    c0 = ch-33

    x = x + floor(c0 MOD 16) * 8;
    y = y + floor(c0 / 16) * 8

    hover = ui.isHover(x, y, 8, 8)
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

    draw.text(getChar(char), x, y, 1, color)

    if mouse.down(2) and hover then
        draw.rect(172,132,22,11,0,1)
        draw.rect(172,132,22,11,1,11)
        draw.text(inttotext("%03i",char),174,133,1,18)
    endif
enddef

def fontCanvas(x,y)
    quote = chr(34)
    '
    draw.text("!"+quote+"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHI",0,170,1,15)
    'draw.text("!"+ getChar(241)+ "Ññª!",0,178,1,14)
    draw.text("JKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmnopqrstuvwxyz{|}~",0,178,1,14)
    draw.text("ñÑ©óòýü",0,186,1,12)
    draw.text(getChar(mouse.x()),120,186,1,12)
    for ch = 33 to 254
        fontChar(x,y,ch)
    next

enddef
def draw()
    cls(0)
    fontCanvas(184,8)
    drawCanvas(4,4)

    draw.text(intToText("%i",lastFontChar),55,150,1,15)


    ui.buttonW = 20
    if ui.button(160,32, "<") then
        setBinary(lastFontChar)
    endif
    if ui.button(160,48, ">") then
        setSprite(lastFontChar, getBinary(0),getBinary(1),getBinary(2),getBinary(3),getBinary(4),getBinary(5),getBinary(6),getBinary(7))
        updatefont()
    endif

    ui.buttonW = 50
    IF mouse.down(1) THEN  ui.drawPalette() ENDIF

    fontspace = ui.slider(fontspace,20,188,20,20)
    setfontspacing(fontspace)
    'draw.rect(172,132,22,11,0,1)
    'draw.rect(172,132,22,11,1,11)
    'draw.text(inttotext("%03i",lastFontChar),174,133,1,18)
    'draw.text(getchar(ccc),210,133,1,18)
    'draw.text(getchar(ccc),210,140,1,18)
    'print(getchar(ccc));
    ui.drawmouse()


enddef