# Helpful Links for Virtual Pipe Organ Construction

Note: I haven't used all these products or followed all these techniques, but these are links I've saved along the way. It's by no means complete, but perhaps this list will be helpful in your journey.

## Software

* [Hauptwerk](https://www.hauptwerk.com/) (The one most people use - paid, but it gives you the most)
* [GrandOrgue](https://sourceforge.net/projects/ourorgan/) (A heavyweight free alternative)
* [jOrgan](https://sourceforge.net/projects/jorgan/) (A lightweight free alternative)
    * [jOrgan Discovery](https://jorgan.info/)
    * [South African Virtual Pipe Organ Project](https://sites.google.com/site/savirtualorgans/home)

## Microprocessor

* The [Teensy 4.1 Development Board](https://www.pjrc.com/store/teensy41.html)
    * As I describe in my [other documentation](../teensy/README.md), this is the microprocessor board I have used in my project.

### Circuit board building

* [Common Connectors explained](http://www.mattmillman.com/info/crimpconnectors/)
* [Using perfboard](https://www.instructables.com/Using-Perfboard/)
* [Crimping Dupont Connectors](https://www.instructables.com/Make-a-Good-Dupont-Pin-Crimp-EVERY-TIME/)
    * Note, you can avoid having to do this if you get pre-crimped wires, or if you get (for example) a bundle of pre-made female-to-female Dupont wires (which you can even take the plastic off of and then attach your own Dupont multi-connectors to).
* [Another article on how to crimp connectors](https://iotexpert.com/jst-connector-crimping-insanity/)
    * Again, I advise getting pre-crimped wires and saving yourself the time.

### MIDI

Note that the libraries for Teensy are often based upon the libraries for Arduino (although I think some of them have optimizations, so I always used the Teensy versions).

* General MIDI info
    * [Great MIDI Tutorial by Sparkfun](https://learn.sparkfun.com/tutorials/midi-tutorial/all)

* Arduino MIDI
    * [A MIDI Shield](http://sandsoftwaresound.net/connect-midi-shield-littlebits-arduino/)
    * [MIDI with Arduino](https://www.instructables.com/Send-and-Receive-MIDI-with-Arduino/)
    * [Midi example](https://www.arduino.cc/en/Tutorial/BuiltInExamples/Midi)
    * [USB MIDI](https://github.com/lathoub/Arduino-USBMIDI)
    * [Custom Arduino MIDI Controller](https://www.instructables.com/Custom-Arduino-MIDI-Controller/)
* MIDI for Teensy (what I used)
    * [Using USB Midi](https://www.pjrc.com/teensy/td_midi.html)
    * [Using Serial MIDI](https://www.pjrc.com/teensy/td_libs_MIDI.html)
    * [Teensy 4.1 MIDI Breakout board](https://www.tindie.com/products/deftaudio/teensy-41-midi-breakout-board-8in-8out-usb-host/)

### Buttons

I used momentary push buttons for pistons, and reed switches (in conjunction with magnets) to retrofit the pedalboard.

* [Reed Switches and Hall Effect Sensors](https://www.kjmagnetics.com/blog.asp?p=reed-switches-and-hall-effect-sensors)
* [Documentation on Reed Switch Activation](https://www.kjmagnetics.com/pdfs/AN104.pdf)
* [How Reed Switches are used with a Permanent Magnet](https://www.sos.sk/a_info/resource/h/meder/Reedswitch_used_with_permanent_magnet.pdf)
* [A Guide to Debouncing](https://my.eng.utah.edu/~cs5780/debouncing.pdf)
    * Although most people can get by with just using the `Bounce` or `Bounce2` library for Arduino, this is a super-thorough explanation of how to do debouncing yourself.

### Button Labels

* [Putting Clearcoat over decals](https://www.rocketryforum.com/threads/clearcoat-over-decals.8296/)
    * I might try this: printing a see-through label for each piston button, then clearcoating over it


### Expression Pedals

* [Yamaha FC7](https://usa.yamaha.com/products/music_production/accessories/fc7/index.html)
    * I ended up using 4 of these pedals
    * [Yamaha Pedal Wiring](https://hugh.blemings.id.au/2015/10/20/yamaha-pedal-wiring/)
    * [Adding Resistance to Movement](https://engineering.stackexchange.com/questions/6951/how-do-i-add-a-resistance-to-mechanical-movement-in-both-directions-apart-from)
        * I think I might add a friction-based mechanism to my Yamaha pedals at some point, to stiffen their movement
* [Peterson Expression and Crescendo Pedals](http://www.petersonemp.com/products/pdf/EXPRESSION.PDF)
    * Lots of good measurements which were helpful
* For Arduino
    * [Expression pedal for Arduino](https://www.kuassa.com/diy-tutorial-how-to-build-arduino-based-expression-pedal-for-efektor-wf3607/)
    * [Making a guitar pedal from an Arduino](https://create.arduino.cc/projecthub/jonasvugt/turn-an-arduino-uno-into-a-midi-controller-guitar-pedals-ac36a2)


Debouncing

### Using Shift Registers
 
I didn't end up using shift registers at all, but it's one way you can increase your inputs. You can use them as part of a button matrix as well.

* [Sparkfun Shift Registers Tutorial](https://learn.sparkfun.com/tutorials/shift-registers)
* [This is for an *output* shift register](https://www.arduino.cc/en/Tutorial/Foundations/ShiftOut)
* [Shift Registers – 74HC595 & 74HC165 with Arduino](https://dronebotworkshop.com/shift-registers/)
* [Using SPI with shift registers](http://www.gammon.com.au/forum/?id=11979)

### IO Expanders

I didn't use these but here they are for reference.

* [The MCP23017 I/O Expander](https://www.best-microcontroller-projects.com/mcp23017.html)

### Matrix Scanning

Input matrix scanning is one way to hook a lot of buttons up to a microprocessor, even when you have a lot more buttons than input pins on the microprocessor.

Piano keyboards (as well as a lot of other things like keypads that have lots of "buttons") have used matrixes for along time.

I didn't end up needing to scan my keyboards directly, since I just used the electronics that came along with my Keystation keyboards and just used the MIDI out from those. But if you want to do away with (or don't have) those electronics for your keyboard, this could be helpful in hooking the keyboard matrix directly to an Arduino.

I did, however, create a matrix out of the piston buttons. So I still needed to utilize the principles here.

* [Input Matrix Scanning](http://www.openmusiclabs.com/learning/digital/input-matrix-scanning/index.html)
    * A hugely useful resource for how to scan a lot of buttons with a limited number of IO pins
* [HOW TO MAKE A KEYBOARD - THE MATRIX](http://blog.komar.be/how-to-make-a-keyboard-the-matrix/)
    * This guy explained matrixes very well, understandable to me as a beginner. Great companion resource to the link above from OpenMusicLabs.
* [Figuring Out a Key Matrix (Scan Matrix)](https://www.instructables.com/Figuring-out-a-Key-Matrix-Scan-Matrix/)
* [Moura's Keyboard Scanner](https://github.com/oxesoft/keyboardscanner)
* Arduino Blog Posts
    * [Keyboard (Piano) Scanner Casio CTK 485 - Arduino Mega 2560](https://forum.arduino.cc/t/keyboard-piano-scanner-casio-ctk-485-arduino-mega-2560/646089)
    * [Blog post on scanning a keyboard matrix with Arduino](https://forum.arduino.cc/t/replace-chip-pcb-in-usb-midi-keyboard-with-arduino/487321/2)
* PJRC Blog Posts
    * [Music Keyboard matrix scanning](https://forum.pjrc.com/threads/53588-Music-Keyboard-matrix-scanning)
    * [Help needed: debouncing a button matrix](https://forum.pjrc.com/threads/46188-Help-neede-Debouncing-a-button-matrix)
    * [Midi - 8X8 diode matrix vs 16:1 MUX](https://forum.pjrc.com/threads/40492-Midi-8X8-diode-matrix-vs-16-1-MUX?p=125947&viewfull=1#post125947)
    * [key matrix 56 keys, no diodes on teensy 4.0](https://forum.pjrc.com/threads/61065-key-matrix-56-keys-no-diodes-on-teensy-4-0?highlight=matrix+keypad+delay)
* [How to scan an old Casio keyboard matrix](https://medium.com/@mero11.gh/midi-fying-an-old-casio-digital-keyboard-6a9ae9f3092b)
* [Recycle Piano Keyboard With Arduino](https://www.instructables.com/Recycle-Piano-Keyboard-With-Arduino/)
* [Adding MIDI to Old Home Organs](https://www.instructables.com/Adding-MIDI-to-Old-Home-Organs/)
* [Switching pins from input to output](https://electronics.stackexchange.com/questions/101869/is-it-safe-for-micro-to-switch-pins-from-input-to-output-in-a-loop/101876#101876)
    * This is how the Keypad Arduino library does it, essentially.
* 

## General Pipe Organ Information

* [A Young Person’s Guide to the Pipe Organ](https://www.agohq.org/young-persons-guide/)
* Pistons
    * [The Anglican Way of setting up and using pistons!](https://www.contrebombarde.com/concerthall/home/view_cat/cat/1/sort/100/order/last_modified/limit/10)
        * This post helped me decide that I needed 8 divisional pistons
    * [Piston Layout](http://www.organworks.co.uk/news/piston-layout/)
    * [Sequencing registrations](https://www.organworks.com/index.php/articles/261-the-registration-quick-step)

* BYU/LDS (I'm not a Mormon, but I have to admit they have a lot of good pipe organ information)
    * [ORGAN TYPES AND COMPONENTS](https://organ.byu.edu/orpipe.html)
    * [The Latter-day Saint Organist's Resource Blog](http://organlessons.blogspot.com/2010/01/lesson-1-acquiring-essentials.html)
    * OrganTutor
        * [Main Page](https://organtutor.byu.edu/)
        * [Units and Lessons](https://organ.byu.edu/organtutor/index.htm)
    * 

## Wood Finishing

I built my console with soft wood (the "white wood" at Lowe's, which seems like pine). I chose to finish it with multiple layers of garnet shellac, which I bought a bag of flakes of and then dissolved it in denatured alcohol.

* [How to Prevent Blotching Using a Washcoat](http://www.refinishwizard.com/washcoatsolids.htm)
* [SUCCESS WITH SHELLAC](https://www.woodcraft.com/blog_entries/success-with-shellac)
* [SHELLAC – A TRADITIONAL FINISH STILL YIELDS SUPERB RESULTS](http://www.antiquerestorers.com/Articles/jeff/shellac.htm)

## Choosing a keyboard action

* [Three-keyboard stack by John's Organ Works](http://www.johnsorganworks.co.uk/ms3.html)
    * The fact that he creates these out of M-Audio Keystation 61 MK3 keyboards was a confirmation to me that that model of keyboard would be a good choice for my build.
* [Paul Kuzan's Blog](http://sylva-autokits.blogspot.com/search?q=yamaha)
    * This also helped steer me toward the M-Audio Keystation 61 MK3, although he also talks about the Yamaha YPT 210.
* [Walworth Virtual Pipe Organ Manuals](https://sites.google.com/site/walworthvirtualpipeorgan/manuals)
    * He uses the Yamaha YPT 210

## Others who have built similar projects

The best compilation site for this is: [PCorgan.com](https://www.pcorgan.com/Fotos3bEN.html)

Here are some other links for specific virtual pipe organ builds that I've found along the way.

* [The Walworth Virtual Pipe Organ](https://sites.google.com/site/walworthvirtualpipeorgan/)
    * This is one of my favorites
* [Virtual Pipe Organ on Hackaday](https://hackaday.io/project/19077-virtual-pipe-organ)
    * Another favorite. I used his circuit designs as my early inspiration although I didn't end up using shift registers like he did.
    * [His github](https://github.com/Nkawu/Virtual_Pipe_Organ)
* [Teensy Hauptwerk Projects](https://teensyhauptwerk.wordpress.com/)
* [virtualpipeorgans.wordpress.com](https://virtualpipeorgans.wordpress.com/building-my-organ-console/)
* [Yet Another Organ Pedalboard Project](http://www.musanim.com/mam/organproject.html)
* Helpful Teensy forum posts:
    * [Adding Midi to a 32 note Pedalboard via Teensy?](https://forum.pjrc.com/threads/23550-Adding-Midi-to-a-32-note-Pedalboard-via-Teensy)
* [Home Electronic Organ Project](https://www.angelfire.com/mn/organlcms/organpro.html)
* [Eivind Fivelsdal](http://www.fivelsdal.no/Hauptwerk%20Console/Building%20an%20organ%20console%20-%20DIY%20project.htm)
* [prodan.org](http://prodan.org/virtual-private-organ.html)
* [Organ Bench](https://fagerjord.org/pipe-organ/543/)
* [Keyboard Stack](https://fagerjord.org/pipe-organ/540/)
* [Advanced Hauptwerk console](https://midi-orgue.com/tech/concept/)
* [Hauptwerk Virtual Pipe Organ](http://www.omayes.com/VirtualOrgan/OrganPage0.html)