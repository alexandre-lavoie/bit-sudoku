/**
 * Byte Sudoku
 * Created by Alexandre Lavoie
 */

// Includes
#include <iostream>
#include <fstream>
#include <math.h>

// To prevent having to use std::
using namespace std;

// Partition of bits in base_system.
const char BASE_SYSTEM = 8;

// Buffer size of file read.
const char BUFFER_SIZE = 3;

/**
 *  Get the length of the char for file indexing. 
 */
char sizeOfChar(char c)
{
    if (c < 10)
    {
        return 2;
    }
    else if (c < 100)
    {
        return 3;
    }
    else if (c < 1000)
    {
        return 4;
    }
}

// For each byte, we check if there is only one power of two without any other output.
// * Truth case: if the bytes are 00000000 1000000.
// ! False case: if the bytes are 00000001 1000000.
bool powerOfTwo(unsigned char *ptr, int numberOfBytes)
{
    bool isPowerOfTwo = false;

    for (int b = 0; b < numberOfBytes; b++)
    {
        // Declare a variable to prevent having to always go back and read the address.
        unsigned char byte = *(ptr + b);

        // Formula to determine if the value is a power of two.
        if (byte && !(byte & (byte - 1)))
        {
            // If other bytes were not powers of two, then it could be a power of two.
            if (!isPowerOfTwo)
            {
                isPowerOfTwo = true;
                continue;
            }
        }

        // If there is no power of two for a set entry or a power of two again, this is not a power of two!
        if (byte > 0)
        {
            isPowerOfTwo = false;
            break;
        }
    }

    return isPowerOfTwo;
}

