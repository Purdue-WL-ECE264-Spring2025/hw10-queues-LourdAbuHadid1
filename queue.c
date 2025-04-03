#include "queue.h"
#include <stdlib.h>
#include <stdbool.h>

// helper function to check if a game state is solved
static bool is_solved(const struct game_state *state) {
  uint8_t expected = 1;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (i == 3 && j == 3) {
        if (state->tiles[i][j] != 0) return false;
      } else {
        if (state->tiles[i][j] != expected++) return false;
      }
    }
  }
  return true;
}

// helper function to check if a state has been visited
static bool is_visited(struct linked_list *visited, uint64_t serialized) {
  struct list_node *current = visited->head;
  while (current != NULL) {
    if (current->value == serialized) {
      return true;
    }
    current = current->next;
  }
  return false;
}

// initialize a new queue (not in header but needed internally)
static struct queue new_queue() {
  struct queue q;
  q.data.head = NULL;
  return q;
}

// check if queue is empty (not in header but needed internally)
static bool empty(struct queue *q) {
  return q->data.head == NULL;
}

// free queue memory (not in header but needed internally)
static void free_queue(struct queue *q) {
  free_list(q->data);
}

void enqueue(struct queue *q, struct game_state state) {
  uint64_t serialized = serialize(state);
  insert_at_tail(&q->data, serialized);
}

struct game_state dequeue(struct queue *q) {
  uint64_t serialized = remove_from_head(&q->data);
  return deserialize(serialized);
}

int number_of_moves(struct game_state start) {
  // check if already solved
  if (is_solved(&start)) {
    return 0;
  }

  // initialize queue and visited list
  struct queue q = new_queue();
  struct linked_list visited = {NULL};

  // enqueue starting state
  enqueue(&q, start);
  insert_at_tail(&visited, serialize(start));

  while (!empty(&q)) {
    struct game_state current = dequeue(&q);

    // generate all possible moves
    struct game_state moves[4];
    int num_moves = 0;

    if (current.empty_row > 0) { // can move down
      moves[num_moves] = current;
      move_down(&moves[num_moves]);
      num_moves++;
    }
    if (current.empty_row < 3) { // can move up
      moves[num_moves] = current;
      move_up(&moves[num_moves]);
      num_moves++;
    }
    if (current.empty_col > 0) { // can move right
      moves[num_moves] = current;
      move_right(&moves[num_moves]);
      num_moves++;
    }
    if (current.empty_col < 3) { // can move left
      moves[num_moves] = current;
      move_left(&moves[num_moves]);
      num_moves++;
    }

    // process each move
    for (int i = 0; i < num_moves; i++) {
      // check if solved
      if (is_solved(&moves[i])) {
        int result = moves[i].num_steps;
        free_queue(&q);
        free_list(visited);
        return result;
      }

      // check if visited
      uint64_t serialized = serialize(moves[i]);
      if (!is_visited(&visited, serialized)) {
        enqueue(&q, moves[i]);
        insert_at_tail(&visited, serialized);
      }
    }
  }

  free_queue(&q);
  free_list(visited);
  return -1; // should never reach here for solvable puzzles
}
