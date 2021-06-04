
def midi_number_to_name( midi_number):
    note_name_index = midi_number % len(notes)
    note_octave = (midi_number // len(notes)) - 1
    return f"{notes[note_name_index]}{note_octave}"

num_rows = 9
num_cols = 12

for i in range (0, num_rows):
    row_array = [f"{x+1+(i*num_cols):>3}" for x in range(0, num_cols)] 
    joined_row = ", ".join(row_array)
    print(f"  {{ {joined_row} }},")



#print('  // Fourth row of pins, up the lower right side of the board')
#for i in range(0, 8):
#    pin_number = 34+i 
#    midi_number = 60+i
#    note_name = midi_number_to_name(midi_number)
#    print(f"  {{  {pin_number}, {midi_number} }}, // {note_name}") 
