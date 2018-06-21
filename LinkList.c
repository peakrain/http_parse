#include"LinkList.h"
#include<malloc.h>
list_t list_create()
{
	list_t list=(list_t)malloc(sizeof(struct LNode));
	if(!list)
	{
		printf("list can't be create!\n");
		return NULL;
	}
	list->next=NULL;
	return list;
}
void list_insert(list_t list,void *element)
{
	list_t p=list;
	while(p->next!=NULL)
		p=p->next;
	list_t s=(list_t)malloc(sizeof(struct LNode));
	s->element=element;
	s->next=p->next;
	p->next=s;
}
void list_getElem(list_t list,int i,void *element)
{
	int j=0;
	list_t p=list;
	while(p->next!=NULL&&j<i)
	{
		p=p->next;
		j++;
	}
	element=p->element;
}
void list_delete(list_t list,int i,void *element);
int list_length(list_t list);
