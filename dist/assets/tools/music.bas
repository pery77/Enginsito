import "assets/lib/ui.bas"

music.set("t130>e4<b8>c8d8e16d16c8<b8a4a8>c8e4d8c8<b4.>c8d4e4c4<a4a2r8>d4f8a4g8f8e4.c8e4d8c8<b4b8>c8d4e4c4<a4a4r4.o2e8>e8<e8>e8<e8>e8<e8>e8<a8>a8<a8>a8<a8>a8<a8>a8<g+8>g+8<g+8>g+8<e8>e8<e8>e8<a8>a8<a8>a8<a8>a8<b8>c8d8<d8r8d8r8d8a8f8c8>c8r8c8<c8g8g8r8b8>b8r8b8r8e8r8g+8<a8>e8<a8>e8<a4r4")

music.play()

def draw()
    cls(0)
    time = time + delta()

    draw.rect(0,0,320,9,0,3)
    draw.text(intToText("T:%06i",time), 1, 1, 1, 0)


    if ui.button(3,10,"Play") then
        music.play()
    endif
    if ui.button(3,22,"Stop") then
        music.stop()
    endif

    'envA = ui.knob(envA,80,36,0,255)

    'note = ui.slider(note,40,180,120,120)


    IF mouse.down(1) THEN  ui.drawPalette() ENDIF
    IF NOT ui.mouseWorking THEN  ui.drawmouse() ENDIF

enddef