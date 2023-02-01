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

def drawPalette()
    y=200
    for i = 0 to 15
        x = i*20
        draw.rect(x,y-10,20,10,0,i)
        draw.font(intToText("%02i",i), x+3, y-8, 8, 0)
        draw.font(intToText("%02i",i), x+5, y-8, 8, 0)
        draw.font(intToText("%02i",i), x+4, y-7, 8, 0)
        draw.font(intToText("%02i",i), x+4, y-9, 8, 0)
        draw.font(intToText("%02i",i), x+4, y-8, 8, 15)
    next
    draw.rect(0,y-21,59,11,0,0)
    draw.rect(0,y-21,59,11,1,11)
    draw.font(intToText("%03i,%03i",mouse.x(),mouse.y()), 2, y-19, 8, 11)
enddef

def drawMouse()
    draw.triangle(mouse.x(), mouse.y(), mouse.x(), mouse.y()+10, mouse.x()+7, mouse.y()+7, 0, mouseColorBorder)
    draw.triangle(mouse.x(), mouse.y(), mouse.x(), mouse.y()+9, mouse.x()+7, mouse.y()+7, 1, mouseColor)
enddef

'tools
def isHover(x,y,w,h)
    return mouse.x() > x AND mouse.y() > y AND mouse.x() < x + w AND mouse.y() < y + h
enddef
angle = 3.141592*5
def polar2cartX(r,a)
    x = r * cos(a/angle)
    return x
enddef
def polar2cartY(r,a)
    y = r * sin(a/angle)
    return y
enddef
def clamp(v,min,max)
    IF v < min THEN return min ENDIF
    IF v > max THEN return max ENDIF
    return v
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
    xc = (buttonW * 0.5) - (measureFont(txt,8) * 0.5)
    draw.font(txt,x + xc,y+2,8,colH)

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
    
    draw.rect(x,y,w + 10,buttonH,0,0)
    draw.rect(x,y,w + 10,buttonH,1,cb)
    draw.rect(mx,my,8,8,0,col)
    textPos = intToText("%02i",v)
    draw.text(textPos, x - measureText(textPos ,8)-2, y,8, cb)

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

def knob(v,x,y)

    radius = 7
    colH = colorHi
    colB = colorBase
    colRing = colorSlider

    id= x + (y * 200)

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
    
    x1= polar2cartX(r+5,-v)
    y1= polar2cartY(r+5,-v)

    text = intToText("%03i", v)
    textPos = measureFont(text, 8) / 2
    draw.font(text,  x - textPos, y-radius-17,8, colH)
    
    draw.ring(x,y,radius+3,radius+8,0,360,10,1,colorBase)
    draw.poly(x,y,8,radius,-v,0,colB)
    draw.poly(x,y,8,radius,-v,2,colH)
    draw.ring(x,y,radius+4,radius+7,0,v,10,1,colRing)


    return v
enddef

endclass