#include "state.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snake_utils.h"



/* Helper function definitions */
static void set_board_at(game_state_t* state, unsigned int row, unsigned int col, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_row(unsigned int cur_row, char c);
static unsigned int get_next_col(unsigned int cur_col, char c);
static void find_head(game_state_t* state, unsigned int snum);
static char next_square(game_state_t* state, unsigned int snum);
static void update_tail(game_state_t* state, unsigned int snum);
static void update_head(game_state_t* state, unsigned int snum);

/* Task 1 */
game_state_t* create_default_state() {
  game_state_t* state;
  int columns = 20;
  int rows = 18;

  //初始化棋盘
  state = (game_state_t*) malloc(sizeof(game_state_t));
  state->num_rows = rows;
  state->num_snakes = 1;
  state->board = (char**) malloc(sizeof(char*) * state->num_rows);
  for (int i = 0; i < rows; i++) {
    state->board[i] = (char*) malloc(sizeof(char) * (columns + 1));
  }
  strcpy(state->board[0], "####################");
  strcpy(state->board[1], "#                  #");
  strcpy(state->board[2], "# d>D    *         #");
  strcpy(state->board[3], "#                  #");
  strcpy(state->board[4], "#                  #");
  strcpy(state->board[5], "#                  #");
  strcpy(state->board[6], "#                  #");
  strcpy(state->board[7], "#                  #");
  strcpy(state->board[8], "#                  #");
  strcpy(state->board[9], "#                  #");
  strcpy(state->board[10], "#                  #");
  strcpy(state->board[11], "#                  #");
  strcpy(state->board[12], "#                  #");
  strcpy(state->board[13], "#                  #");
  strcpy(state->board[14], "#                  #");
  strcpy(state->board[15], "#                  #");
  strcpy(state->board[16], "#                  #");
  strcpy(state->board[17], "####################");
  


  //初始化蛇的位置
  state->snakes = (snake_t*)malloc(sizeof(snake_t));
  state->snakes->head_row = 2;
  state->snakes->head_col = 4;
  state->snakes->tail_row = 2;
  state->snakes->tail_col = 2;
  state->snakes->live = true;

  return state;
}

/* Task 2 */
void free_state(game_state_t* state) {
  // TODO: Implement this function.

  //free 数组的行
  for (int i = 0; i < state->num_rows; i++) {
    free(state->board[i]);
  }

  //free数组本身
  free(state->board);

  //free掉snakes
  free(state->snakes);

  free(state);
  state = NULL;
  return;
}

/* Task 3 */
void print_board(game_state_t* state, FILE* fp) {
  // TODO: Implement this function.

  for (int i = 0; i < state->num_rows; i++) {
    fprintf(fp, "%s\n", state->board[i]);
  }
  return;
}

/*
  Saves the current state into filename. Does not modify the state object.
  (already implemented for you).
*/
void save_board(game_state_t* state, char* filename) {
  FILE* f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */

/*
  Helper function to get a character from the board
  (already implemented for you).
*/
char get_board_at(game_state_t* state, unsigned int row, unsigned int col) {
  return state->board[row][col];
}

/*
  Helper function to set a character on the board
  (already implemented for you).
*/
static void set_board_at(game_state_t* state, unsigned int row, unsigned int col, char ch) {
  state->board[row][col] = ch;
}

/*
  Returns true if c is part of the snake's tail.
  The snake consists of these characters: "wasd"
  Returns false otherwise.
*/
static bool is_tail(char c) {
  // TODO: Implement this function.
  if (c == 'w' || c == 's' || c == 'a' || c == 'd') return true;

  return false;
}

/*
  Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASDx"
  Returns false otherwise.
*/
static bool is_head(char c) {
  // TODO: Implement this function.
  if (c == 'W' || c == 'S' || c == 'A' || c == 'D' || c == 'x') return true;
  return false;
}

/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<v>WASDx"
*/
static bool is_snake(char c) {
  // TODO: Implement this function.
  if (is_head(c) || is_tail(c) || c == '^' || c == '<' || c == 'v' || c == '>') return true;
  return false;
}

/*
  Converts a character in the snake's body ("^<v>")
  to the matching character representing the snake's
  tail ("wasd").
*/
static char body_to_tail(char c) {
  // TODO: Implement this function.
  if (c == '^') {
    return 'w';
  } else if (c == '<') {
    return 'a';
  } else if (c == 'v') {
    return 's';
  } else if (c == '>') {
    return 'd';
  }
  return '?';
}

/*
  Converts a character in the snake's head ("WASD")
  to the matching character representing the snake's
  body ("^<v>").
*/
static char head_to_body(char c) {
  // TODO: Implement this function.
    if (c == 'W') {
    return '^';
  } else if (c == 'A') {
    return '<';
  } else if (c == 'S') {
    return 'v';
  } else if (c == 'D') {
    return '>';
  }
  return '?';
}

/*
  Returns cur_row + 1 if c is 'v' or 's' or 'S'.
  Returns cur_row - 1 if c is '^' or 'w' or 'W'.
  Returns cur_row otherwise.
*/
static unsigned int get_next_row(unsigned int cur_row, char c) {
  // TODO: Implement this function.
  if (c == 'v' || c == 's' || c == 'S') return cur_row + 1;
  if (c == '^' || c == 'w' || c == 'W') return cur_row - 1;
  return cur_row;
}

/*
  Returns cur_col + 1 if c is '>' or 'd' or 'D'.
  Returns cur_col - 1 if c is '<' or 'a' or 'A'.
  Returns cur_col otherwise.
*/
static unsigned int get_next_col(unsigned int cur_col, char c) {
  // TODO: Implement this function.
  if (c == '>' || c == 'd' || c == 'D') return cur_col + 1;
  if (c == '<' || c == 'a' || c == 'A') return cur_col - 1;
  return cur_col;
}

/*
  Task 4.2

  Helper function for update_state. Return the character in the cell the snake is moving into.

  This function should not modify anything.
*/
static char next_square(game_state_t* state, unsigned int snum) {
  // TODO: Implement this function.
  int head_row = state->snakes[snum].head_row;
  int head_column = state->snakes[snum].head_col;

  char c = get_board_at(state, head_row, head_column);

  int next_head_row = get_next_row(head_row, c);
  int next_head_column = get_next_col(head_column, c);
  
  return get_board_at(state, next_head_row, next_head_column);
}

/*
  Task 4.3

  Helper function for update_state. Update the head...

  ...on the board: add a character where the snake is moving

  ...in the snake struct: update the row and col of the head

  Note that this function ignores food, walls, and snake bodies when moving the head.
*/
static void update_head(game_state_t* state, unsigned int snum) {
  // TODO: Implement this function.
  int head_row = state->snakes[snum].head_row;
  int head_column = state->snakes[snum].head_col;

  char c = get_board_at(state, head_row, head_column);
  int next_head_row = get_next_row(head_row, c);
  int next_head_column = get_next_col(head_column, c);



  state->snakes[snum].head_row = next_head_row;
  state->snakes[snum].head_col = next_head_column;

  set_board_at(state, next_head_row, next_head_column, c);
  set_board_at(state, head_row, head_column, head_to_body(c));
  return;
}

/*
  Task 4.4

  Helper function for update_state. Update the tail...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^<v>) into a tail character (wasd)

  ...in the snake struct: update the row and col of the tail
*/
static void update_tail(game_state_t* state, unsigned int snum) {
  // TODO: Implement this function.

  int tail_row = state->snakes[snum].tail_row;
  int tail_column = state->snakes[snum].tail_col;

  char c = get_board_at(state, tail_row, tail_column);
  int next_tail_row = get_next_row(tail_row, c);
  int next_tail_column = get_next_col(tail_column, c);
  
  state->snakes[snum].tail_row = next_tail_row;
  state->snakes[snum].tail_col = next_tail_column;

  char body_char = get_board_at(state, next_tail_row, next_tail_column);

  set_board_at(state, next_tail_row, next_tail_column, body_to_tail(body_char));
  set_board_at(state, tail_row, tail_column, ' ');

  return;
}

/* Task 4.5 */
void update_state(game_state_t* state, int (*add_food)(game_state_t* state)) {
  // TODO: Implement this function.

  for (int i = 0; i < state->num_snakes; i++) {
    if (!state->snakes[i].live) continue;

    char next = next_square(state, i);

    if (next == '#' || is_snake(next)) {
      state->snakes[i].live = false;
      set_board_at(state, state->snakes[i].head_row, state->snakes[i].head_col, 'x');
    } else if (next == '*') {
      update_head(state, i);
      add_food(state);
    } else {
      update_head(state, i);
      update_tail(state, i);
    }
  }
  return;
}

/* Task 5 */
game_state_t* load_board(char* filename) {
  // TODO: Implement this function.
  return NULL;
}

/*
  Task 6.1

  Helper function for initialize_snakes.
  Given a snake struct with the tail row and col filled in,
  trace through the board to find the head row and col, and
  fill in the head row and col in the struct.
*/
static void find_head(game_state_t* state, unsigned int snum) {
  // TODO: Implement this function.
  return;
}

/* Task 6.2 */
game_state_t* initialize_snakes(game_state_t* state) {
  // TODO: Implement this function.
  return NULL;
}
