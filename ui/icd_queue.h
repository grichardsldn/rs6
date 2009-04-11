/***************************************************************************
 * Module   : icd_queue.h
 *
 * Summary  : Queue handling routines
 *
 * Project  : Ethernet PIE P3 ICD
 * 
 * Author   : G.Richards
 * 
 * Created  : Tue Jan 19 15:49:08 GMT 1999
 *
 * Status   : Basic tested under unixware, documentation incomplete.
 *
 * Design Documentation : ICD design is 1SAA15310AAC/BTA
 *
 * This header contains macros for the manipulation of queues.  These
 * are implemented as macros so that any datatypes can be used without
 * emplying nasty casts.
 *
 * 'queue' macros:
 * 
 * A queue is a singly-linked-list, items are added to the tail of the
 * queue, items may nly be removed from the head of the queue.
 * 
 * Two structures are used, and item structure and a queue structure.
 * These may of any form, but must include specific field names.
 *
 * Eg:
 * struct MyItemStruct 
 * { 
 *	struct MyItemStruct * next;
 * 	"my own fields here..."
 * };
 *
 * struct MyQueue
 * {
 * 	struct MyItemStruct *head;
 * 	struct MyItemStruct *tail;
 * 	struct int count;
 *	"my own fields here..." 
 * };
 * 
 *
 ***************************************************************************
 * 
 * Revision History: 
 *
 * Date		Version		Description		Made by	
 * Jan 19       1.1             initial - queue only    GDR
 * Feb 18	1.2		added set macros	GDR
 * Apr 9	1.3		changed next > b_next	GDR
 * Oct 20	1.4		changed back		GDR
 *
 ***************************************************************************
 * 
 * THIS IS AN UNPUBLISHED WORK  THE COPYRIGHT IN WHICH VESTS IN GPT LIMITED.
 * ALL RIGHTS RESERVED. 
 * 
 * THE  INFORMATION  CONTAINED HEREIN IS THE  PROPERTY OF GPT LIMITED AND IS
 * SUPPLIED  WITHOUT LIABILITY  FOR ERRORS  OR  OMISSIONS,  NO  PART MAY  BE
 * REPRODUCED  OR USED  EXCEPT  AS AUTHORISED  BY CONTRACT  OR OTHER WRITTEN
 * PERMISSION, THE COPYRIGHT AND  THE FOREGOING RESTRICTIONS ON REPRODUCTION
 * AND USE EXTEND TO ALL MEDIA IN WHICH THE INFORMATION MAY BE EMBODIED.
 *
 ***************************************************************************/ 

#ifndef __ICD_QUEUE_H_
#define __ICD_QUEUE_H_

/* Queue macros - a 'queue' is a singly-linked-list which can be
 * added to at the tail and items removed only from the head */

#define ENQUEUE(q,i) 	if ((q)->count) (q)->tail->next = i ; \
			else (q)->head = (i); \
			(q)->tail = (i); \
			(q)->count++; \
			(i)->next = NULL; 
	
#define DEQUEUE(q, i) 	if ((q)->count == 0) (i) = NULL; \
			else \
				{(i)  = (q)->head;\
				(q)->head = (i)->next;\
				(i)->next = NULL;\
				(q)->count--;}\

#define INIT_QUEUE(q) (q)->count = 0;(q)->head=NULL;(q)->tail=NULL

/* "set" routines.  A 'set' is a non-ordered doubly-linked-list from
 * which specified items can be removed from anywhere within the
 * list */

#define ENSET(s,i) \
	if ((s)->count){(s)->tail->next=(i);(i)->prev=(s)->tail;}\
	else {(s)->head=(i);(i)->prev=NULL;}\
	(s)->tail=(i); (s)->count++;(i)->next=NULL;

/* TRANSLATION:

void
add_to_set(void *vset, void *vitem)
{
	struct set_template * set;
	struct set_item_template *item;


	set = (struct set_template *)vset;
	item = (struct set_item_template *)vitem;

	if (set->count) 
	{
		set->tail->next = item;
		item->prev = set->tail;
	}
	else 
	{
		set->head = item;
		item->prev = NULL;
	}

	set->tail = item;
	set->count ++;
	item->next = NULL;
} */

#define DESET(s,i) \
	(s)->before=(i)->prev; (s)->after=(i)->next;\
	if ((s)->before == NULL) { (s)->head=(s)->after;} \
	else {(s)->before->next = (s)->after;} \
	if ((s)->after==NULL){(s)->tail=(s)->before;} \
	else {(s)->after->prev = (s)->before;} \
	(s)->count--;

/* TRANSLATION:
void
remove_from_set(void *vset, void *vitem)
{
	struct set_template * set;
	struct set_item_template *item;
	struct set_item_template *before;
	struct set_item_template *after;


	set = (struct set_template *)vset;
	item = (struct set_item_template *)vitem;

	before = item->prev;
	after = item->next;

	if (before == NULL)
	{
		set->head = after;
	}
	else
	{
		before->next = after;
	}

	if (after == NULL)
	{
		set->tail = before;
	}
	else
	{
		after->prev = before;
	}
	set->count --;
} */

#define INIT_SET(s) \
	(s)->head = NULL;\
	(s)->tail = NULL;\
	(s)->count = 0;\
	(s)->before = NULL;\
	(s)->after = NULL;

	
#endif /* __ICD_QUEUE_H_ */
