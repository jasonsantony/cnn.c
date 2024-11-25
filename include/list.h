#pragma once

typedef struct node {
  void *val;
  struct node *next;
  struct node *prev;
} node;

typedef struct list {
  int size;
  node *front;
  node *back;
} list;

list *make_list();
int list_find(list *l, void *val);

void list_insert(list *, void *);

void free_list_contents(list *l);