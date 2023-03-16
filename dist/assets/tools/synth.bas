import "assets/lib/ui.bas"

wave= 0
note = 69
envA = 0
envT = 76
envP = 0
envR = 102
fSlide = 0
fDelta = 0
vibratoD = 0
vibratoS = 0
toneAmount = 0 
toneSpeed = 0
toneSquare = 0
toneDuty = 0
repSp = 0
repOf = 0
repSw = 0
lpfCutoff = 255
lpfSweep = 0
lpfRes = 0
hpfCutoff = 0
hpfSweep = 0

bgCol = 1

def tick()
    k = key.get()
    IF k <> 0 THEN 
    ENDIF

    IF key.pressed(32) THEN
        sfx.env(currentSound, envA, envT, envP, envR)
        sfx.freq(currentSound, fSlide, fDelta, vibratoD, vibratoS)
        sfx.tone(currentSound, toneAmount, toneSpeed, toneSquare, toneDuty)
        sfx.repeat(currentSound, repSp, repOf, repSw)
        sfx.filter(currentSound, lpfCutoff, lpfSweep, lpfRes, hpfCutoff, hpfSweep)
        sfx.wave(currentSound, wave)
        sfx.render(currentSound, note)
        sfx.play(currentSound) 

    ENDIF  

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
        DRAW.text(intToText("%i", code),x,y + size,1,12)
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
    DRAW.text(intToText("%02i", x/20),x+4,y + size+1,1,14)

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
    wave = ui.slider(wave,x+8,y,16,3)

    FOR i = 0 TO 3
        onColor = 0
        IF i = wave THEN
            onColor = 13
        ENDIF
        DRAW.circle(x+i*14-8, y+16, 3, 1, onColor)
        DRAW.circle(x+i*14-8, y+16, 3, 0, 2)
    NEXT

    Draw.rect(x-17,y-7,60,30,2,0)
    Draw.rect( x-4, y-12,34,10,0,bgCol)
    Draw.text("Wave", x-2, y-10,1,0)

ENDDEF

currentSound = 0
def draw()
    cls(bgCol)
    time = time + delta()

    draw.rect(0,0,320,9,0,0)
    draw.text(intToText("T: %06i",time), 0, 0, 1, 3)

    drawWave(260,25)
    envA = ui.knob(envA,80,36,0,255)
    envT = ui.knob(envT,112,36,0,255)
    envP = ui.knob(envP,144,36,0,255)
    envR = ui.knob(envR,176,36,0,255)
    
    note = ui.slider(note,40,180,120,120)
    currentSound = ui.slider(currentSound,20,12,32,16)

    fSlide = ui.knob(fSlide,80, 78,-127,127)
    fDelta = ui.knob(fDelta,112,78,-127,127)
    vibratoD = ui.knob(vibratoD,144,78,0,255)
    vibratoS = ui.knob(vibratoS,176,78,0,255)

    toneAmount = ui.knob(toneAmount,80, 120,-127,127)
    toneSpeed = ui.knob(toneSpeed,112,120,0,255)
    toneSquare = ui.knob(toneSquare,144,120,0,255)
    toneDuty = ui.knob(toneDuty,176, 120,-127,127)

    repSp = ui.knob(repSp,220,78,0,255)
    repOf = ui.knob(repOf,252,78,-127,127)
    repSw = ui.knob(repSw,284,78,-127,127)

    lpfCutoff = ui.knob(lpfCutoff,80, 162, 0, 255)
    lpfSweep = ui.knob(lpfSweep,112,162,-127,127)
    lpfRes = ui.knob(lpfRes,144,162,0,255)
    hpfCutoff = ui.knob(hpfCutoff,176, 162,0,255)
    hpfSweep = ui.knob(hpfSweep,208, 162,-127,127)

    IF ui.button(256,176,"Save") THEN 
    for currentSound = 0 to 15
        sfx.env(currentSound, envA, envT, envP, envR)
        sfx.freq(currentSound, fSlide, fDelta, vibratoD, vibratoS)
        sfx.tone(currentSound, toneAmount, toneSpeed, toneSquare, toneDuty)
        sfx.repeat(currentSound, repSp, repOf, repSw)
        sfx.filter(currentSound, lpfCutoff, lpfSweep, lpfRes, hpfCutoff, hpfSweep)
        sfx.wave(currentSound, wave)
        sfx.save(currentSound)
        sfx.render(currentSound, note)
        dumpMemory("bios.bin")
    next
    ENDIF

    IF mouse.down(1) THEN  ui.drawPalette() ENDIF
    IF NOT ui.mouseWorking THEN  ui.drawmouse() ENDIF

enddef