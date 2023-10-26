'TEXTSIZE(text, scale)
'Return the size in pixels of the text at current text
'scale is from 1 to 4

hello1 = textSize("Hello",1)
hello2 = textSize("Hello",2)

def draw()
	cls(0)
	text("Hello",50,50,1,3)
	rect(50,50,hello1,8,1,13)
	
	text("Hello",50,80,2,3)
	rect(50,80,hello2,16,1,13)
enddef