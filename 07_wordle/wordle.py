import sys


def move_cursor(x: int, y: int, message=None):
    assert x >= 0 and y >= 0, "Dimensions x, y cannot be negative!"
    x *= 2
    if message is None:
        sys.stdout.write(f"\033[{y};{x}H")
    sys.stdout.write(f"\033[{y};{x}H{message}")


def clear_terminal():
    sys.stdout.write("\033[2J")
    sys.stdout.flush()


def draw_box(width: int, height: int, x=0, y=0) -> None:
    assert width >= 3 and height >= 3, "Invalid width or height. Fix it dummy!"
    width *= 2
    top = "┌" + "─" * (width - 2) + "┐"
    bottom = "└" + "─" * (width - 2) + "┘"
    middle = "│" + " " * (width - 2) + "│"

    move_cursor(x, y, top)
    for i in range(1, height - 1):
        move_cursor(x, y + i, middle)
    move_cursor(x, y + height - 1, bottom)


def wordle():
    clear_terminal()
    draw_box(20, 20, 1, 1)
    draw_box(7, 10, 2, 2)

    move_cursor(3, 3, "▒")
    move_cursor(4, 3, "▒")
    move_cursor(5, 3, "▒")
    move_cursor(6, 3, "▒")
    move_cursor(7, 3, "▒")

    move_cursor(3, 5, "▒")
    move_cursor(4, 5, "▒")
    move_cursor(5, 5, "▒")
    move_cursor(6, 5, "▒")
    move_cursor(7, 5, "▒")


if __name__ == "__main__":
    wordle()
