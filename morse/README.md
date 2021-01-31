# Morse
Prints a sentence using morscode, when sentence is fully printed it starts over.

## charbyte
### Format
* 8 bits
	* First 3: Sum of all dots and dashes
	* Last 5: 1 for a dash, 0 for a dot
		* Read in reverse
* Max of 5 dots/dashes when using a single byte

### Generate
```bash
python3 helpers/gen-morsebytes.py
```

### Table
| char | len | signals | hex |
| -- | -- | -- | -- |
| A | 010 | 00010 | 0x42 |
| B | 100 | 00001 | 0x81 |
| C | 100 | 00101 | 0x85 |
| D | 011 | 00001 | 0x61 |
| E | 001 | 00000 | 0x20 |
| F | 100 | 00100 | 0x84 |
| G | 011 | 00011 | 0x63 |
| H | 100 | 00000 | 0x80 |
| I | 010 | 00000 | 0x40 |
| J | 100 | 01110 | 0x8e |
| K | 011 | 00101 | 0x65 |
| L | 100 | 00010 | 0x82 |
| M | 010 | 00011 | 0x43 |
| N | 010 | 00001 | 0x41 |
| O | 011 | 00111 | 0x67 |
| P | 100 | 00110 | 0x86 |
| Q | 100 | 01011 | 0x8b |
| R | 011 | 00010 | 0x62 |
| S | 011 | 00000 | 0x60 |
| T | 001 | 00001 | 0x21 |
| U | 011 | 00100 | 0x64 |
| V | 100 | 01000 | 0x88 |
| W | 010 | 00010 | 0x42 |
| X | 100 | 01001 | 0x89 |
| Y | 100 | 01101 | 0x8d |
| Z | 100 | 00011 | 0x83 |
| 0 | 101 | 11111 | 0xbf |
| 1 | 101 | 11110 | 0xbe |
| 2 | 101 | 11100 | 0xbc |
| 3 | 101 | 11000 | 0xb8 |
| 4 | 101 | 10000 | 0xb0 |
| 5 | 101 | 00000 | 0xa0 |
| 6 | 101 | 00001 | 0xa1 |
| 7 | 101 | 00011 | 0xa3 |
| 8 | 101 | 00111 | 0xa7 |
| 9 | 101 | 01111 | 0xaf |
