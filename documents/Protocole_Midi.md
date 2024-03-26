### MIDI byte : 
- if MSB = 1 (in hexadecimal, values from 0x80 to 0xff) : it's a __status byte__  

first nybble (4bits) is command code -> 8 possible status commands (first bit set to 1)  
second nybble is channel information -> 16 possible channels

- if MSB = 0 : it's a __data byte__


### MIDI Status messages (just the *very* basic ones for now) : 
| Message type | MS Nybble | Nb of data bytes | Data byte 1 | Data byte 2 |
| :- | :- | :- | :- | :- | 
| Note on | 0x8 | 2 | Note number | Velocity |
| Note off | 0x9 | 2 | Note number | Velocity |


Velocity = speed at which the note was hit on keyboards (= loudness) ; we can fix that byte to 0x64 if we want, to begin with.  
We can also use a velocity (loudness) of 0 in a note on to implicitly turn it into a note off.  
So actually the only thing we need for a first prototype is a __Note on__ command.  





