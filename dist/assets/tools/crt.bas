import "assets/lib/ui.bas"
ui.buttonW = 30

blurPower = 50
blurFactor = 170
chromatic = 80
curvature = 100
vignetting = 50
scanline = 50
grille = 0
vLine = 10
gForce = 255

def draw()
    cls(0)

    for id = 0 to 15
        x=id*20
        draw.rect(x,0,19,32,0,id)
    next
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
    //grille = ui.knob(grille,80,48,0,2)

    chromatic = ui.knob(chromatic,20,88,0,255)
    curvature = ui.knob(curvature,50,88,0,255)
    vignetting = ui.knob(vignetting,80,88,0,255)

    scanline = ui.knob(scanline,20,128,0,255)
    vLine = ui.knob(vLine,50,128,0,255)
    //gForce = ui.knob(gForce,80,128,0,255)

    crt.blurPower(blurPower)

    crt.blurFactor(blurFactor)
    crt.chromatic(chromatic)
    crt.curvature(curvature)
    crt.vignetting(vignetting)
    crt.scanline(scanline)
    crt.grille(grille)
    crt.verticalLine(vLine)
    crt.grilleForce(gForce)


    IF mouse.down(2) THEN ui.drawPalette() ENDIF
    ui.drawmouse()
enddef