-KEYWRODS-
-========-
REM     -> ' Is used to add comments or explanatory notes within the code that are ignored by the interpreter.

MOD     -> ' Performs the modulo operation, which calculates the remainder after division.

AND     -> ' A logical operator used to perform a logical conjunction (also known as a logical AND) between two conditions.
OR      -> ' A logical operator used to perform a logical disjunction (also known as a logical OR) between two conditions.
NOT     -> ' A logical operator used to negate a conditionresulting in its opposite value.

IS      -> ' Used for comparison and checks if a value or variable is of a specific type or meets certain conditions.

NIL     -> ' Represents a null or empty value.

LET     -> ' Used to assign a value to a variable. (Optional)
DIM     -> ' It is used to declare and allocate memory for variables or arrays.

IF      -> ' Begins a conditional statement that executes a block of code if a certain condition is met.
THEN    -> ' Specifies the code to be executed if the condition of an "IF" statement is true.
ELSEIF  -> ' Used within an "IF" statement to specify an additional condition to be checked if the previous condition was false.
ELSE    -> ' Specifies the code to be executed if none of the previous conditions in an "IF" statement were true.
ENDIF   -> ' Marks the end of an "IF" statement.

FOR     -> ' Begins a loop that repeats a block of code for a specified number of times.
IN      -> ' Used in conjunction with the "FOR" keyword to specify the range or collection to iterate over.
TO      -> ' Specifies the upper limit of the range in a "FOR" loop.
STEP    -> ' Specifies the increment or decrement value for each iteration in a "FOR" loop.
NEXT    -> ' Marks the end of a "FOR" loop and proceeds to the next iteration.

WHILE   -> ' Begins a loop that repeats a block of code while a certain condition is true.
WEND    -> ' Marks the end of a "WHILE" loop.
DO      -> ' Begins a loop that repeats a block of code until a certain condition is true.
UNTIL   -> ' Specifies the condition for exiting a "DO" loop.
EXIT    -> ' Terminates a loop prematurely.

GOTO    -> ' Transfers the program execution to a specified line number or label.
GOSUB   -> ' Transfers the program execution to a specified subroutine or procedure and returns to the calling code afterwards.
RETURN  -> ' Marks the end of a subroutine or procedure and returns to the calling code.
CALL    -> ' Used to invoke a subroutine or function.

DEF     -> ' Begins the definition of a user-defined function.
ENDDEF  -> ' Marks the end of a user-defined function.

CLASS   -> ' Begins the definition of a user-defined class.
ENDCLASS-> ' Marks the end of a user-defined class.
ME      -> ' Refers to the current instance of a class.
NEW     -> ' Creates a new instance of a class.
VAR     -> ' Used to declare a variable within a class.

REFLECT -> ' Provides reflection capabilities to access and manipulate class members dynamically.
MEM     -> ' Refers to the memory address of a variable or object.
TYPE    -> ' Used to define a custom data type or structure.
END     -> ' Marks the end of the program or a code block.


Mathematical Functions
----------------------
ABS      -> ' Returns the absolute value of a number.
SGN      -> ' Returns the sign of a number.
SQR      -> ' Returns the square root of a number.
FLOOR    -> ' Returns the greatest integer not greater than a number.
CEIL     -> ' Returns the least integer not less than a number.
FIX      -> ' Returns the integer part of a number.
ROUND    -> ' Rounds a number to the nearest integer.
SRND     -> ' Sets the seed for the random number generator.
RND      -> ' Returns a random float number between [0.0, 1.0] with RND, or [0, max] with
            ' RND(max), or [MIN, MAX] with RND(min,max).
EXP      -> ' Returns the base-e exponential of a number.
LOG      -> ' Returns the natural logarithm (base e) of a specified number.

