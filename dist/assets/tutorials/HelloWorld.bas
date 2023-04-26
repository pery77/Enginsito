'Wellcome to peryEngine, this is a comment, 
'is ignored and used for human info.

'This is the draw function,
'peryEngine call it 60 times per second.
def draw()
    'This line clear screen with the color of the parameter, 
    'in this case is 0 (black)
    cls(0) 
    'Whit that we draw a text ("Hello World") 
    'in the position X = 104, and y = 40
    '2 is the size (the minimun is 1) and 3 is the color (white), 
    'colors go from 0 to 15
    draw.text("Hello World!",65,30,2,3) 
    'Another fancy text with usefull information.
    draw.text("Press ESCAPE key to close.",55,55,1,2)
    draw.text("ESCAPE",55 + 6*8,55,1,4)
enddef

def pause()
	cls(8)
	draw.text("Paused!",65,30,2,3
enddef



