import speech_recognition as sr
import pyttsx3
import turtle as T
import random
import numpy as np
import serial
r = sr.Recognizer()

global initial 
initial = (0,0)
global limit
limit = (50,0)
def speech():
    with sr.Microphone() as source2:
        print("Calibrating")

        r.adjust_for_ambient_noise(source2,duration=2)

        audio_save = r.listen(source2)
        audio_text = r.recognize_google(audio_save)
        audio_text = audio_text.upper()
        print(audio_text)
        return audio_text

def to_letters(letter):

    letters = ["A","B","C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"," " ]
   
    A = [(0,0), (0,2), (1,2), (1,1), (0,1), (1,1), (1,0), (3,0)]
    B = [(0,0), (0,2), (1,2), (1,1), (0,1), (1,1), (1,0), (0,0), (3,0)]
    C = [(0,0), (0,2), (1,2), (0,2), (0,0), (1,0), (3,0)]
    D = [(0,0), (0,2), (1,2), (1,0), (0,0), (3,0)]
    E = [(0,0), (0,1), (1,1), (0,1), (0,2), (1,2), (0,2), (0,0), (1,0), (3,0)]
    F = [(0,0), (0,1), (1,1), (0,1), (0,2), (1,2), (0,2), (0,0), (3,0)]
    G = [(0,0), (0,2), (1,2), (0,2), (0,0), (1,0), (1,1), (1,0), (3,0)]
    H = [(0,0), (0,2), (0,1), (1,1), (1,2), (1,0), (3,0)]
    I = [(0,0), (1,0), (1,2), (0,2), (2,2), (1,2), (1,0), (2,0), (4,0)]
    J = [(0,0), (0,1), (0,0), (1,0), (1,2), (0,2), (2,2), (1,2), (1,0), (3,0)]
    K = [(0,0), (0,2), (0,1), (1,2), (0,1), (1,1), (1,0), (3,0)]
    L = [(0,0), (0,2), (0,0), (1,0), (3,0)]
    M = [(0,0), (0,2), (1,1), (2,2), (2,0), (4,0)]
    N = [(0,0), (0,2),(1,1), (1,2), (1,0), (3,0)]
    O = [(0,0), (0,2), (1,2), (1,0), (0,0), (1,0), (3,0)]
    P = [(0,0), (0,2), (1,2), (1,1), (0,1), (0,0), (2,0)]
    Q = [(0,0), (0,2), (1,2), (1,0), (0,1), (1,0), (0,0), (3,0)] 
    R = [(0,0), (0,2), (1,2), (1,1), (0,1), (1,0), (3,0)]
    S = [(0,0), (1,0), (1,1), (0,1), (0,2), (1,2), (0,2), (0,1), (1,1), (1,0), (3,0)]
    T = [(0,0), (0,2), (-1,2), (1,2), (0,2), (0,0), (3,0)]
    U = [(0,0), (0,2), (0,0), (1,0), (1,2), (1,0), (3,0)]
    V = [(0,0), (-1,1), (-1, 2), (-1,1),(0,0), (1,1), (1,2), (1,1), (0,0), (3,0)]
    W = [(0,0), (0,2), (0,0), (1,1), (2,0), (2,2), (2,0), (4,0)]
    X = [(0,0), (2,2), (1,1), (0,2), (2,0), (4,0)]
    Y = [(0,0), (0,1), (-1,2), (0,1), (1,2), (0,1), (0,0), (3,0)]
    Z = [(0,0), (1,0), (0,0), (2,2), (1,2), (2,2), (0,0), (4,0)]

    space = [(0,0), (3,0)]
    
    letters2 = [A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, space]
    for element in letters:
        if element == letter:
            return letters2[letters.index(element)]

def print_return_one_line(line): #returns the points to write on one line
    move = []
    global initial
    for element in line:
        character = to_letters(element)
        for tup in character:
            move.append(tuple(item1 + item2 for item1, item2 in zip(initial, tup)))
        initial = tuple(item1 + item2 for item1, item2 in zip(initial, character[-1]))
    return move


def determine_one_line(text): # goes up to the last word that can still be written on one line
    global limit
    start = (0,0)
    last_space = None
    line = []
    for i in range(len(text)):
        character = to_letters(text[i])
        #print(i)
        #print(text[i])
        #print(character)
        if len(character) == 2:
            last_space = i
            #print(last_space)
            #print(i)
        line.append(text[i])
        start = tuple(item1 + item2 for item1, item2 in zip(start, character[-1]))
        if start[0] > limit[0]:
            if last_space != None:
                for j in range(len(line)-last_space-1):
                    line.pop()
                return line
            else:
                return line
    return line

def main(text):
    global initial
    #text = speech()
    text = list(text)
    print("TERMINATE")
    final = []
    while len(text) > 0:
        line = determine_one_line(text)
        #Remove line from text
        text = text[len(line):]
        move = print_return_one_line(line) #Final one line sequence
        # Move to next line
        ycoord = initial[1]
        move.append((0,ycoord))
        lst = list(initial)
        lst[1] = ycoord-4
        lst[0] = 0
        initial = tuple(lst)
        move.append(initial)
        final.extend(move)
    return final

def simple_motor_conversion(coord1, coord2): #Horizontal and Vertical (H,V)
    motors = []
    x1,y1 = coord1[0], coord1[1]
    x2, y2 = coord2[0], coord2[1]

    diff_y = y2-y1
    diff_x = x2-x1

    if diff_x == 0:
        if diff_y > 0:
            for i in range(diff_y):
                motors.append((0,1))
        else:
            for i in range(-diff_y):
                motors.append((0,2))
    elif diff_y == 0:
        if diff_x > 0:
            for i in range(diff_x):
                motors.append((1,0))
        else:
            for i in range(-diff_x):
                motors.append((2,0))
    elif diff_x > 0 and diff_y > 0:
        for i in range(diff_y):
            motors.append((1,1))
    elif diff_x > 0 and diff_y < 0:
        for i in range(-diff_y):
             motors.append((1,2))
    elif diff_x < 0 and diff_y > 0:
        for i in range(diff_y):
             motors.append((2,1))
    elif diff_x < 0 and diff_y < 0:
        for i in range(diff_y):
             motors.append((2,2))
    
    return motors

def total_motor_conversion(line):
    #print(line)
    final = []
    for i in range(len(line)-1):
        final.extend(simple_motor_conversion(line[i], line[i+1]))
    return final

def write_file(motors):
    with open('file.txt', 'w') as fp:
        fp.write('\n'.join('%s %s' % x for x in motors))

final = main("THE QUICK BROWN FOX JUMPED OVER THE LAZY BROWN DOG")

#final = main()
print(total_motor_conversion(final))
mylist = total_motor_conversion(final)

write_file(mylist)

for i in range(len(final)):
    temp = tuple(5*item for item in final[i])
    T.goto(temp[0], temp[1])
T.mainloop()