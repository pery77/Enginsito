colorbutton = 0
colorbuttonHover = 5
colorButtonText = 12
colorButtonTextHover = 15
buttonW = 50
buttonH = 10

mouseWorking = false

def drawPalette(y)
    for i = 0 to 15
        x = i*20
        draw.rect(x,y,20,10,0,i)
        draw.rect(x,y,20,10,1,0)
        draw.text(intToText("%02i",i), x+6, y+9, 8, 0)
        draw.text(intToText("%02i",i), x+4, y+9, 8, 15)
    next
    draw.rect(0,y-8,45,11,0,0)
    draw.rect(0,y-8,45,11,1,11)
    draw.text(intToText("%03i,%03i",mouse.x(),mouse.y()), 0, y-8, 8, 11)
enddef

def drawmouse()
    draw.triangle(mouse.x(), mouse.y(), mouse.x(), mouse.y()+9, mouse.x()+7, mouse.y()+7, 0, 0)
    draw.triangle(mouse.x(), mouse.y(), mouse.x(), mouse.y()+9, mouse.x()+7, mouse.y()+7, 1, 15)
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

def button(x,y,txt)
    cb = colorbutton
    ct = colorButtonText
    hover = isHover(x,y,buttonW, buttonH)

    IF hover THEN 
        cb = colorbuttonHover
        ct = colorButtonTextHover 
        IF mouse.down() THEN 
            t = ct
            ct = cb
            cb = colorbutton
        ENDIF
    ENDIF

    draw.rect(x,y,buttonW,buttonH,0,cb)
    draw.rect(x,y,buttonW,buttonH,1,ct)
    xc = (buttonW * 0.5) - (measureText(txt,8) * 0.5)
    draw.text(txt,x + xc,y,8,ct)

    return (mouse.released(0) AND hover)
enddef

def slider(v,x,y,w,max)

    v = ROUND(w * v/max)
    cb = colorButtonText
    colN = 8
    colNH = 13
    col = colN

    mx = x+v+1
    my = y+1

    hover = isHover(x,y,w+10,buttonH)
    IF hover THEN
        col = colNH
        cb = colorButtonTextHover
        
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
    cb = colorButtonText
    col = colorButton
    colOn = 3

    hover = isHover(x-r,y-r,r*2,r*2)
    IF hover THEN
        cb = colorButtonTextHover
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

    r=7
    cb = colorButtonText
    col = colorButton
    id= x * 320 + y
    hover = isHover(x-r,y-r,r*2,r*2)

    IF hover THEN
        cb = colorButtonTextHover
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
    textPos = measureText(text, 8) / 2
    draw.text(text,  x - textPos, y-r-17,8, cb)
    
    draw.ring(x,y,r+3,r+8,0,360,10,1,0)
    draw.poly(x,y,8,r,-v,0,0)
    draw.poly(x,y,8,r,-v,2,cb)
    draw.ring(x,y,r+4,r+7,0,v,10,1,13)
    draw.ring(x,y,r+4,r+7,0,v,10,0,8)

    return v
enddef
