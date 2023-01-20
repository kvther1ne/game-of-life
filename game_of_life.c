#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <curses.h>
#include <time.h>

#define X 80
#define Y 25

void memory(int *** matrix);
int input(int ** matrix);
void run_game(int ** a, int ** b);
void life_act(int ** a, int ** b);
int check_n(int ** a, int x, int y);
void draw(int ** a);

int main() {
  initscr();
  noecho();

  scrollok(stdscr, TRUE);
  nodelay(stdscr, TRUE);

  srand(time(NULL));
  int ** a;
  memory(&a);
  int ** b;
  memory(&b);

  if (input(a)) {
    if (freopen("/dev/tty", "r", stdin)) {
        printf("  ");
    }
    run_game(a, b);
    free(a);
    free(b);
  } else {
    printw("n/a");
  }

  endwin();
  return 0;
}

void memory(int *** matrix) {
  *matrix = (int **)malloc(Y * sizeof(int *) + Y * X * sizeof(int));
  int * start = (int *)(*matrix + Y);
  for (int i = 0; i < Y; i++) {
  (*matrix)[i] = start + i * X;
  }
}

int input(int ** matrix) {
  int f = 1;
  for (int i = 0; i < Y; i++) {
    for (int j = 0; j < X; j++) {
        scanf("%d", &matrix[i][j]);
    }
  }
  return f;
}

void run_game(int ** a, int ** b) {
  int ticks = 0;
  int delay = 150000;
  char knopka = ' ';
  while (knopka != 'q' && ticks != 2000) {
    refresh();
    move(0, 0);
    if (ticks % 2 == 0) {
      life_act(a, b);
      draw(b);
    } else {
      life_act(b, a);
      draw(a);
    }
    ticks++;
    usleep(delay);
    knopka = getch();
    if (knopka == 'a' && delay > 10000) {
      delay -= 10000;
    } else if (knopka == 'z' && delay < 200000) {
      delay += 10000;
    }
  }
}

void life_act(int ** a, int ** b) {
  int n;
  for (int i = 0; i < Y; i++) {
    for (int j = 0; j < X; j++) {
      n = check_n(a, j, i);
      if (a[i][j] == 0) {
        if (n == 3) {
          b[i][j] = 1;
        } else {
          b[i][j] = 0;
        }
      } else {
        if (n < 2 || n > 3) {
          b[i][j] = 0;
        } else {
          b[i][j] = 1;
        }
      }
    }
  }
}

int check_n(int ** a, int x, int y) {
  int neigh = 0;
  int m, n;
  for (int i = -1; i < 2; i++) {
    for (int j = -1; j < 2; j++) {
      if (i == 0 && j == 0) {
        continue;
      }
      m = y + i;
      n = x + j;
      if (m < 0) {
        m = Y + m;
      } else if (m > Y - 1) {
        m = m - Y;
      }
      if (n < 0) {
        n = X + n;
      } else if (n > X - 1) {
        n = n - X;
      }
      if (a[m][n] == 1) {
        neigh++;
      }
    }
  }
  return neigh;
}

void draw(int ** a) {
  for (int i = 0; i < Y; i++) {
    for (int j = 0; j < X; j++) {
      if (a[i][j] == 1) {
        printw("X");
      } else {
        printw(" ");
      }
    }
    if (i < X - 1) {
      printw("\n");
    }
  }
  printw("a - speed up, z - speed down, q - exit");
}
