# qr code generator

This is a simple C program that encodes an input string into a QR code and displays it in a window using SDL2 or rencode library.

## Dependencies

*   **libqrencode**: For generating QR codes.
*   **libSDL2**: For creating a graphical window and displaying the image.
*   **C Compiler** (e.g., `gcc`)
*   **lpng**: Link to libpng library.This is the standard library for processing PNG image format.
*   **lz**: Link to zlib library.zlib provides data compression and decompression functions, especially Deflate algorithm.
*   **lm**: Link to Mathematics Library (libm).

## Installing Dependencies

You need to install the development versions of `libqrencode` and `libSDL2`.

**On Debian/Ubuntu systems:**

```bash
sudo apt update
sudo apt install build-essential libqrencode-dev libsdl2-dev libpng-dev zlib1g-dev gcc
```
Please compile other systems by yourself. If you don’t know how to install and compile, ask AI. I won’t introduce them one by one.
On other systems:

Please use your system's package manager (e.g., dnf for Fedora, brew for macOS, vcpkg or msys2 for Windows) to install the development files for libqrencode and SDL2.

I have downloaded the necessary libraries and packaged them into static libraries. You can compile them directly, or you can install them directly and then compile.

# Compilation

The source code file is named sdl_qrcode.c or terminal_qrcode.c or png_qrcode.c:
```
gcc -O2 sdl_qrcode.c -o sdl_qrcode -L. -lqrencode -lSDL2

gcc -O2 terminal_qrcode.c -o terminal_qrcode -L. -lqrencode

gcc -O2 png_qrcode.c -o png_qrcode -L. -lqrencode -lpng -lz -lm

```
```
-O2 is an optimization flag (optional).
-o sdl_qrcode specifies the output executable file name.
-lqrencode links the libqrencode library.
-lSDL2 links the libSDL2 library.
-lpng: Link to libpng library.
Purpose: This is the standard library for processing PNG image format.
-lz: Link to zlib library.
Purpose: zlib provides data compression and decompression functions, especially Deflate algorithm.
-lm: Link to Mathematics Library (libm).
Purpose: Provides standard mathematical functions, such as sqrt, sin, cos, etc.
```
Usage

Run the compiled program from the command line, passing the text data you want to encode as an argument. 
If your data contains spaces or special characters, enclose it in quotes.
```
./sdl_qrcode "<Text_to_Encode>"
```


# Examples:

Example with a simple string
```
./sdl_qrcode "Hello, World!"
```
Example with a URL
```
./sdl_qrcode "https://www.example.com"
```
Example with data containing spaces
```
./sdl_qrcode KwDiBf89QgGbjEhKnhXJuH7LrciVrZi3qYjgd9M7rFU73sVHnoWn
```
or
```
./terminal_qrcode bc1qt3nh2e6gjsfkfacnkglt5uqghzvlrr6jahyj2k
```
```
./png_qrcode echo earn pink table vehicle awful true shop hazard latin useful admit
The text to be encoded(待编码的文本): "echo earn pink table vehicle awful true shop hazard latin useful admit"
The QR code has been successfully saved to(二維碼成功保存到) png_qrcode.png

```
The program will open a window and display the QR code of the provided data.

Close the window to exit the program.

Or
terminal_qrcode will print the QR code directly in the window.

png_qrcode will convert the input text data into a QR code image and save it in the program directory.

# ⚠️ Caution ⚠️

Be careful when encoding sensitive information when using a device in public or on a network. QR codes can be easily scanned by anyone.

Your command line history may record the data you enter as parameters. If the data is sensitive, consider clearing the history or using another method.

# Sponsorship
If this project was helpful to you, please buy me a coffee. Your support is greatly appreciated. Thank you!
```
BTC: bc1qt3nh2e6gjsfkfacnkglt5uqghzvlrr6jahyj2k
ETH: 0xD6503e5994bF46052338a9286Bc43bC1c3811Fa1
DOGE: DTszb9cPALbG9ESNJMFJt4ECqWGRCgucky
TRX: TAHUmjyzg7B3Nndv264zWYUhQ9HUmX4Xu4
```

# 📜 Disclaimer
Reminder: Do not input real private keys on connected devices!

This tool is provided for learning and research purposes only. Please use it with an understanding of the relevant risks. The developers are not responsible for financial losses or legal liability -caused by the use of this tool.
