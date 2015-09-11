#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "hashMap.h"



/*the first hashing function you can use*/
int stringHash1(char * str)
{
	int i;
	int r = 0;
	for (i = 0; str[i] != '\0'; i++)
		r += str[i];
	return r;
}

/*the second hashing function you can use*/
int stringHash2(char * str)
{
	int i;
	int r = 0;
	for (i = 0; str[i] != '\0'; i++)
		r += (i+1) * str[i]; /*the difference between stringHash1 and stringHash2 is on this line*/
	return r;
}

/* initialize the supplied hashMap struct*/
void _initMap (struct hashMap * ht, int tableSize)
{
	int index;
	if(ht == NULL)
		return;
	ht->table = (hashLink**)malloc(sizeof(hashLink*) * tableSize);
	ht->tableSize = tableSize;
	ht->count = 0;
	for(index = 0; index < tableSize; index++)
		ht->table[index] = NULL;
}

/* allocate memory and initialize a hash map*/
hashMap *createMap(int tableSize) {
	assert(tableSize > 0);
	hashMap *ht;
	ht = malloc(sizeof(hashMap));
	assert(ht != 0);
	_initMap(ht, tableSize);
	return ht;
}

/*
 Free all memory used by the buckets.
 Note: Before freeing up a hashLink, free the memory occupied by key and value
 */
void _freeMap (struct hashMap * ht)
{  
	/*write this*/
	for (int x = 0; x < ht->tableSize; x++)
	{

		struct hashLink *current = ht->table[x];
		struct hashLink *nextL;
		while (current != NULL)
		{
			char* T = current->key;
			nextL = current->next;
			free(T);
			current = nextL;

		}
	}
}

/* Deallocate buckets and the hash map.*/
void deleteMap(hashMap *ht) {
	assert(ht!= 0);
	/* Free all memory used by the buckets */
	_freeMap(ht);
	/* free the hashMap struct */
	free(ht);
}

/* 
Resizes the hash table to be the size newTableSize 
*/
void _setTableSize(struct hashMap * ht, int newTableSize)
{
	/*write this*/		
	struct hashLink *temp;
	struct hashLink *current;// = (struct hashLink *)malloc(sizeof(struct hashLink));
	struct hashMap *new = createMap(newTableSize);
	

	for (int x = 0; x < ht->tableSize; x++)
	{
		current = ht->table[x];

		while(current != NULL)
		{
			insertMap(new, current->key, current->value);
				temp = current;
				current = current->next;
		}
	}
	struct hashLink **tempT = ht->table;
	ht->tableSize = newTableSize;
	ht->table = new->table;
	
	free(tempT);
}

/*
 insert the following values into a hashLink, you must create this hashLink but
 only after you confirm that this key does not already exist in the table. For example, you
 cannot have two hashLinks for the word "taco".
 
 if a hashLink already exists in the table for the key provided you should
 replace that hashLink--this requires freeing up the old memory pointed by hashLink->value
 and then pointing hashLink->value to value v.
 
 also, you must monitor the load factor and resize when the load factor is greater than
 or equal LOAD_FACTOR_THRESHOLD (defined in hashMap.h).
 */
void insertMap (struct hashMap * ht, KeyType k, ValueType v)
{  
	/*write this*/	
	int hashIndex;
	
	if (HASHING_FUNCTION == 1)
		{
			hashIndex = stringHash1(k) % ht->tableSize;
		}
	else if (HASHING_FUNCTION == 2)
		{
			hashIndex = stringHash2(k) % ht->tableSize;
		}
	if (hashIndex < 0)
		{
			hashIndex += ht->tableSize;
		}


	if (containsKey(ht, k) == 0)
	{

		struct hashLink *newLink = (struct hashLink *)malloc(sizeof(struct hashLink));
		assert(newLink != 0);
		newLink->value = v;
		newLink->key = k;
		newLink->next = ht->table[hashIndex];
		ht->table[hashIndex] = newLink;


	
		ht->count += 1;
	
	}
	else if(containsKey(ht, k) != 0)
	{
		struct hashLink *current = ht->table[hashIndex];
		while (current != NULL)
		{
			if (strcmp(current->key, k) == 0)
			{	
				current->value = v;

			}
			else
			{
				current = current->next;
			}

		}
	}

		if(tableLoad(ht) > LOAD_FACTOR_THRESHOLD)
		{
			_setTableSize(ht, (ht->tableSize) * 2);
		}
}