int main(int argc, char *argv[])
{
    // Usage message.
    if (argc < 2)
    {
        cout << "Usage: " << argv[0] << " FILENAME [OUTPUT]"
             << endl
             << endl;
        return 1;
    }

    // Create fstream.
    fstream fileInput;

    // Open sudoku file.
    fileInput.open(argv[1], ios::in);

    // Buffer for file input.
    char buffer[BUFFER_SIZE];

    // Gets the first like of file (this should be set to the sudoku size).
    fileInput.get(buffer, sizeof(buffer), ' ');

    // Board size n x n for big section.
    const int BOARD_SIZE = atoi(buffer);
    // The square of the board size (to make it n x n with columns and rows)
    const int BOARD_SIZE_2 = BOARD_SIZE * BOARD_SIZE;
    // Number of bytes needed for a row, column, and section.
    const int NUMBER_OF_BYTES = ceil(BOARD_SIZE_2 / 8.0);
    // Max bits for byte according to board.
    const int MAX_BIT = ((1 << BASE_SYSTEM) - 1);

    // Board to solve.
    unsigned char *board[BOARD_SIZE_2][BOARD_SIZE_2];

    // Define array for row, column, and section.
    unsigned char *row[BOARD_SIZE_2];
    unsigned char *column[BOARD_SIZE_2];
    unsigned char *section[BOARD_SIZE][BOARD_SIZE];

    // For rows, columns, and sections.
    for (int i = 0; i < BOARD_SIZE_2; i++)
    {
        // Memory allocate bit holder. Uses a char pointer (8 bit) to access the bits.
        row[i] = (unsigned char *)malloc(NUMBER_OF_BYTES);
        column[i] = (unsigned char *)malloc(NUMBER_OF_BYTES);
        section[i / BOARD_SIZE][i % BOARD_SIZE] = (unsigned char *)malloc(NUMBER_OF_BYTES);

        for (int b = 0; b < NUMBER_OF_BYTES; b++)
        {
            // Initalize bit holder to 0.
            *(row[i] + b) = 0;
            *(column[i] + b) = 0;
            *(section[i / BOARD_SIZE][i % BOARD_SIZE] + b) = 0;
        }
    }

    int fileIndex = sizeOfChar(BOARD_SIZE);

    for (int i = 0; i < BOARD_SIZE_2; i++)
    {
        for (int j = 0; j < BOARD_SIZE_2; j++)
        {
            // Change the file reading index.
            fileInput.seekg(fileIndex, ios::beg);

            // Get the chars until the next space.
            fileInput.get(buffer, BUFFER_SIZE, ' ');

            // If new line, increase the index to get the next number.
            if (buffer[0] == '\n')
            {
                fileIndex += 2;
                j--;
            }
            else
            {
                // Convert the read buffer of chars to integer.
                char sudokuCell = atoi(buffer);

                // Allocate memory to the board.
                board[i][j] = (unsigned char *)malloc(NUMBER_OF_BYTES);

                for (int b = 0; b < NUMBER_OF_BYTES; b++)
                {
                    // If the cell is empty.
                    if (sudokuCell == 0)
                    {
                        // Fill the byte with "all values are possible".
                        *(board[i][j] + b) = ((b + 1) * BASE_SYSTEM > BOARD_SIZE_2) ? MAX_BIT >> ((b + 1) * BASE_SYSTEM - BOARD_SIZE_2) : MAX_BIT;
                    }
                    else
                    {
                        // Get the base number of bits for the number.
                        unsigned char binaryIndex = 1 << (sudokuCell - b * BASE_SYSTEM - 1);

                        // If the binary index is set in this byte.
                        if (binaryIndex > 0)
                        {
                            // Insert binary index into row.
                            *(row[i] + b) |= binaryIndex;
                            // Insert binary index into column.
                            *(column[j] + b) |= binaryIndex;
                            // Insert binary index into section.
                            *(section[i / BOARD_SIZE][j / BOARD_SIZE] + b) |= binaryIndex;
                            // Insert index into board.
                            *(board[i][j] + b) = binaryIndex;
                        }
                        else
                        {
                            // If there is no bit index in this byte, set the byte to zero.
                            *(board[i][j] + b) = 0;
                        }
                    }
                }

                // Increase the file index to get the next char.
                fileIndex += sizeOfChar(sudokuCell);
            }
        }
    }

    // Closes the file input.
    fileInput.close();

    // Condition to check if board is complete.
    bool boardComplete = false;

    while (!boardComplete)
    {
        // Board is complete unless it isn't.
        boardComplete = true;

        // For each cell.
        for (int i = 0; i < BOARD_SIZE_2; i++)
        {
            for (int j = 0; j < BOARD_SIZE_2; j++)
            {
                // If the number is a power of two (kept here to prevent recalculating in byte loop)
                bool isPowerOfTwo = powerOfTwo(board[i][j], NUMBER_OF_BYTES);

                // For each byte.
                for (int b = 0; b < NUMBER_OF_BYTES; b++)
                {
                    // Get the value
                    unsigned char byte = *(board[i][j] + b);

                    if (isPowerOfTwo)
                    {
                        if (byte > 0)
                        {
                            *(row[i] + b) |= byte;
                            *(column[j] + b) |= byte;
                            *(section[i / BOARD_SIZE][j / BOARD_SIZE] + b) |= byte;
                        }
                    }
                    else
                    {
                        // Board must not be complete.
                        boardComplete = false;

                        // Check which numbers are possible.
                        // ? If the cell containes 1011 (4,2,1) as current possibilites and we get 1001 (4,1) from the or operation,
                        // ? the cell will now contain 0010 (2). This is "not" p->q, or p & !q.
                        *(board[i][j] + b) &= ~(*(row[i] + b) | *(column[j] + b) | *(section[i / BOARD_SIZE][j / BOARD_SIZE] + b));
                    }
                }
            }
        }
    }

    ofstream fileOutput;

    fileOutput.open((argv[2]) ? argv[2] : "solution.txt");

    for (int i = 0; i < BOARD_SIZE_2; i++)
    {
        for (int j = 0; j < BOARD_SIZE_2; j++)
        {
            unsigned int cell = 0;

            for (int b = 0; b < NUMBER_OF_BYTES; b++)
            {
                int byte = *(board[i][j] + b);

                if (byte > 0)
                    cell += log2(byte << (b * BASE_SYSTEM)) + 1;
            }

            fileOutput << cell << ' ';
        }
        fileOutput << endl;
    }

    fileOutput << endl
               << "Solved by Byte Sudoku";

    fileOutput.close();

    cout << endl
         << "Solved " << BOARD_SIZE << "x" << BOARD_SIZE << " board. Saved to " << ((argv[2]) ? argv[2] : "solution.txt") << '.'
         << endl
         << endl;

    return 0;
}