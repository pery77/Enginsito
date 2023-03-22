import "assets/lib/ui.bas"
'Tetris




def mario()
    m1 = "T100 @1 v30 >>e16-e8-e8-c16-e8-g4-<g4>-c8.<g8.e8.a8b8a+16a8g16.>e16g16.a8f16g8e8c16d16<b8.>c8.<g8.e8.a8b8a+16a8g16.>e16g16.a8f16g8e8c16d16<b4&b16>g16f+16f16d+8e8<g+16a16>c8<a16>c16d8.g16f+16f16d+8e8>c8c16c4.<g16f+16f16d+8e8<g+16a16>c8<a16>c16d8.d+8.d8.c2&c8g16f+16f16d+8e8<g+16a16>c8<a16>c16d8.g16f+16f16d+8e8>c8c16c4.<g16f+16f16d+8e8<g+16a16>c8<a16>c16d8.d+8.d8.c2"
    m2 = "T100 @0 v100 <d16d8d8d16d8g4<g4>g8.e8.c8.f8g8f+16f8c16.>c16e16.f8d16e8c8<a16b16g8.g8.e8.c8.f8g8f+16f8c16.>c16e16.f8d16e8c8<a16b16g8.c8.g8.>c8<f8.>c16c16c16<f8c8.e8.g16>c4.&c16<g8c8.g8.>c8<f8.>c16c16c16<f8c8g+8.a+8.>c8.<g16g8c8c8.g8.>c8<f8.>c16c16c16<f8c8.g8.g16>c4.&c16<g8c8.g8.>c8<f8.>c16c16c16<f8c8g+8.a+8.>c8."
    m3 = "T100 @3 v30 >>e16-e8-e8-c16-e8-g4-<g4>-c8.<g8.e8.a8b8a+16a8g16.>e16g16.a8f16g8e8c16d16<b8.>c8.<g8.e8.a8b8a+16a8g16.>e16g16.a8f16g8e8c16d16<b4&b16>g16f+16f16d+8e8<g+16a16>c8<a16>c16d8.g16f+16f16d+8e8>c8c16c4.<g16f+16f16d+8e8<g+16a16>c8<a16>c16d8.d+8.d8.c2&c8g16f+16f16d+8e8<g+16a16>c8<a16>c16d8.g16f+16f16d+8e8>c8c16c4.<g16f+16f16d+8e8<g+16a16>c8<a16>c16d8.d+8.d8.c2"
    m4 = "T100 @5 v70 <<d16d8d8d16d8g4<g4>g8.e8.c8.f8g8f+16f8c16.>c16e16.f8d16e8c8<a16b16g8.g8.e8.c8.f8g8f+16f8c16.>c16e16.f8d16e8c8<a16b16g8.c8.g8.>c8<f8.>c16c16c16<f8c8.e8.g16>c4.&c16<g8c8.g8.>c8<f8.>c16c16c16<f8c8g+8.a+8.>c8.<g16g8c8c8.g8.>c8<f8.>c16c16c16<f8c8.g8.g16>c4.&c16<g8c8.g8.>c8<f8.>c16c16c16<f8c8g+8.a+8.>c8."

    music.set(0,m1)
    music.set(1,m2)
    music.set(2,m3)
    music.set(3,m4)
enddef

def tetris()
    t1 = "t130 @3 v60 >e4<b8>c8d8e16d16c8<b8a4a8>c8e4d8c8<b4.>c8d4e4c4<a4a2r8>d4f8a4g8f8e4.c8e4d8c8<b4b8>c8d4e4c4<a4a4r4"
    t2 = "t130 @0 v80 o2e8>e8<e8>e8<e8>e8<e8>e8<a8>a8<a8>a8<a8>a8<a8>a8<g+8>g+8<g+8>g+8<e8>e8<e8>e8<a8>a8<a8>a8<a8>a8<b8>c8d8<d8r8d8r8d8a8f8c8>c8r8c8<c8g8g8r8b8>b8r8b8r8e8r8g+8<a8>e8<a8>e8<a4r4"

    music.set(0,t1)
    music.set(1,t2)
    music.set(2,"")
    music.set(3,"")
