#include <ncurses.h>
#include <math.h>
#include <unistd.h>

#define PI 3.14159265

// Function to draw a cube based on its projected coordinates
void draw_cube(WINDOW *win, double angle_x, double angle_y) {
    // Clear the window
    werase(win);

    // Cube vertices in 3D space
    double cube[8][3] = {
        {-1, -1, -1}, {1, -1, -1}, {1, 1, -1}, {-1, 1, -1},
        {-1, -1, 1}, {1, -1, 1}, {1, 1, 1}, {-1, 1, 1}
    };

    // Rotation matrices
    double cos_x = cos(angle_x);
    double sin_x = sin(angle_x);
    double cos_y = cos(angle_y);
    double sin_y = sin(angle_y);

    // Projected coordinates
    int projected[8][2];
    for (int i = 0; i < 8; i++) {
        // Rotate around the X-axis
        double y = cube[i][1] * cos_x - cube[i][2] * sin_x;
        double z = cube[i][1] * sin_x + cube[i][2] * cos_x;
        double x = cube[i][0];

        // Rotate around the Y-axis
        cube[i][0] = x * cos_y + z * sin_y;
        cube[i][1] = y;
        cube[i][2] = -x * sin_y + z * cos_y;

        // Project onto 2D plane (simple perspective projection)
        projected[i][0] = (int)(cube[i][0] * 10 + COLS / 2); // x-coordinate
        projected[i][1] = (int)(cube[i][1] * 10 + LINES / 2); // y-coordinate
    }

    // Draw edges of the cube
    int edges[12][2] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0},
        {4, 5}, {5, 6}, {6, 7}, {7, 4},
        {0, 4}, {1, 5}, {2, 6}, {3, 7}
    };

    for (int i = 0; i < 12; i++) {
        int x1 = projected[edges[i][0]][0];
        int y1 = projected[edges[i][0]][1];
        int x2 = projected[edges[i][1]][0];
        int y2 = projected[edges[i][1]][1];

        // Draw the line between the two points
        mvwaddch(win, y1, x1, '*');
        mvwaddch(win, y2, x2, '*');
    }

    // Refresh the window to show the new frame
    wrefresh(win);
}

int main() {
    initscr();              // Start ncurses mode
    noecho();               // Don't echo pressed keys
    curs_set(0);           // Hide the cursor
    nodelay(stdscr, TRUE); // Make getch() non-blocking

    double angle_x = 0;
    double angle_y = 0;

    while (1) {
        draw_cube(stdscr, angle_x, angle_y);
        
        angle_x += 0.05; // Increment angle for rotation around x-axis
        angle_y += 0.05; // Increment angle for rotation around y-axis

        usleep(20000); // Sleep to control the speed of rotation

        // Check for user input to exit
        if (getch() != ERR) break; // Break if any key is pressed
    }

    endwin();             // End ncurses mode
    return 0;
}
