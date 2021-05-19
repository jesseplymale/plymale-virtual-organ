notes = [ 'C', 'C#', 'D', 'D#', 'E', 'F', 'F#', 'G', 'G#', 'A', 'A#', 'B' ]

def midi_number_to_name( midi_number):
    note_name_index = midi_number % len(notes)
    note_octave = (midi_number // len(notes)) - 1
    return f"{notes[note_name_index]}{note_octave}"

print('  // First row of pins, down the upper left side of the board')
for i in range(0, 8):
    pin_number = 2+i 
    midi_number = 36+i
    note_name = midi_number_to_name(midi_number)
    print(f"  {{  {pin_number}, {midi_number} }}, // {note_name}") 

print('  // Second row of pins, up the upper right side of the board')
for i in range(0, 8):
    pin_number = 16+i 
    midi_number = 44+i
    note_name = midi_number_to_name(midi_number)
    print(f"  {{  {pin_number}, {midi_number} }}, // {note_name}") 

print('  // Third row of pins, down the lower left side of the board')
for i in range(0, 8):
    pin_number = 24+i 
    midi_number = 52+i
    note_name = midi_number_to_name(midi_number)
    print(f"  {{  {pin_number}, {midi_number} }}, // {note_name}") 

print('  // Fourth row of pins, up the lower right side of the board')
for i in range(0, 8):
    pin_number = 34+i 
    midi_number = 60+i
    note_name = midi_number_to_name(midi_number)
    print(f"  {{  {pin_number}, {midi_number} }}, // {note_name}") 
