#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

const int NEIGHBOUR_SIZE = 9;
const char* const CHARACTER = " #";

struct termios originalTermios;

typedef struct {
    uint32_t capacity;
    uint8_t width;
    uint8_t height;
    uint8_t* arrayBits;
} BitArray;

BitArray* bitarray_create(uint8_t width, uint8_t height) {
    if (width <= 0 || height <= 0) {
        fprintf(stderr, "ERROR: dimensions cannot be less than or equal to zero\n");
        exit(1);
    }

    BitArray* bitArray = malloc(sizeof(BitArray));
    if (bitArray == NULL) {
        fprintf(stderr, "ERROR: unable to allocate memory for bit array\n");
        exit(1);
    }

    bitArray->capacity = width * height;
    bitArray->arrayBits = malloc(sizeof(uint8_t) * bitArray->capacity);
    if (bitArray->arrayBits == NULL) {
        fprintf(stderr, "ERROR: unable to allocate memory for array of bits\n");
        exit(1);
    }

    for (uint32_t i = 0; i < bitArray->capacity; i++) {
        bitArray->arrayBits[i] = 0;
    }

    bitArray->width = width;
    bitArray->height = height;
    return bitArray;
}

void bitarray_draw_boarder(BitArray* bitArray, uint8_t paddingY, uint8_t paddingX) {
    char* topBottom = malloc(sizeof(char) * bitArray->width + 3);
    memset(topBottom, '-', bitArray->width + 2);
    topBottom[bitArray->width + 2] = '+';
    topBottom[0] = '+';
    topBottom[bitArray->width + 3] = '\0';

    char* middle = malloc(sizeof(char) * bitArray->width + 3);
    memset(middle, ' ', bitArray->width + 2);
    middle[bitArray->width + 2] = '|';
    middle[0] = '|';
    middle[bitArray->width + 3] = '\0';

    for (uint8_t i = 0; i < bitArray->height + 2; i++) {
        if (i == 0 || i == bitArray->height + 1) {
            printf("\033[%d;%dH%s", i + paddingX - 1, paddingY - 1, topBottom);
        } else {
            printf("\033[%d;%dH%s", i + paddingX - 1, paddingY - 1, middle);
        }
    }
}

void bitarray_draw_conway(BitArray* bitArray, uint8_t paddingY, uint8_t paddingX) {
    uint32_t x = 0;
    uint32_t y = 0;

    for (uint32_t i = 0; i < bitArray->capacity; i++) {
        printf("\033[%d;%dH%c", y + paddingX, x + paddingY, CHARACTER[bitArray->arrayBits[i]]);
        x = (x + 1) % bitArray->width;
        if (x == 0) {
            y++;
        }
    }
}

uint32_t bitarray_indexat(BitArray* bitArray, uint8_t x, uint8_t y) {
    return y * bitArray->width + x;
}

void bitarray_setat(BitArray* bitArray, uint8_t x, uint8_t y, uint8_t sign) {
    uint32_t index = y * bitArray->width + x;
    if (index >= bitArray->capacity) {
        fprintf(stderr, "ERROR: index out of range of bit array\n");
        exit(1);
    }

    bitArray->arrayBits[index] = sign;
}

uint8_t bitarray_convolution(BitArray* bitArray, uint32_t index) {
    uint8_t sum = 0;
    uint32_t width = bitArray->width; // Assuming width is defined in BitArray
    uint32_t height = bitArray->height; // Assuming width is defined in BitArray

    // Calculate x and y coordinates
    uint32_t x = index % width; 
    uint32_t y = index / width;

    // Loop through neighbors
    for (int8_t i = -1; i <= 1; i++) {
        for (int8_t j = -1; j <= 1; j++) {
            // Skip the center cell
            if (i == 0 && j == 0) {
                continue;
            }

            // Calculate neighbor coordinates with wrapping
            uint32_t neighborX = (x + i + width) % width; // Wrap around horizontally
            uint32_t neighborY = (y + j + height) % height; // Wrap around vertically

            // Calculate the neighbor index
            uint32_t neighborIndex = neighborY * width + neighborX;

            // Add to sum if the neighbor is within bounds
            sum += bitArray->arrayBits[neighborIndex];
        }
    }
    return sum;
}

void bitarray_update(BitArray* bitArray) {
    uint8_t* tempArray = (uint8_t*)malloc(bitArray->capacity * sizeof(uint8_t));
    if (tempArray == NULL) {
        fprintf(stderr, "ERROR: unable to alloctate memory for temporary array in update");
        exit(1);
    }

    for (uint32_t i = 0; i < bitArray->capacity; i++) {
        uint8_t convolution = bitarray_convolution(bitArray, i);
        
        if (bitArray->arrayBits[i]) { 
            if (convolution < 2 || convolution > 3) {
                tempArray[i] = 0;
            } else {
                tempArray[i] = 1;
            }
        } else if (convolution == 3) {
            tempArray[i] = 1;
        } else {
            tempArray[i] = 0;
        }
    }

    for (uint32_t i = 0; i < bitArray->capacity; i++) {
        bitArray->arrayBits[i] = tempArray[i];
    }

    free(tempArray);
}

void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &originalTermios);
}

void enable_raw_mode() {
    tcgetattr(STDIN_FILENO, &originalTermios);
    atexit(disableRawMode);

    struct termios raw = originalTermios;
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

uint8_t cell_convlolution() {
    return 0;
}

int main() {
    system("clear");
    printf("\033[?25l"); // hide cursor
    enable_raw_mode();

    BitArray* bitArray = bitarray_create(40, 20);
    bitarray_draw_boarder(bitArray, 10, 5);
    bitarray_setat(bitArray, 10, 10, 1);
    bitarray_setat(bitArray, 11, 10, 1);
    bitarray_setat(bitArray, 12, 10, 1);
    bitarray_setat(bitArray, 12, 9, 1);
    bitarray_setat(bitArray, 11, 8, 1);

    bitarray_draw_conway(bitArray, 10, 5);
    fflush(stdout);

    char input;
    int i = 0;
    while (read(STDIN_FILENO, &input, 1) == 1 && input != 'q') {

        bitarray_update(bitArray);
        bitarray_draw_conway(bitArray, 10, 5);
        fflush(stdout);
        i++;
        printf("\033[3;9H%d", i);
    }

    printf("\033[?25h"); // hide cursor
    return 0;
}
