#include"LinkList.h"
#include<malloc.h>
list_t list_create()
{
	list_t list=(list_t)malloc(sizeof(Node));
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
	list_t s=(list_t)malloc(sizeof(Node));
	s->element=element;
	s->next=p->next;
	p->next=s;
}
void *list_getElem(list_t list,int i)
{
	int j=0;
	list_t p=list;
	while(p->next!=NULL&&j<i)
	{
		p=p->next;
		j++;
	}
	if(p==NULL||j>i||j<i)
		return NULL;
	return p->element;
}
int list_delete(list_t list,int i)
{
	int j=0;
	list_t p=list;
	while(p->next!=NULL&&j<i-1)
	{
		p=p->next;
		j++;
	}
	if((p->next==NULL)||j>i-1||j<i-1)
		return EOF;
	list_t q=p->next;
	p->next=q->next;
	free(q);
	return 0;
}
int list_length(list_t list)
{
	int len=0;
	list_t p=list->next;
	if(p==NULL)
		return 0;
	do
	{
		len++;
		p=p->next;
	}while(p!=NULL);
	return len;
}