enddef

def castel()
    c1 = "T180 v80r2>ag>d<a+ag>d<a+ag>d<a+ag>d<a+ag>d<a+ag>d<a+agd<a+agd<a+4&a+>g4a4a+4a4f4a4g2g4a4a+4>c4<a+4a4g2g4a4a+4a4f4a4g8.
    &g32a+4.&a+32>c4.<a+4a8.a+>c4f2t110gd>d<d>c<da+dada+dadgdada+d>c<da+dadfdadgdgd>d<d>c<da+dada+dadgdada+d>c<da+dadfdadgdgd>d<d>c
    <da+dada+dadgdada+d>c<da+dadfdadgdgd>d<d>c<da+dada+dadgdada+d>c<da+dadfdadg2&g8t100c8fd2&dc8<a+4&a+>f8.d2&d32c32d32c32<a+2.>c8
    <a+8f+4a+4>c4d+4d2.&dgaa+agfa8gf2&fd2&d>d+2d4c8<a+8a8a+8>c8<a8t110gd>d<d>c<da+dada+dadgdada+d>c<da+dadfdadgdgd>d<d>c<da+dada+da
    dgdada+d>c<da+dadfdadgdgd>d<d>c<da+dada+dadgdada+d>c<da+dadfdadgdgd>d<d>c<da+dada+dadgdada+d>c<da+dadfdadgdc8fd2&dc8<a+8>c8.f8.d4.
    <a8g8>c8fd2&dc8d8d+8.f4&fd4d+4c8fd2&dc8<a+8>c8.f8.d4.<a8g8>c8fd2&dc8d8d+8.f4&fd8f+8g8a8a8.g4&ga8.g4&ga8.g4&ga+8a8g8f8a8.g8.d8a8.g8
    .d8a8.g8.d8a+8>c8<a8a+8gd>d<d>c<da+dada+dadgdada+d>c<da+dadfdadgdgd>d<d>c<da+dada+dadgdada+d>c<da+dadfdadgdgd>d<d>c<da+dada+dadgdada+d>c
    <da+dadfdadgdgd>d<d>c<da+dada+dadgdada+d>c<da+dadfda8g32a32a+32>c32d<ga+g>c<g>d<g>c<faf>f<f>dd+d2<c4<f.g.a>d<ga+g>c<g>d<g>c<faf>f<f>dd+d4.
    <d+fd+4d4>d+<g+bg+>c+<g+>d+<g+>c+<f+a+f+>f+<f+>d+ed+2c+4<f+.g+.a+>d+<g+bg+>c+<g+>d+<g+>c+<f+a+f+>f+<f+>d+ed+4.d+ef+c+ec+d+fga+g+d+>d+<d+>
    c+<d+bd+a+d+bd+a+d+g+d+a+d+bd+>c+<d+bd+a+d+f+d+a+d+g+d+g+d+>d+<d+>c+<d+bd+a+d+bd+a+d+g+d+a+d+bd+>c+<d+bd+a+d+f+d+a+d+g+d+g+d+>d+<d+>c+<d
    +bd+a+d+bd+a+d+g+d+a+d+bd+>c+<d+bd+a+d+f+d+a+d+g+d+g+d+>d+<d+>c+<d+bd+a+d+bd+a+d+g+d+a+d+bd+>c+<d+bd+a+d+f+d+a+d+g+d+c+8f+d+2&d+c+8<b8>c
    +8.f+8.d+4.<a+8g+8>c+8f+d+2&d+c+8d+8e8.f+4&f+d+4e4c+8f+d+2&d+c+8<b8>c+8.f+8.d+4.<a+8g+8>c+8f+d+2&d+c+8d+8e8.f+4&f+d+8g8g+8a+8a+8.g+4&g+
    a+8.g+4&g+a+8.g+4&g+b8a+8g+8f+8a+8.g+8.d+8a+8.g+8.d+8a+8.g+8.d+8b8>c+8<a+8b8a+8.g+4&g+a+8.g+4&g+a+8.g+4&g+b8a+8g+8f+8a+8.g+8.d+8a+8.g+8.
    d+8a+8.g+8.d+8b8>c+8<a+8b8g+d+>d+<d+>c+<d+bd+a+d+bd+a+d+g+d+a+d+bd+>c+<d+bd+a+d+f+d+a+d+g+d+g+d+>d+<d+>c+<d+bd+a+d+bd+a+d+g+d+a+d+bd+>c+
    <d+bd+a+d+f+d+a+d+g+d+g+d+>d+<d+>c+<d+bd+f+d+bd+a+d+g+d+a+d+bd+>c+<d+bd+>c+<d+f+d+a+d+g+d+>d+<d+>d+<d+>c+<d+bd+a+d+bd+a+d+g+d+a+d+bd+>c+<
        d+bd+a+d+f+d+a+2&a+o2g+b>g+d+bg+>d+<b>g+d+bg+>d+<b>g+d+bg+>d+<b>g+1&g+4.&g+,t80l16v13r2>d1d1o2t100g8>d8g8d8a8d8a+8d8a8d8f8d8a8d8g4<d+
       "
    music.set(0,c1)
    music.set(1,"")
    music.set(2,"")
    music.set(3,"")
