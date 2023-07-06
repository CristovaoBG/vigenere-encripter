# Vigenere Encripter
A simple terminal based log or diary written in C that encrypts, stores, and decrypts text files using Vigenère's cipher. This is an old project that is maintained here mainly for personal use, I have been using it for a long time to write personal and not so critical stuff. However, this is a brief documentation in case anyone is curious.

# What is it
The Vigenère cipher is a simple cryptographic technique used for encrypting and decrypting messages. It utilizes a keyword to determine the shifting pattern applied to the plaintext characters. Each letter of the keyword corresponds to a shift value. By repeatedly applying these shifts to the corresponding plaintext letters, the ciphertext is generated.

## How it works

The Vigenère cipher is a method of encrypting messages using a keyword-based polyalphabetic substitution. It provides stronger security than simple ciphers like the Caesar cipher.

To encrypt a phrase using the Vigenère cipher, follow these steps:

1. Choose a keyword. For example, let's use the keyword "KEY".
2. Repeat the keyword to match the length of the phrase. For instance, if the phrase is "HELLO WORLD", repeat the keyword as "KEYKEYKEYKE".
3. Assign any numerical values to each letter of both the phrase and the repeated keyword, such as A=0, B=1, and so on.
4. Take the first letter of the phrase and the corresponding letter from the repeated keyword.
5. Add the numerical values of these letters together, considering modular arithmetic (wrap around the alphabet).
6. Convert the resulting numerical value back to a letter.
7. Repeat steps 4-6 for each letter in the phrase, using the corresponding letter from the repeated keyword.
8. The resulting sequence of letters is the encrypted ciphertext.

For example, let's encrypt the phrase "HELLO WORLD" using the key "KEY":

- Phrase: HELLO WORLD
- Key: KEY
- Repeated Key: KEYKEYKEYKE
- Encrypted Ciphertext: RBYPA WGASA

During encryption, only alphabetical characters are encrypted, while non-alphabetical characters remain unchanged in the ciphertext.

# Usage
Enter a long passwoord with only letters to create a file (if you haven't already used before) and write your text next. To finish writting press Enter twice, and you will be prompted if you want to sabe the file. The file will store the text, using the Vigenère's cipher method and the date on a file. when you open the program next time, enter your password and everything you wrote before will be decripted and shown to you above the time and the date. You can navigate through each entry you made in the past.

### Navigation
    <A> - View next page
    <B> - View previous page
    <E> - View last page
    <W> - Write new entrya

Cristóvão Bartholo Gomes
cristovao@live.com
https://github.com/CristovaoBG
