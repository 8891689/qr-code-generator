# qr code generator

This is a simple C program that encodes an input string into a QR code and displays it in a window using SDL2 or rencode library.

## Dependencies

*   **libqrencode**: For generating QR codes.
*   **libSDL2**: For creating a graphical window and displaying the image.
*   **C Compiler** (e.g., `gcc`)

## Installing Dependencies

You need to install the development versions of `libqrencode` and `libSDL2`.

**On Debian/Ubuntu systems:**

```bash
sudo apt update
sudo apt install libqrencode-dev libsdl2-dev gcc
```

On other systems:

Please use your system's package manager (e.g., dnf for Fedora, brew for macOS, vcpkg or msys2 for Windows) to install the development files for libqrencode and SDL2.

I have downloaded the necessary libraries and packaged them into static libraries. You can compile them directly, or you can install them directly and then compile.

# Compilation

The source code file is named sdl_qrcode.c or terminal_qrcode.c:
```
gcc -O2 sdl_qrcode.c -o sdl_qrcode -L. -lqrencode -lSDL2

gcc -O2 terminal_qrcode.c -o terminal_qrcode -L. -lqrencode

```

-O2 is an optimization flag (optional).

-o sdl_qrcode specifies the output executable file name.

-lqrencode links the libqrencode library.

-lSDL2 links the libSDL2 library.

Usage

Run the compiled program from the command line, passing the text data you want to encode as an argument. If your data contains spaces or special characters, enclose it in quotes.
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
./sdl_qrcode "This is some data with spaces"
```
or
```
./terminal_qrcode bc1qt3nh2e6gjsfkfacnkglt5uqghzvlrr6jahyj2k
```
The program will open a window showing the QR code of the provided data.

Close the window to exit the program.
or
terminal_qrcode program prints the QR code directly in the window.

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

