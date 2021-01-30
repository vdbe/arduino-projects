import sys
"""
FORMAT: A: 010 00010 -> 0x42
first 3 bits: Sum of all dots and dashed
last 5 bits: 0 for a dot 1 for a dash
The last 5 bits are read in te reverse order after the len provided by the first 3 bits
everything is ingnored.
"""

morseList = {
    "A": "01",  # Basic
    "B": "1000",    # 1
    "C": "1010",    # 2
    "D": "100", # 3
    "E": "0",   # 4
    "F": "0010",    # 5
    "G": "110",
    "H": "0000",
    "I": "00",
    "J": "0111",
    "K": "101",
    "L": "0100",
    "M": "11",
    "N": "10",
    "O": "111",
    "P": "0110",
    "Q": "1101",
    "R": "010",
    "S": "000",
    "T": "1",
    "U": "001",
    "V": "0001",
    "W": "01",
    "X": "1001",
    "Y": "1011",
    "Z": "1100", # 25
    "0": "11111",   # 26
    "1": "01111",
    "2": "00111",
    "3": "00011",
    "4": "00001",
    "5": "00000",
    "6": "10000",
    "7": "11000",
    "8": "11100",
    "9": "11110", }  # 35
#    ",": "110011",  # Symbols (doesn't fit in a single byte)
#    ".": "010101",
#    "?": "001100",
#    ";": "10101",
#    ":": "111000",
#    "/": "10010",
#    "-": "100001",
#    "'": "011110",
#    "()": "101101",
#    "_": "001101",
#    "Á": "01101",   # Extendex ASCII
#    "Ä": "0101",
#    "É": "00100",
#    "Ñ": "11011",
#    "Ö": "1110",
#    "Ü": "0011",
#    }

def main(depth, type):
    if type is not 1:
        #print("//const char morse[] = {")
        print("//                   { ", end="")
        for key in morseList:
            print(f"\'{key}\'".rjust(4), end=", ")
        print("};\nconst char morse[] = { ", end="")

    for key in morseList:
        # Generate the first 3 bits (length of the signal)
        length = len(morseList[key])
        blength = bin(length)[2:].zfill(3)

        # Get the last 5 bits 0 for dot 1 for dash (order is reversed)
        morse = morseList[key][::-1].zfill(5)

        # add first and last bits and convert them to hex
        hexadecimal = hex(int(f"{blength}{morse}", 2))

        if type is 1:
            print(f"\'{key}\': {blength} {morse} -> {hexadecimal}")
        else:
            print(hexadecimal, end=", ")

    print("};", end="")


if __name__ == '__main__':
    depth = 0
    type = 0

    # Not implemented
    if len(sys.argv) > 1:
        if sys.argv[1] is "symbols":
            depth = 1
        elif sys.argv[1] is "extended":
            depth = 2

    if len(sys.argv) > 2:
        if sys.argv[2] == "list":
            type = 1

    main(depth, type)
