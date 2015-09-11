/********************************************************
 * Brianna Carter
 * 7/19/15
 * This program creates a circularly double linked list
 * with one sentinel, has functions to create new lists,
 * add links to both the front and the back, 
 * remove links from both the front and back, and reverse
 * the order of the list. 
 * *****************************************************/



#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <float.h>
#include "cirListDeque.h"

/* Double Link Struture */
struct DLink {
	TYPE value;/* value of the link */
	struct DLink * next;/* pointer to the next link */
	struct DLink * prev;/* pointer to the previous link */
};

# define TYPE_SENTINEL_VALUE DBL_MAX 


/* ************************************************************************
 Deque ADT based on Circularly-Doubly-Linked List WITH Sentinel
 ************************************************************************ */

struct cirListDeque {
	int size;/* number of links in the deque */
	struct DLink *Sentinel;	/* pointer to the sentinel */
};
/* internal functions prototypes */
struct DLink* _createLink (TYPE val);
void _addLinkAfter(struct cirListDeque *q, struct DLink *lnk, TYPE v);
void _removeLink(struct cirListDeque *q, struct DLink *lnk);



/* ************************************************************************
	Deque Functions
************************************************************************ */

/* Initialize deque.

	param: 	q		pointer to the deque
	pre:	q is not null
	post:	q->Sentinel is allocated and q->size equals zero
*/
void _initCirListDeque (struct cirListDeque *q) 
{
  	q->Sentinel = malloc(sizeof(struct DLink)); //save space for links
  	assert(q->Sentinel !=0);	//make sure sentinel isn't null
  	q->Sentinel->next = q->Sentinel;//Sentinel points to itself 
  	q->Sentinel->prev = q->Sentinel;//initially
  	q->size = 0;
}

/*
 create a new circular list deque
 
 */

struct cirListDeque *createCirListDeque()
{
	struct cirListDeque *newCL = malloc(sizeof(struct cirListDeque));//allocate space for list
	_initCirListDeque(newCL);//initiate the new list
	return(newCL);
}


/* Create a link for a value.

	param: 	val		the value to create a link for
	pre:	none
	post:	a link to store the value
*/
struct DLink * _createLink (TYPE val)
{
	struct DLink *l = (struct DLink*) malloc(sizeof(struct DLink));//create a new link
	l->value = val;

	return l;	 

}

/* Adds a link after another link

	param: 	q		pointer to the deque
	param: 	lnk		pointer to the existing link in the deque
	param: 	v		value of the new link to be added after the existing link
	pre:	q is not null
	pre: 	lnk is not null
	pre:	lnk is in the deque 
	post:	the new link is added into the deque after the existing link
*/
void _addLinkAfter(struct cirListDeque *q, struct DLink *lnk, TYPE v)
{
	assert(q != 0);
	assert(lnk != 0);

	struct DLink *new = _createLink(v);	 //create the link

	lnk->next->prev = new;		//make sure next and prev are correct
	new->next = lnk->next;	
	new->prev = lnk;
	lnk->next = new;
	q->size++;			//increase size of list

//if the sentinel is by link, make sure links point to it correctly
	if(lnk == q->Sentinel)
	{
		q->Sentinel->next->prev = new;
		q->Sentinel->next = new;
	}
	if(lnk == q->Sentinel->prev)
	{
		q->Sentinel->prev->next = new;
		q->Sentinel->prev = new;
		new->next = q->Sentinel;
	}

}

/* Adds a link to the back of the deque

	param: 	q		pointer to the deque
	param: 	val		value for the link to be added
	pre:	q is not null
	post:	a link storing val is added to the back of the deque
*/
void addBackCirListDeque (struct cirListDeque *q, TYPE val) 
{
	assert(q != 0);
	_addLinkAfter(q, q->Sentinel->prev, val);	 

}

/* Adds a link to the front of the deque

	param: 	q		pointer to the deque
	param: 	val		value for the link to be added
	pre:	q is not null
	post:	a link storing val is added to the front of the deque
*/
void addFrontCirListDeque(struct cirListDeque *q, TYPE val)
{
	assert(q != 0);
	_addLinkAfter(q, q->Sentinel, val);	


}

