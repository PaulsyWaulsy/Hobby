#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

const int GRID_SIZE = 3;
const int PADDING_X = 7;
const int PADDINT_Y = 3;

const char* const CHARACTERS = " XO";
const char* const AI_ARGS = "--ai";

struct termios originalTermios;

typedef enum {
    PLAYER_ONE = 1,
    PLAYER_TWO = 2,
} Player;

typedef enum {
    OK,
} ExitStatus;

typedef struct {
    bool ai;
} Parameters;

Parameters parse_args(int argc, char* argv[]);
void initialise_board(int gridSize, int board[gridSize][gridSize]);
void reset_board(void);
void enable_raw_mode(void);
void print_introduction(void);
void ai_make_move(int moves, int gridSize, int board[gridSize][gridSize]);
bool check_win(int gridSize, int board[gridSize][gridSize]);
bool check_draw(int gridSize, int moves);
bool is_valid_input(char input, int gridSize, int board[gridSize][gridSize]);
char* create_horizontal(int gridSize);
void print_board(int paddingX, int paddingY, int gridSize, int board[gridSize][gridSize]);
void make_move(int player, int index, int gridSize, int board[gridSize][gridSize]);

int main(int argc, char* argv[]) {
    system("clear");
    printf("\033[?25l"); // hide cursor
    enable_raw_mode();
    print_introduction();

    int board[GRID_SIZE][GRID_SIZE];
    initialise_board(GRID_SIZE, board);
    print_board(PADDING_X, PADDINT_Y, GRID_SIZE, board);

    Parameters params = parse_args(argc, argv);
    char input;
    int turn = PLAYER_ONE;
    int moves = 0;
    while (read(STDIN_FILENO, &input, 1) == 1 && input != 'q') {
        if (!is_valid_input(input, GRID_SIZE, board)) {
            // retry the input
            continue;
        }

        moves++;
        int intInput = (int)input - 48;
        make_move(turn, intInput, GRID_SIZE, board);
        print_board(PADDING_X, PADDINT_Y, GRID_SIZE, board);
        fflush(stdout);

        if (check_win(GRID_SIZE, board)) {
            printf("\033[6GPlayer %d Wins!\n", turn);
            printf("\033[?25h"); // hide cursor
            exit(OK);
        }

        if (check_draw(GRID_SIZE, moves)) {
            printf("\033[10GDraw!\n");
            printf("\033[?25h"); // hide cursor
            exit(OK);
        }

        // flip turns at the end of the logic
        turn = (turn == PLAYER_ONE) ? PLAYER_TWO : PLAYER_ONE;
    }

    printf("\033[?25h"); // hide cursor
    return OK;
}

Parameters parse_args(int argc, char* argv[]) {
    Parameters params = {
        .ai = false,
    };
    argc--;
    argv++;
    while (argc > 0 && !strncmp(argv[0], "--", 2)) {
        if (!strcmp(argv[0], AI_ARGS) && !params.ai) {
            params.ai = true;
        }
        argc--;
        argv++;
    }
    return params;
}

void print_introduction() {
    printf("\033[32mWelcome to Tic-Tac-Toe!\033[0m\n\n");
}

void initialise_board(int gridSize, int board[gridSize][gridSize]) {
    for (int row = 0; row < gridSize; row++) {
        for (int col = 0; col < gridSize; col++) {
            board[row][col] = 0;
        }
    }
}

char* create_horizontal_line(int gridSize) {
    int length = gridSize * 3 + (gridSize - 1);
    char* horizontal = malloc(sizeof(char) * length);
    memset(horizontal, '-', length);
    horizontal[length] = '\0';

    for (int i = 3; i < length; i += 4) {
        horizontal[i] = '+';
    }
    return horizontal;
}

void ai_make_move(int moves, int gridSize, int board[gridSize][gridSize]) {
    // [0, 0, 0,
    //  0, 0, 0,
    //  0, 0, 0]
}

void print_board(int paddingX, int paddingY, int gridSize, int board[gridSize][gridSize]) {
    char* horizontal = create_horizontal_line(gridSize);
    for (int row = 0; row < gridSize; row++) {
        printf("\033[%d;%dH", paddingY + row * 2, paddingX);
        for (int col = 0; col < gridSize; col++) {
            printf(" %c ", CHARACTERS[board[row][col]]);
            if (col < gridSize - 1) {
                printf("|");
            }
        }
        if (row < gridSize - 1) {
            printf("\033[%d;%dH%s", paddingY + row * 2 + 1, paddingX, horizontal);
        }
    }
    printf("\n\n");
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

bool is_valid_input(char input, int gridSize, int board[gridSize][gridSize]) {
    int intInput = (int)input - 48;
    if (intInput < 0 || intInput > 8) {
        return false;
    }

    int row = intInput / 3;
    int col = intInput % 3;
    if (board[row][col] != 0) {
        return false;
    }

    return true;
}

bool matchHorizontal(int gridSize, int board[gridSize][gridSize]) {
    bool isMatched = false;
    for (int col = 0; col < gridSize; col++) {
        int current;
        int next;
        for (int row = 0; row < gridSize - 1; row++) {
            current = board[row][col];
            next = board[row + 1][col];

            if (current == 0 || next != current) {
                // break if the two spots are not eqaul
                isMatched = false;
                break;
            }
            isMatched = true;
        }
        if (isMatched) {
            return true;
        }
    }
    return false;
}

bool matchVertical(int gridSize, int board[gridSize][gridSize]) {
    bool isMatched = false;
    for (int row = 0; row < gridSize; row++) {
        int current;
        int next;
        for (int col = 0; col < gridSize - 1; col++) {
            current = board[row][col];
            next = board[row][col + 1];

            if (current == 0 || next != current) {
                // break if the two spots are not eqaul
                isMatched = false;
                break;
            }
            isMatched = true;
        }
        if (isMatched) {
            return true;
        }
    }
    return false;
}

bool matchDiagonal(int gridSize, int board[gridSize][gridSize]) {
    int current;
    int next;
    bool isMatched = false;
    for (int row = 0, col = 0; row < gridSize - 1 && col < gridSize - 1; row++, col++) {
        current = board[row][col];
        next = board[row + 1][col + 1];
        if (current == 0 || next != current) {
            // break if the two spots are not eqaul
            isMatched = false;
            break;
        }
        isMatched = true;
    }
    if (isMatched) {
        return true;
    }

    for (int row = 0, col = gridSize - 1; row < gridSize - 1 && col > 0; row++, col--) {
        current = board[row][col];
        next = board[row + 1][col - 1];
        if (current == 0 || next != current) {
            // break if the two spots are not eqaul
            isMatched = false;
            break;
        }
        isMatched = true;
    }
    if (isMatched) {
        return true;
    }
    return false;
}

bool check_win(int gridSize, int board[gridSize][gridSize]) {
    bool isMatched = matchHorizontal(gridSize, board) | matchVertical(gridSize, board) |
                     matchDiagonal(gridSize, board);

    return isMatched;
}

bool check_draw(int gridSize, int moves) {
    int square = gridSize * gridSize;
    if (moves >= square) {
        return true;
    }
    return false;
}

void make_move(int player, int index, int gridSize, int board[gridSize][gridSize]) {
    int row = index / 3;
    int col = index % 3;

    board[row][col] = player;
}
