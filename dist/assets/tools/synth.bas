import "assets/lib/ui.bas"
    envA = 0
    envD = 30
    envS = 0
    envR = 40
    wave= 0

def tick()

 k = key.get()
    IF k <> 0 THEN 
        sfx.set(0,wave,k-30,envA,envD,envS,envR)
        sfx.play(0, 1000)
    ENDIF
    IF key.pressed(32) THEN sfx.play(0, 1000) ENDIF  
    IF key.pressed(65) THEN sfx.play(1, 1000) ENDIF  
enddef

soundOn = false
instrument = 0
DEF sKey(id)

    x = id * 10
    y = 120
    size = 50
    t = 0
    IF id MOD 2 <> 0 THEN 
        t = 1
    ENDIF

    borderCol = 0
    keyBCol = 5
    code = id + 56
    IF t = 0 THEN
    y1 = y+20
        hover = ui.isHover(x,y1 + size / 2,20,size)
        IF hover THEN
            borderCol = 14
        ENDIF
        DRAW.rect(x,y1,20,size,0,12)
        DRAW.rect(x,y1,20,size,1,borderCol)
        DRAW.font(intToText("%i", code),x,y + size,8,12)
    ENDIF
    IF t = 1 THEN
        
        hover = ui.isHover(x,y+1,14,size * 0.65)
        IF hover THEN
            keyBCol = 14
        ENDIF
        DRAW.rect(x,y+1,14,size * 0.65,0,0)
        DRAW.rect(x,y+1,14,size * 0.65,2,keyBCol) 
    ENDIF
'IF key.pressed(72) THEN sound.note(80,0,100) ENDIF
    IF hover AND mouse.pressed(0) THEN
        sound.note(code,instrument,100)
        soundOn = true;
    ENDIF
    IF mouse.released(0) and soundOn THEN
        sound.stop(code,instrument)
        soundOn = false;
    ENDIF

enddef

def synthKeys()

    FOR id = 0 TO 30
        sKey(id)
    NEXT


enddef

buttonSelected = 0
DEF soundButton(id,x,y,size)

    borderCol = 1
    keyBCol = 5
    selColor = 0

    hover = ui.isHover(x,y,20,size)
    IF hover THEN
        borderCol = 14
    ENDIF
    
    IF id = buttonSelected THEN
        selColor = 3
    ENDIF

    DRAW.rect(x,y,20,size,0,12)
    DRAW.rect(x,y,20,size,3,selColor)
    DRAW.rect(x+1,y+1,20-2,size-2,2,borderCol)
    DRAW.text(intToText("%02i", x/20),x+4,y + size+1,8,14)

    DRAW.circle(x+10,y+5,3,1,selColor)
    DRAW.circle(x+10,y+5,3,0,14)

    IF hover AND mouse.released(0) THEN
        buttonSelected = id
    ENDIF

ENDDEF

DEF drawSoundButtons(x)
    y = 160
    size = 20

    DRAW.rect(0,y+size+1,320,10,0,0)
    FOR i = 0 TO 15
        x = i*20
        soundButton(i,x,y,size)S
    NEXT
ENDDEF

wave = 0
DEF drawWave(x,y)
    wave = ui.slider(wave,x,y,8,3)

    FOR i = 0 TO 3
        onColor = 0
        IF i = wave THEN
            onColor = 3
        ENDIF
        DRAW.circle(x-10,15+y+i*10,3,1,onColor)
        DRAW.circle(x-10,15+y+i*10,3,0,12)
    NEXT

    Draw.rect(x-17,y-7,60,60,2,0)
    Draw.rect( x-4, y-12,30,10,0,1)
    Draw.text("Wave", x-2, y-12,8,0)

ENDDEF

def draw()
    cls(2)
    time = time + delta()
    draw.rect(0,0,320,9,0,0)
    draw.text(intToText("T: %06i",time), 0, 0, 1, 15)
    draw.text(intToText("D: %04i", delta()), 54, 0, 1, 15)
    draw.text(intToText(">: %03i", envD ), 160, 0, 1, 15)

    'IF button (2,16,"play") THEN print sfx.play(0,1000) ENDIF
'
    'IF button (2,28,"square") THEN print sfx.set(0,0) ENDIF
    'IF button (2,38,"sawtooth") THEN print sfx.set(0,1) ENDIF
    'IF button (2,48,"sine") THEN print sfx.set(0,2) ENDIF
    'IF button (2,58,"noise") THEN print sfx.set(0,3) ENDIF

    'p = slider(p,80,16,128,255)

    instrument = ui.slider(instrument,17,85,41,16)

    drawWave(20,30)

    envA = ui.knob(envA,80,36,0,100)
    envD = ui.knob(envD,112,36,0,100)
    envS = ui.knob(envS,144,36,0,100)
    envR = ui.knob(envR,176,36,0,100)

    lx = 77
    ly = 88
    h = 20
    w = 20
    s = 20
    lx1 = lx+envA * (w / 100)
    ly1 = ly-h
    lx2 = lx1 + envD  * (w / 100)
    ly2 = h + ly1 - envS * (h / 100)
    lx3 = lx2 + envR * (w / 100) + s
    draw.line(lx,ly,lx1,ly1,2,14)
    draw.line(lx1,ly1,lx2,ly2,2,14)
    draw.line(lx2,ly2,lx2+s,ly2,2,14)
    draw.line(lx2+s,ly2,lx3,ly,2,14)


    c = ui.toogle(c,5,110,3)
    e = ui.toogle(e,20,110,3)
IF c THEN
    synthKeys()
    ELSE
    drawSoundButtons()
ENDIF
    IF mouse.down(1) THEN  ui.drawPalette() ENDIF
    IF NOT ui.mouseWorking THEN  ui.drawmouse() ENDIF

enddef