#include "arve/terminal.h"
#include <termios.h>
#include <unistd.h>
namespace arve{


void direct_input() {
    struct termios ctrl {};
    tcgetattr(STDIN_FILENO, &ctrl);
    ctrl.c_lflag &= ~ICANON;
    ctrl.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &ctrl);
}

void reset_terminal() {
    struct termios ctrl {};
    tcgetattr(STDIN_FILENO, &ctrl);
    ctrl.c_lflag |= ICANON;
    ctrl.c_lflag |= ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &ctrl);
}

}