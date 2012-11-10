#include "stdafx.h"
#include "my570list.hpp"
#include <iostream>

using namespace std;

My570List::My570List()
{
	num_members = 0;
	My570ListElem *anchor = new My570ListElem();
	anchor->prev = anchor;
	anchor->next = anchor;
	this->anchor = anchor;
}

My570List::~My570List()
{
	if(anchor != NULL)
	delete anchor;//Deallocating List members should be taken care by separately.	
}

int My570List::Append(void *obj)
{
	My570ListElem *insert_Elem = new My570ListElem();
	insert_Elem -> obj = obj;
	insert_Elem -> next = anchor;
	insert_Elem -> prev = anchor->prev;
	anchor->prev-> next = insert_Elem;
	anchor->prev = insert_Elem;
	num_members++;
	return TRUE;
}

int My570List::Prepend(void *obj)
{
	My570ListElem* insert_Elem = new My570ListElem();
	insert_Elem->obj = obj;
	insert_Elem->next = anchor->next;
	insert_Elem->prev = anchor;
	anchor -> next->prev = insert_Elem;
	anchor -> next = insert_Elem;
	num_members++;
	return TRUE;
}

void My570List::Unlink(My570ListElem* E)
{
	My570ListElem* prev_E = E->prev;
	My570ListElem* next_E = E->next;
	E->next = NULL;
	E->prev = NULL;
	prev_E -> next = next_E;
	next_E -> prev = prev_E;
	num_members--;
	if(E != NULL)
	delete E;
	E = NULL;
	return;
}

void My570List::UnlinkAll()
{
	/*
	My570ListElem *iElem = Next(anchor);
	for(My570ListElem *nElem = Next(iElem); Next(iElem) != NULL;)
	{
		iElem -> next = iElem -> prev = NULL; 
		if(iElem != NULL)
		delete iElem;
		iElem = nElem; 
		nElem = Next(nElem);
	}
	iElem -> next = iElem -> prev = NULL;
	if(iElem != NULL) delete iElem;
	*/
	My570ListElem *iElem = First();
	while(iElem != NULL)
	{
		this->Unlink(iElem);
		iElem = First();
	}
	num_members = 0;
	return;
}

int My570List::InsertBefore(void *obj, My570ListElem *elem)
{
	//cout<<"hi";
	if(elem == NULL){return Prepend(obj); }
	My570ListElem *new_Elem = new My570ListElem();
	new_Elem -> obj = obj;
	new_Elem -> prev = elem -> prev;
	//cout<<"After assign newElem->prev";
	new_Elem -> next = elem;
	//cout<<"After assign newElem->next";
	elem->prev->next = new_Elem;
	//cout<<"After assign elem->prev->next";
	elem->prev = new_Elem;
	//cout<<"After elem->prev";
	num_members++;
	return 1;
}

int  My570List::InsertAfter(void *obj, My570ListElem *elem)
{
	if(elem == NULL){return Append(obj);}
	My570ListElem *new_Elem = new My570ListElem();
	new_Elem -> obj = obj;
	new_Elem -> prev = elem;
	new_Elem -> next = elem->next;
	elem->next->prev = new_Elem;
	elem->next = new_Elem;
	num_members++;
	return 1;
}

My570ListElem* My570List::First()
{
	return Next(anchor);
}

My570ListElem* My570List::Last()
{
	return Prev(anchor);
}

My570ListElem* My570List::Next(My570ListElem *cur)
{
	return cur->next == anchor ? NULL : cur->next;
}

My570ListElem* My570List::Prev(My570ListElem *cur)
{
	return (cur->prev == anchor) ? NULL : cur->prev;
}

My570ListElem* My570List::Find(void *obj)
{
	My570ListElem *E = NULL;
	
	for(E = this->First(); E != NULL; E = Next(E))	{if(E->Obj() == obj)	return E;}
	return NULL;
}

My570ListElem::My570ListElem(void *object)
{
	this->obj = object;
}

My570ListElem::~My570ListElem()
{
	;
}