Trigonometric Functions
-----------------------
SIN  -> ' Returns the sine of an angle (in radians).
COS  -> ' Returns the cosine of an angle (in radians).
TAN  -> ' Returns the tangent of an angle (in radians).
ASIN -> ' Returns the arcsine of a number, resulting in an angle (in radians).
ACOS -> ' Returns the arccosine of a number, resulting in an angle (in radians).
ATAN -> ' Returns the arctangent of a number, resulting in an angle (in radians).

String Manipulation Functions
-----------------------------
ASC   -> ' Returns the ASCII code value of a character.
CHR   -> ' Returns the character associated with an ASCII code.
LEFT  -> ' Returns a specific number of characters from the left of a string.
MID   -> ' Returns a specific number of characters from the specific position of a string.
RIGHT -> ' Returns a specific number of characters from the right of a string.
STR   -> ' Converts a number to a string.
PRINT -> ' Displays output to the console or standard output device.

VAL   -> ' Returns the number representation of a string, or the value of a dictionary iterator,
         'overridable for referenced usertype and class instance.
LEN   -> ' Returns the length of a string or an array, or the element count of a LIST or a DICT,
         'overridable for referenced usertype and class instance.

Data Structures and Operations
------------------------------

LIST         -> ' Creates a list.
DICT         -> ' Creates a dictionary.
PUSH         -> ' Pushes a value to the tail of a list, overridable for
               '  referenced usertype and class instance.
POP          -> ' Pops a value from the tail of a list, overridable for
                '  referenced usertype and class instance.
BACK         -> ' Peeks the value at tail of a list, overridable for
                '  referenced usertype and class instance.
INSERT       -> ' Inserts a value at a specific position of a list,
                '  overridable for referenced usertype and class instance.
SORT         -> ' Sorts a list increasingly, overridable for referenced usertype and class instance.
EXISTS       -> ' Tells whether a list contains a specific value, or whether a dictionary contains 
                '  a specific key, overridable for referenced usertype and class instance.
INDEX_OF     -> ' Gets the index of a value in a list, overridable for referenced usertype and class instance.
GET          -> ' Returns the value at a specific index in a list, or the value with a specific key in a dictionary, 
                '  or a member of a class instance, overridable for referenced usertype and class instance.
SET          -> ' Sets the value at a specific index in a list, or the value with a specific key in a dictionary,
                '  or a member variable of a class instance, overridable for referenced usertype and class instance.
REMOVE       -> ' Removes the element at a specific index in a list, or the element with a specific key in a dictionary,
                '  overridable for referenced usertype and class instance.
CLEAR        -> ' Clears a list or a dictionary, overridable for referenced usertype and class instance.
CLONE        -> ' Clones a collection, or a referenced usertype
TO_ARRAY     -> ' Copies all elements from a list to an array
ITERATOR     -> ' Gets an iterator of a list or a dictionary,
                ' overridablefor referenced usertype and class instance.
MOVE_NEXT    -> ' Moves an iterator to next position over a list or a dictionary, 
                '  overridable for referenced user type and class instance.


- FLOW -
-======-
 * Gameflow Functions *
 INIT()     -> ' When the program start
 TICK()     -> ' In each frame if not paused
 DRAW()     -> ' In each frame after tick if not paused
 PAUSE()    -> ' In each frame only in pause mode
 CLOSE()      -> ' When the program end

* Time *
frame       -> ' number of frame since game started
delta       -> ' return the millisecond frame duration

-INPUTS-
-======-

* Mouse *
MOUSE_WHEEL()           -> ' get the whell value
MOUSE_PRESSED(button)	-> ' true if button is pressed
MOUSE_DOWN(button)      -> ' true if button is down
MOUSE_RELEASED(button)  -> ' true if button is released
MOUSE_UP(button)        -> ' true if button is up
MOUSE_SETPOS(x, y)      -> ' set the cursor at position x, y
    'button -> 0, left, 1 right, 2 middel, 3...

Mouse_X -> 'position x of mouse 
Mouse_Y -> 'position y of mouse 

