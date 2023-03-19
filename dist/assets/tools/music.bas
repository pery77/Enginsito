import "assets/lib/ui.bas"
sfx.wave(1,2)
'Tetris
'music.set(0, "t130 @0 >e4<b8>c8d8e16d16c8<b8a4a8>c8e4d8c8<b4.>c8d4e4c4<a4a2r8>v120d4f8a4g8f8e4.c8e4d8c8<b4b8>c8d4e4c4<a4a4r4")
'music.set(1, "t130 @0 o2e8>e8<e8>e8<e8>e8<e8>e8<a8>a8<a8>a8<a8>a8<a8>a8<g+8>g+8<g+8>g+8<e8>e8<e8>e8<a8>a8<a8>a8<a8>a8<b8>c8d8<d8r8d8r8d8a8f8c8>c8r8c8<c8g8g8r8b8>b8r8b8r8e8r8g+8<a8>e8<a8>e8<a4r4")
'Mario
'music.set(0,"T100 @3 >>e16-e8-e8-c16-e8-g4-<g4>-c8.<g8.e8.a8b8a+16a8g16.>e16g16.a8f16g8e8c16d16<b8.>c8.<g8.e8.a8b8a+16a8g16.>e16g16.a8f16g8e8c16d16<b4&b16>g16f+16f16d+8e8<g+16a16>c8<a16>c16d8.g16f+16f16d+8e8>c8c16c4.<g16f+16f16d+8e8<g+16a16>c8<a16>c16d8.d+8.d8.c2&c8g16f+16f16d+8e8<g+16a16>c8<a16>c16d8.g16f+16f16d+8e8>c8c16c4.<g16f+16f16d+8e8<g+16a16>c8<a16>c16d8.d+8.d8.c2")
'music.set(1,"T100 @88 <d16d8d8d16d8g4<g4>g8.e8.c8.f8g8f+16f8c16.>c16e16.f8d16e8c8<a16b16g8.g8.e8.c8.f8g8f+16f8c16.>c16e16.f8d16e8c8<a16b16g8.c8.g8.>c8<f8.>c16c16c16<f8c8.e8.g16>c4.&c16<g8c8.g8.>c8<f8.>c16c16c16<f8c8g+8.a+8.>c8.<g16g8c8c8.g8.>c8<f8.>c16c16c16<f8c8.g8.g16>c4.&c16<g8c8.g8.>c8<f8.>c16c16c16<f8c8g+8.a+8.>c8.")

''music.set(2,"")
'music.set(3,"")
'music.set(0,"T140 o2 cdefgab>c")
'music.set(1,"T140 o3 cdefgab>c")
'music.set(1,"T140 O3 cdefgab>c")
music.set(2,"T140 @22 O6 cdefgab>c")
music.stop()
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