enddef

'music.stop()
'music.play()

OSC_SINE = 0;
OSC_SQUARE50 = 1;
OSC_SQUARE25 = 2;
OSC_SQUARE12 = 3;
OSC_TRIANGLE = 4;
OSC_SAW_DIG = 5;
OSC_NOISE = 6;

def kettonote(k,o)
    IF key.pressed(k) THEN
        music.note(0,o,69,50)
    endif
    if key.released(k) then
        music.noteoff(0)
    endif
enddef

def tick()
    k = key.get()
    IF k <> 0 THEN
        print k;
    ENDIF

kettonote(49,OSC_SINE)
kettonote(50,OSC_SQUARE50)
kettonote(51,OSC_SQUARE25)
kettonote(52,OSC_SQUARE12)
kettonote(53,OSC_TRIANGLE)
kettonote(54,OSC_SAW_DIG)
kettonote(55,OSC_NOISE)

enddef

envA = 1
envD = 1
envS = 255
envR = 5
envAM =255
lfoN = 0
lfoA = 0

music.osc(0,1)
music.osc(1,0)

def draw()
    cls(1)
    time = time + delta()

    draw.rect(0,0,320,9,0,3)
    draw.text(intToText("Time:%06i",time), 1, 1, 1, 0)
    draw.text(intToText("Tick :%04i",music.Tick()), 128, 1, 1, 0)
    draw.text(intToText("Pos:%04i/(%04i)",music.position(0), music.size(0)), 128, 10, 1, 5)
    draw.text(intToText("Pos:%04i/(%04i)",music.position(1), music.size(1)), 128, 20, 1, 5)
    draw.text(intToText("Pos:%04i/(%04i)",music.position(2), music.size(2)), 128, 30, 1, 5)
    draw.text(intToText("Pos:%04i/(%04i)",music.position(3), music.size(3)), 128, 40, 1, 5)


    if ui.button(264,12,"Play") then
        music.play()
    endif
    if ui.button(264,24,"Stop") then
        music.stop()
    endif
    if ui.button(264,45,"Tetris") then
        Tetris()
    endif
    if ui.button(264,55,"Mario") then
        mario()
    endif
    if ui.button(264,65,"Castel") then
        castel()
    endif
    'envA = ui.knob(envA,80,36,0,255)

    'note = ui.slider(note,40,180,120,120)
    envA = ui.knob(envA,80, 80,0,255)
    envD = ui.knob(envD,112,80,0,255)
    envS = ui.knob(envS,144,80,0,255)
    envR = ui.knob(envR,176,80,0,255)
    envAM = ui.knob(envAM,222,80,0,255)
    lfoN = ui.knob(lfoN,262,80,0,255)
    lfoA = ui.knob(lfoA,292,80,0,255)

    music.env(0,envA, envD, envS, envR, envAM)
    music.lfo(0,lfoN, lfoA)

    IF mouse.down(1) THEN  ui.drawPalette() ENDIF
    IF NOT ui.mouseWorking THEN  ui.drawmouse() ENDIF

enddef