import "assets/lib/ui.bas"
ui.buttonW = 30

blurPower = 16
blurFactor = 16
chromatic = 32
curvature = 32
vignetting = 32
scanline = 32

def draw()
    cls(0)
    for x = 1 to 31
        for y = 1 to 19
            draw.line(x*10,0,x*10,200,1,1)
            draw.line(0,y*10,320,y*10,1,1)
        next
    next
    draw.rect(0,0,320,200,1,3)


    if ui.button (3,3,"On") then crt.enabled(true) endif
    if ui.button (33,3,"Off") then crt.enabled(false) endif

    blurPower = ui.knob(blurPower,20,48,0,255)
    blurFactor = ui.knob(blurFactor,50,48,0,255)

    chromatic = ui.knob(chromatic,20,88,0,255)
    curvature = ui.knob(curvature,50,88,0,255)
    vignetting = ui.knob(vignetting,80,88,0,255)
    scanline = ui.knob(scanline,110,88,0,255)
    
    crt.blurPower(blurPower)
    crt.blurFactor(blurFactor)
    crt.chromatic(chromatic)
    crt.curvature(curvature)
    crt.vignetting(vignetting)
    crt.scanline(scanline)


    IF mouse.down(2) THEN ui.drawPalette() ENDIF
    ui.drawmouse()
enddef