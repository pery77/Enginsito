let a = 2 ' Assignment statement begins with LET
pi = 3.14 ' Another assignment statement without LET

print a * pi

dim nums(10)    ' arrays
dim strs$(2, 2) ' arrays multidimensional, maximum 4 dimensions

nums(6) = 99

print nums(5) ' 0 because none is stored in position 5
print nums(6) ' 99 because we stored it before

strs$(0,0) = "hello"
strs$(0,1) = "world"
strs$(1,0) = "hola"
strs$(1,1) = "mundo"

lang = 1 ' English 0 - Spanish 1

print strs$(lang, 0) + " " + strs$(lang, 1)

print f 'no setted value is 0
f = nil
print f 'now is nil (null) not value
f = 1
print f
let f = 2 'you can use let if you want
print f

'you can compare classes whit IS (see OOP documentation)