* Keys *
KEY_PRESSED(key)    -> ' return true if key is pressed
KEY_DOWN(key)       -> ' return true if key is down
KEY_RELEASED(key)   -> ' return true if key is released
KEY_UP(key)         -> ' return true if key is up
KEY_GET()           -> ' return the key code pressed
KEY_CHAR()          -> ' return the char pressed

* Joystics *
JOY_ISAVIABLE(id)           -> ' return true if the joy id is ok
JOY_NAME(id)                -> ' return the name of the joystic id
JOY_PRESSED(id, button)     -> ' return true if button is pressed
JOY_DOWN(id, button)        -> ' return true if button is down
JOY_RELEASED(id, button)    -> ' return true if button is released
JOY_UP(id, button)          -> ' return true if button is up
JOY_GET()                   -> ' return the last joy button pressed
JOY_AXISCOUNT(id)           -> ' return how many axis the joy have
JOY_AXISVALUE(id, axisId)   -> ' return axis value
Tools
-----

FORMATTEXT(txt, value)  -> ' return formated text "%i" interger "%f" float
FONTSPACE(space)        -> ' set font separation in pixels
TEXTSIZE(txt, size)     -> ' return the size in pixels of the text at current text size (1 - 4)

PEEK(direction)           -> 'return the memory value at this direction
POKE(direction, newValue) -> 'set the memory value at this direction

- DRAW -
-======-
CLS(col)            -> ' Clear screen.
PIXEL(x, y, col)
LINE(x1, y1, x2, y2, thick, col)
CIRCLE(x, y, radius, style, col)
RING(x, y, radioIn, radioOut, startAngle, endAngle, segments, style, col)
ELLIPSE(x, y, rh, rv, style, col)
TRIANGLE(x1, y1, x2, y2, x3, y3, style col)
RECT(x, y, w, h, thick, col)
RECTROUND(x, t, w, h, thick, roundnes, segments, col)
POLY(x, y, sides, radius, rotation, thick, col)
TEXT(string, x, y, size, col)
SPRITE(id, x, y, col, [flags])
META(id, x, y)

' x,y ,x1,y1 >... positions
' col > color (0-15)
' style 0 fill, other line thickness
' segments > quality
' Flags > rotation 0-1-2-3 flip h + 8, flip v + 16

- SOUND -
-=======-

CH_PRESET(ch, preset)   -> 'Set channel preset 
CH_SET(ch, sequence)    -> 'Set sequence in this channel
CH_ON(ch, note, volume) -> 'Start sound until stop it whit ch_off()
CH_OFF(ch)              -> 'Stop note in this channel
CH_PLAY(ch, [sequence]) -> 'Play Channel optional -> and set [sequence] optional
CH_STOP(ch)             -> 'Stop Channel
CH_POS(ch)              -> 'Return current song position
CH_SIZE(ch)             -> 'Return channel song size
CH_FRAME(ch, frame)     -> 'Return average chanel frame
CH_GETNOTE(ch)          -> 'Return last note name

'sequence is a string in format MML (Music Macro Language)

' A - G	    note on	        C(C4) D4.(D4+8) C12C12C12(triplets)
' + or #	sharp	
' -	        flat	
' =	        natural	
' R	        rest	        R1 (whole rest)
' O	        octave	        O0 ... O8 (O4 default)
' > <	    octave up/down	
' L	        length	        L4 default
' Q	        note off ratio	n/8 (Q8 default)
' ^	        tie	            C4^16
' &	        no note off	
' T	        tempo	        T120 default
' V	        volume	        V0 ... V127(max) V+10 V-10
' KJ KI	    transpose(maJor/mInor)	KJg(G major)
' [ ]	    loop	        [...]4 (repeat 4 times)
' :	        skip on the last loop	
' @	        program change



- EDITOR KEYS -
-=============-

ESC -> ' Pause Game 
F1  -> ' Toggle Editor
F2  -> ' Adjust windows size and center
F11 -> ' Toggle Fullscreen
F12 -> ' Take Screenshoot