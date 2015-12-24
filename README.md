# Steganography
[Steganography](https://en.wikipedia.org/wiki/Steganography) is the practice of concealing a file, message, image, or video within another file, message, image, or video. This program embeds information into a Portable Graymap Format (PGM) file which can later be decrypted. I chose the PGM format because it does not have any compression algorithms to deal with.  
To run the program (after compiling):  
1. stego <input PGM> \\\<output PGM\\\> <payload file>  
2. stego_extract <output PGM> <decrypted payload file>  
It's that easy!  
