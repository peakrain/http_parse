#ifndef _linklist_h
#define _lisklist_h

typedef struct node{
	void *element;
	struct node* next;
}Node;
typedef Node *list_t;

list_t list_create();
void list_insert(list_t list,void *element);
void list_getElem(list_t list,int i,void *element);
void list_delete(list_t list,int i,void *element);
int list_length(list_t list);
#endif