/* Get the value of the front of the deque

	param: 	q		pointer to the deque
	pre:	q is not null and q is not empty
	post:	none
	ret: 	value of the front of the deque
*/
TYPE frontCirListDeque(struct cirListDeque *q) 
{
	assert(!isEmptyCirListDeque(q));
	TYPE data = q->Sentinel->next->value;
	return data;
}

/* Get the value of the back of the deque

	param: 	q		pointer to the deque
	pre:	q is not null and q is not empty
	post:	none
	ret: 	value of the back of the deque
*/
TYPE backCirListDeque(struct cirListDeque *q)
{
	assert(!isEmptyCirListDeque(q));
	TYPE data = q->Sentinel->prev->value;	 
	return data;
}

/* Remove a link from the deque

	param: 	q		pointer to the deque
	param: 	lnk		pointer to the link to be removed
	pre:	q is not null and q is not empty
	post:	the link is removed from the deque
*/
void _removeLink(struct cirListDeque *q, struct DLink *lnk)
{
	
	//before removing, make sure the Sentinel's new links are correct
	if(lnk == q->Sentinel->prev)
	{
		lnk->prev->next = q->Sentinel;
		q->Sentinel->prev = lnk->prev;
	}
	else if(lnk == q->Sentinel->next)
	{
		lnk->next->prev = q->Sentinel->prev;
		q->Sentinel->next = lnk->next;
	}
	else
	{
		lnk->next->prev = lnk->prev;
		lnk->prev->next = lnk->next;
	}

	free(lnk);
 	

}

/* Remove the front of the deque

	param: 	q		pointer to the deque
	pre:	q is not null and q is not empty
	post:	the front is removed from the deque
*/
void removeFrontCirListDeque (struct cirListDeque *q) {

	assert(!isEmptyCirListDeque(q));
	_removeLink(q, q->Sentinel->next); 
	q->size--;
}


/* Remove the back of the deque

	param: 	q		pointer to the deque
	pre:	q is not null and q is not empty
	post:	the back is removed from the deque
*/
void removeBackCirListDeque(struct cirListDeque *q)
{
  	assert(!isEmptyCirListDeque(q));
  	_removeLink(q, q->Sentinel->prev); 
	q->size--;
}

/* De-allocate all links of the deque

	param: 	q		pointer to the deque
	pre:	none
	post:	All links (including Sentinel) are de-allocated
*/
void freeCirListDeque(struct cirListDeque *q)
{
	assert(!isEmptyCirListDeque(q));
	for(int x; x < q->size; x++)
	{
		removeFrontCirListDeque(q);
		
	}	 
	free(q->Sentinel);
}

/* Check whether the deque is empty

	param: 	q		pointer to the deque
	pre:	q is not null
	ret: 	1 if the deque is empty. Otherwise, 0.
*/
int isEmptyCirListDeque(struct cirListDeque *q) {
  
	if(q->size == 0)
  	{
  		return 1;
  	}
  	else
	return 0;
}

/* Print the links in the deque from front to back

	param: 	q		pointer to the deque
	pre:	q is not null and q is not empty
	post: 	the links in the deque are printed from front to back
*/
void printCirListDeque(struct cirListDeque *q)
{
	assert(!isEmptyCirListDeque(q));
	assert(q != 0);
	struct DLink *current = q->Sentinel->next;

	for(int x = 0; x < q->size; x++)
	{
		if(current!= q->Sentinel)
		{
		printf("Value %i: %lf\n", x+1, current->value);
		current = current->next; 
		}
	}

}

/* Reverse the deque

	param: 	q		pointer to the deque
	pre:	q is not null and q is not empty
	post: 	the deque is reversed
*/
void reverseCirListDeque(struct cirListDeque *q)
{
	int y = q->size;
	struct cirListDeque* n = createCirListDeque();
	struct DLink *qCurrent = q->Sentinel->next;
	
	for(int x = 0; x < y; x++)
	{
		
		TYPE val = qCurrent->value;
		addFrontCirListDeque(n, val);
		qCurrent = qCurrent->next;
	}
	
		qCurrent = q->Sentinel->prev;
		struct DLink *nCurrent = n->Sentinel->next;

		while(qCurrent != q->Sentinel)
		{	nCurrent->value = qCurrent->value;
			nCurrent = nCurrent->next;
			qCurrent = qCurrent->next;
			removeFrontCirListDeque(q);
		}
	
	q->Sentinel = n->Sentinel;
	q->size = n->size;
	free(n);

}
