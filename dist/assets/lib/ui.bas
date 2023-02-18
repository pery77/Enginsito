class ui

    colorBase = 0
    colorBaseHover = 5
    colorHi = 12
    colorHiHover = 15
    buttonW = 50
    buttonH = 11

    mouseColorBorder = 0
    mouseColor = 15

    colorSlider = 8
    colorSliderHover = 13

    mouseWorking = false

    mustDrawColorPiker = false
    colorPickSelected = -1
    idPickSelected = -1
    colorPikerX = 0
    colorPikerY = 0

def drawPalette()
    y=200
    for i = 0 to 15
        x = i*20
        draw.rect(x,y-10,20,10,0,i)
        draw.text(intToText("%02i",i), x+3, y-8, 1, 0)
        draw.text(intToText("%02i",i), x+5, y-8, 1, 0)
        draw.text(intToText("%02i",i), x+4, y-7, 1, 0)
        draw.text(intToText("%02i",i), x+4, y-9, 1, 0)
        draw.text(intToText("%02i",i), x+4, y-8, 1, 15)
    next
    draw.rect(0,y-21,59,11,0,0)
    draw.rect(0,y-21,59,11,1,11)
    draw.text(intToText("%03i,%03i",mouse.x(),mouse.y()), 2, y-19, 1, 11)
enddef

def drawMouse()
    if (mustDrawColorPiker) then drawColorPickerBox() endif
    draw.triangle(mouse.x(), mouse.y(), mouse.x(), mouse.y()+10, mouse.x()+7, mouse.y()+7, 0, mouseColorBorder)
    draw.triangle(mouse.x(), mouse.y(), mouse.x(), mouse.y()+9, mouse.x()+7, mouse.y()+7, 1, mouseColor)
enddef

'tools
def isHover(x,y,w,h)
    return mouse.x() > x AND mouse.y() > y AND mouse.x() < x + w AND mouse.y() < y + h
enddef

def clamp(v,min,max)
    IF v < min THEN return min ENDIF
    IF v > max THEN return max ENDIF
    return v
enddef

def remap(value, low1, low2, high1, high2)
return low2 + (value - low1) * (high2 - low2) / (high1 - low1)
enddef

'Ui elements
def button(x,y,txt)
    colB = colorBase
    colH = colorHi
    hover = isHover(x,y,buttonW, buttonH)

    IF hover THEN 
        colB = colorBaseHover
        colH = colorHiHover 
        IF mouse.down() THEN 
            colH = colB
            colB = colorBase
        ENDIF
    ENDIF

    draw.rect(x,y,buttonW,buttonH,0,colB)
    draw.rect(x,y,buttonW,buttonH,1,colH)
    xc = (buttonW * 0.5) - (measureText(txt,1) * 0.5)
    draw.text(txt,x + xc,y+2,1,colH)

    return (mouse.released(0) AND hover)
enddef

def slider(v,x,y,w,max)

    v = ROUND(w * v/max)
    cb = colorHiHover
    colN = 8
    colNH = 13
    col = colN

    mx = x+v+1
    my = y+1

    hover = isHover(x,y,w+10,buttonH)
    IF hover THEN
        col = colNH
        cb = colorHiHover
        
        vMul = 1
        IF w/max > 1 THEN
            vMul = w/max
        ENDIF
        v = v + (mouse.wheel() * vMul);
    ENDIF

    IF mouse.down(0) AND hover THEN
        v = v + (mouse.x() - mx - 4);
    ENDIF
    
    v = ROUND(v/w * max)
    v = clamp(v,0, max)
    
    draw.rect(x,y,w + 10,buttonH-2,0,0)
    draw.rect(x,y,w + 10,buttonH-1,1,cb)
    draw.rect(mx,my,8,buttonH-3,0,col)
    textPos = intToText("%02i",v)
    draw.text(textPos, x - measureText(textPos, 1)-2, y, 1, cb)

    return v
enddef

def toogle(v,x,y,r)
    cb = colorHiHover
    col = colorBase
    colOn = 3

    hover = isHover(x-r,y-r,r*2,r*2)
    IF hover THEN
        cb = colorHiHover
    ENDIF
    IF hover AND mouse.released() THEN
        v = NOT v
    ENDIF
    IF v THEN
        col = ColOn
    ENDIF
    draw.circle(x,y,r,1,col)
    draw.circle(x,y,r,0,cb)
    
    return v
enddef

lastMousePosY = 0
lastMousePosX = 0

def knob(v,x,y,min,max)

    radius = 7
    colH = colorHi
    colB = colorBase
    colRing = colorSlider

    id= x + (y * 200)

    v = remap(v,min,0,max,360)

    hover = isHover(x-radius,y-radius,radius*2,radius*2) AND NOT mouseWorking
    IF hover OR ( mouseWorking AND selected = id )THEN
        colH = colorHiHover
        colB = colorBaseHover
        colRing = colorSliderHover
        v=v+mouse.wheel();
    ENDIF

    IF mouse.down(0) AND hover AND NOT mouseWorking THEN
        lastMousePosX = mouse.x()
        lastMousePosY = mouse.y()

        mouseWorking = true
        selected = id
    ENDIF

    IF mouseWorking AND mouse.down(0) AND id = selected THEN
        v = v+mouse.x() - lastMousePosX;
        v = v+lastMousePosY - mouse.y();
        mouse.setpos(lastMousePosX, lastMousePosY)
    ENDIF

    IF mouseWorking AND mouse.released(0) THEN
        mouseWorking = false
        selected = 0
    ENDIF

    v = clamp(v,0,360)
    
    draw.ring(x,y,radius+3,radius+8,0,360,10,1,colorBase)
    draw.poly(x,y,8,radius,-v,0,colB)
    draw.poly(x,y,8,radius,-v,2,colH)
    draw.ring(x,y,radius+4,radius+7,0,v,10,1,colRing)

    v = remap(v,0,min,360,max)
    text = intToText("%03i", v)
    textPos = measureText(text, 1) / 2
    draw.text(text,  x - textPos, y-radius-17, 1, colH)

    return v
enddef

'Color Picker

def colorPiker(x,y,currentColor)
    border = 12
    id= x + (y * 200)
    
    hover = isHover(x,y,8,8)
    if hover then
        border = 15
        if (mouse.released(0)) and mustDrawColorPiker = false then
            mustDrawColorPiker = true
            colorPikerX = x
            colorPikerY = y
            colorPickSelected = currentColor;
            idPickSelected = id
        endif
    endif

    if (currentColor <> colorPickSelected and idPickSelected = id) then
        currentColor = colorPickSelected
        colorPickSelected = -1
        idPickSelected = -1
    endif 

    draw.rect(x,y,8,8,0,currentColor)
    draw.rect(x,y,8,8,1,border)
   
    return currentColor

enddef

def colorButton(x,y,c)
    border = 1
    hover = isHover(x ,y, 6, 6)

    if hover THEN
        border = 15
        if mouse.pressed(0) then
            colorPickSelected = c
            mustDrawColorPiker = false
        endif
    endif

    draw.rect(x ,y,6,6,0,c)
    draw.rect(x-1 ,y-1,8,8,1,border)
enddef

def drawColorPickerBox()
    x = colorPikerX
    y = colorPikerY
    hover = isHover(x,y,32,32)
    if not hover then
        mustDrawColorPiker = false
    endif
    draw.rect(x-1,y-1,34,34,1,15)
    draw.rect(x,y,32,32,0,0)
    for c = 0 to 15
        colorButton(x + (c mod 4)*8+1, y + floor(c / 4) * 8+1, c)
    next
enddef


endclass