/*
 this returns the value (which is void*) stored in a hashLink specified by the key k.
 
 if the user supplies the key "taco" you should find taco in the hashTable, then
 return the value member of the hashLink that represents taco.
 
 if the supplied key is not in the hashtable return NULL.
 */
ValueType* atMap (struct hashMap * ht, KeyType k)
{ 
	/*write this*/

	int x;
	if (HASHING_FUNCTION == 1)
	{
		x = stringHash1(k) % ht->tableSize;
	}
	else if (HASHING_FUNCTION == 2)
	{
		x = stringHash2(k) % ht->tableSize;
	}

	if (x < 0)
	{
		x =+ ht->tableSize;
	}
	
	struct hashLink *current = ht->table[x];
	while (current != NULL)
	{
		if (strcmp(current->key, k) == 0)
		{
			
			return &current->value;
		}
		else
		{
			current = current->next;
		}
	}

	return NULL;

}

/*
 a simple yes/no if the key is in the hashtable. 
 0 is no, all other values are yes.
 */
int containsKey (struct hashMap * ht, KeyType k)
{  
	/*write this*/
	int index;
	int contains = 0;
	if (HASHING_FUNCTION == 1)
	{
		index = stringHash1(k) % ht->tableSize;
	}
	else if (HASHING_FUNCTION == 2)
	{
		index = stringHash2(k) % ht->tableSize;
	}
	if (index < 0)
	{
		index += ht->tableSize;
	}

	struct hashLink *current = ht->table[index];
	while (current != NULL)
	{
		if (strcmp(current->key, k) == 0)
		{
			contains = 1;
			return contains;
			
		}
		else
		{
			current = current->next;
		}
	}
	return contains;
	
}

/*
 find the hashlink for the supplied key and remove it, also freeing the memory
 for that hashlink. it is not an error to be unable to find the hashlink, if it
 cannot be found do nothing (or print a message) but do not use an assert which
 will end your program.
 */
void removeKey (struct hashMap * ht, KeyType k)
{  
	/*write this*/	
	int hashIndex;
	if (HASHING_FUNCTION == 1)
	{
		hashIndex = stringHash1(k) % ht->tableSize;
	}
	else if (HASHING_FUNCTION == 2)
	{
		hashIndex = stringHash2(k) % ht->tableSize;
	}

	if (hashIndex < 0)
	{
		hashIndex += ht->tableSize;
	}

	struct hashLink *temp;
	struct hashLink *prev;
	struct hashLink *current;
	current = ht->table[hashIndex];

	if(current == NULL)
		{
			return;}

	else if(strcmp(current->key, k) == 0)
	{
		
		temp = current;
		current = current->next;
		ht->table[hashIndex] = ht->table[hashIndex]->next;
		current = NULL;
		free(temp);
		return;
	}

	else
	{
		while (current != NULL)
		{
			if (strcmp(current->key, k) == 0)
			{
				temp = current;
				prev->next = current->next;
				free(temp);
				ht->count -= 1;
			
			}
			else
			{
				prev = current;
				current = current->next;	
			}
		
		}
	}

	
}

/*
 returns the number of hashLinks in the table
 */
int size (struct hashMap *ht)
{  
	/*write this*/
	return ht->count;
	
}

/*
 returns the number of buckets in the table
 */
int capacity(struct hashMap *ht)
{  
	/*write this*/
	return ht->tableSize;
}

/*
 returns the number of empty buckets in the table, these are buckets which have
 no hashlinks hanging off of them.
 */
int emptyBuckets(struct hashMap *ht)
{  
	/*write this*/

	int total = 0;
	
	for (int x = 0; x < ht->tableSize; x++)
	{
		if (ht->table[x] == NULL)
		{
			total += 1;
		}
	}
	return total;
}

/*
 returns the ratio of: (number of hashlinks) / (number of buckets)
 
 this value can range anywhere from zero (an empty table) to more then 1, which
 would mean that there are more hashlinks then buckets (but remember hashlinks
 are like linked list nodes so they can hang from each other)
 */
float tableLoad(struct hashMap *ht)
{  
	/*write this*/

	float value = (float)(ht->count) / (ht->tableSize);
	return value;
}
void printMap (struct hashMap * ht)
{
	int i;
	struct hashLink *temp;	
	for(i = 0;i < capacity(ht); i++){
		temp = ht->table[i];
		if(temp != 0) {		
			printf("\nBucket Index %d -> ", i);		
		}
		while(temp != 0){			
			printf("Key:%s|", temp->key);
			printValue(temp->value);
			printf(" -> ");
			temp=temp->next;			
		}		
	}
}


