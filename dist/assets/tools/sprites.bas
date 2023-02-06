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

def setBinary(selectedSprite)    
    for l = 0 to 7
        g = getSpriteByte(selectedSprite,l)
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

'def makeprites()
'    for sp = 0 to 255
'        setSprite(sp,255,0,sp,sp,sp,sp,0,255)
'        setSprite(sp,0,0,0,0,0,0,0,0)
'    next
'enddef
'
'makeprites()
'renderSprites()

selectedSprite = 0
rotation = 0
h=0
v=0
def drawSprite(id,x,y)
    col = 8

    x = floor(id MOD 16) * 8 + x
    y = floor(id / 16) * 8 + y

    hover = ui.isHover(x,y,9,9)
    if hover then
        col = 13
        draw.rect(x,y,8,8,0,5)
        if mouse.pressed(0) then
            selectedSprite = id
        endif
    endif
    
    if id = selectedSprite then
        col = 13
        draw.rect(x,y,8,8,0,0)
    endif

    sprite(id, x , y, col, rotation + (h*8) + (v*16))

enddef

def drawSheet(x,y)
    draw.rect(x,y,128,128,0,1)
    for sp = 0 to 255
        drawsprite(sp,x,y)
    next

    draw.font(intToText("%03i" ,selectedSprite),136,4,8,11)

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
        draw.font(intToText("%i",id),x+2,y+2,8,7)
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
        draw.font(intToText("%03i",getBinary(j)),x+132,4 + y + j * 16,8,11)
    NEXT

ENDDEF

setSprite(0,255,128,191,176,160,160,160,160)
renderSprites()

'id, postition, sprite_id, offset_x,  offset_y,  color, flags
addmetasprite(0,0,0,0,0,4,0)
addmetasprite(0,1,0,8,0,11,1)
addmetasprite(0,2,0,8,8,7,2)
addmetasprite(0,3,0,0,8,6,3)

addmetasprite(1,0,2,0,0,9,0)
addmetasprite(1,1,1,0,0,15,0)





DEF draw()
    cls(2)
    drawSheet(4,4)
    drawCanvas(164,4)

    ui.buttonW = 20
    if ui.button(138,16, "<") then
        setSprite(selectedSprite, getBinary(0),getBinary(1),getBinary(2),getBinary(3),getBinary(4),getBinary(5),getBinary(6),getBinary(7))
        renderSprites()
    endif
    if ui.button(138,48, ">") then
        setBinary(selectedSprite)
    endif

    h = ui.toogle(h,18,170,5)
    v = ui.toogle(v,30,170,5)
    rotation = ui.slider(rotation,18,140,32,32)
draw.rect(82,150,8,8,0,1)

    metasprite(0,82,150)
    for x = 0 to 40
        for y = 10 to 20
            metasprite(1,x*8,y*8)
        next
    next
    

    IF mouse.down(1) THEN  ui.drawPalette() ENDIF
    ui.drawmouse()


ENDDEF