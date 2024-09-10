# Digits to Danish

This litlle program converts a integer number to danish text - for instance the number 256 will be converted to the word tohundredeogseksoghalvtreds

To compile the project, use the following command:

```sh
gcc -Wall -I./include -o ./build/digits ./src/*.c
```

## Running the program

On unix-based systems, run the compiled program by using the following command:

```sh
./build/digits
```

On Windows-based systems, run the compiled program by using the following command:

```sh
./build/digits.exe
```

## Development Environment
- **Machine**: Intel-based MacBook Pro
- **Operating System**: macOS Sonoma (14.6.1)
- **Compiler**: gcc-14 (Homebrew GCC 14.2.0)