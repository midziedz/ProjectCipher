# ProjectCipher
A minimal Linux-runnable GUI for managing and using a Vigenère cipher for short messages, both saved in files and written on-the-go. A "hardcore", console-runnable version is planned as a kind of CLI-designing challenge.
## Important notice
At the moment, the help functionality is disabled. If You feel like running the program, but feel uncertain of how to do it properly, make use of the tooltips for all the other menubar functionalities. The basic set of keys is actually set by default, so if You really cannot understand the cipher management and I/O loading and saving logic, just use the Cypher and Decypher options, pointed to by the "gears" and "restore" icons, respectively, as they are the key functionalities of the program, but be warned You will only be able to use CAPITAL letters of the English alphabet, spaces and newlines, without any digits or special characters, which resembles the basic Vigenere idea, except the spaces are considered a part of the text to cypher, just to make sure You cannot determine the length of words in the original message by just looking at its cyphered form, which aims at making the cypher harder to break.
## Known bugs and issues
- Sometimes, especially for input length exceeding the volume of the input box, the program will crash on cyphering/decyphering it. It only happens for too large or really unfortunate, rare input, so the program is still usable, but if it crashes for Your input, try dividing it into smaller parts.
- At the moment, the program does not accept Polish and other language-specific diacritic characters, "narrowing" them down to Latin/basic English characters (even with the proper character set enabled), as there are indexing issues when using multiple-byte characters (which is the case for diacritics) alongside "normal", single-byte characters in C++ string objects.
