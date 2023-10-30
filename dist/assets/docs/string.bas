'[
ASC   ->  Returns the ASCII code value of a character.
CHR   ->  Returns the character associated with an ASCII code.

LEFT  ->  Returns a specific number of characters from the left of a string.
MID   ->  Returns a specific number of characters from the specific position of a string.
RIGHT ->  Returns a specific number of characters from the right of a string.

STR   ->  Converts a number to a string.
VAL   -> ' Returns the number representation of a string, or the value of a dictionary iterator,
         'overridable for referenced usertype and class instance.
LEN   -> ' Returns the length of a string or an array, or the element count of a LIST or a DICT,
         'overridable for referenced usertype and class instance.
         
PRINT ->  Displays output to the console or standard output device.
']

print asc("*") 'print 42, the value of *
print chr(42)  'print *, the character 42

print left ("abcdefg", 3) 'print abc 3 from left
print right("abcdefg", 3) 'print efg 3 from right
print mid  ("abcdefg", 4, 2) 'print de 4 from left and 2 characters

print str(234.32) 			'convert this number in a string
print val("234.32") - 200 	'convert this string in a number

print len("abcdefg") 'how long is this string