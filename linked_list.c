#include "linked_list.h"
#include <stdlib.h>

struct list_node *new_node(size_t value) { return NULL; }

void insert_at_head(struct linked_list *list, size_t value) {
  struct list_node *new_node = malloc(sizeof(struct list_node));
  if (new_node == NULL){
    return;
  }
  new_node->value = value;
  new_node->next = list->head;
  list->head = new_node;
}

void insert_at_tail(struct linked_list *list, size_t value) {
  struct list_node *new_node = malloc(sizeof(struct list_node));
  if (new_node == NULL){
    return;
  }
  new_node->value = value;
  new_node->next = NULL;

  if (list->head == NULL) {
    list->head = new_node;
  }
  else {
    struct list_node *current = list->head;
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = new_node;
  }
}

size_t remove_from_head(struct linked_list *list) {
  if (list->head == NULL) {
    return 0;
  }
  struct list_node *to_remove = list->head;
  size_t value = to_remove->value;

  list->head = to_remove->next;
  free(to_remove);
  return value;
}

size_t remove_from_tail(struct linked_list *list) {
    if (list->head == NULL) {
        return 0;
    }
    
    struct list_node *to_remove = list->head;
    size_t value;
    
    if (to_remove->next == NULL) {
        value = to_remove->value;
        list->head = NULL;
        free(to_remove);
        return value;
    }
    
    while (to_remove->next->next != NULL) {
        to_remove = to_remove->next;
    }
    
    value = to_remove->next->value;
    free(to_remove->next);
    to_remove->next = NULL;
    
    return value;
}

void free_list(struct linked_list list) {
 struct list_node *current = list.head;
    while (current != NULL) {
        struct list_node *to_free = current;
        current = current->next;
        free(to_free);
    }
}

// Utility function to help you debugging, do not modify
void dump_list(FILE *fp, struct linked_list list) {
  fprintf(fp, "[ ");
  for (struct list_node *cur = list.head; cur != NULL; cur = cur->next) {
    fprintf(fp, "%zu ", cur->value);
  }
  fprintf(fp, "]\n");
}
