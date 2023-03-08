import "assets/lib/ui.bas"
ui.buttonW = 30

blurPower = 36
blurFactor = 36
chromatic = 48
curvature = 90
vignetting = 24
scanline = 24
grille = 0
gScale = 20
gForce = 20

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
    grille = ui.knob(grille,80,48,0,2)

    chromatic = ui.knob(chromatic,20,88,0,255)
    curvature = ui.knob(curvature,50,88,0,255)
    vignetting = ui.knob(vignetting,80,88,0,255)
    scanline = ui.knob(scanline,110,88,0,255)
    
    gScale = ui.knob(gScale,20,128,0,255)
    gForce = ui.knob(gForce,50,128,0,255)
    crt.blurPower(blurPower)

    crt.blurFactor(blurFactor)
    crt.chromatic(chromatic)
    crt.curvature(curvature)
    crt.vignetting(vignetting)
    crt.scanline(scanline)
    crt.grille(grille)
    crt.grilleScale(gScale)
    crt.grilleForce(gForce)


    IF mouse.down(2) THEN ui.drawPalette() ENDIF
    ui.drawmouse()
enddef