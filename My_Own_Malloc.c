#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#define HEAP_CAPACITY  640000
#define CHUNK_LIST_CAP 1024
#define HEAP_DFREED_CAP 1024

#define UNIMPLEMENTED \
do{\
	fprintf(stderr,"%s:%zu: TODO:%s is not implemented!.\n"\
		,__FILE__,__LINE__,__func__);\
	abort();\
}while(0);

typedef struct{
	void* start;
	size_t size;
}Heap_Chunk;

typedef struct{
	Heap_Chunk chunk_array[CHUNK_LIST_CAP];
	size_t count;
}Chunk_List;



char heap[HEAP_CAPACITY] = {0};
size_t heap_size = 0;

Chunk_List alloced_chunk = {0};
Chunk_List freed_chunk = {
	.count = 1,
	.chunk_array[0].start = heap,
	.chunk_array[0].size = sizeof(heap),
};

void* HeapMalloc(size_t size);
void  HeapFree(void* ptr);
void  HeapCollect(void);
void  ChunkListDump(const Chunk_List *list);
int   ChunkListFind(Chunk_List *chunk , void* ptr);
void  ChunkListRemove(Chunk_List * chunk,size_t index);
void  ChunkListInsert(Chunk_List *chunk,void* ptr, size_t size);

int main(int argc, char **argv)
{
	/*
	 char *root = HeapMalloc(26);
	for(int i=0 ; i< 26 ; i++)
	{
		root[i] = i+ 'A';
	}
	*/
	
	for(int i=0 ; i<10 ; i++)
	{
		void* p = HeapMalloc(i);
		if(i %2 == 0)
		{
			HeapFree(p);
		}
	}

	for(int i=0 ; i<10 ; i+=2)
	{
		void* p = HeapMalloc(i);
	}

	ChunkListDump(&alloced_chunk);
	ChunkListDump(&freed_chunk);

	//HeapFree(root);

	return(0);
}

void* HeapMalloc(size_t size)
{
	if(size > 0){
	for(size_t i = 0 ; i <freed_chunk.count ; i++)
	{
		if(freed_chunk.chunk_array[i].size >= size)
		{
			Heap_Chunk chunk = freed_chunk.chunk_array[i];
			ChunkListRemove(&freed_chunk,i);
			size_t tail_size = chunk.size - size;

			ChunkListInsert(&alloced_chunk,chunk.start,size);
			if(tail_size > 0)
			{
				ChunkListInsert(&freed_chunk,chunk.start + size, tail_size);
			}

			return(chunk.start);
		}
	}
	}
	return(NULL);
}

void HeapFree(void* ptr)
{
	if(ptr != NULL)
	{
		const int index = ChunkListFind(&alloced_chunk,ptr);
		printf("%d\n",index);
		assert(index >= 0);

		ChunkListInsert(&freed_chunk,
		alloced_chunk.chunk_array[index].start,
		alloced_chunk.chunk_array[index].size);

		ChunkListRemove(&alloced_chunk, index);
	}
	else
	{
		return;
	}
	

	
}

void HeapCollect(void)
{
	UNIMPLEMENTED;
}

void ChunkListDump(const Chunk_List *list)
{
	printf("Alloceted Chunks(%zu): \n",list->count);
	for(int i=0 ; i< list->count ; i++)
	{
		printf("The Start address of Chunk: %p,The size of Chunk: %li.\n",\
				list ->chunk_array[i].start,\
				list->chunk_array[i].size);
	}
}

int ChunkListFind(Chunk_List *chunk , void* ptr)
{
	for(int i = chunk->count-1 ; i>= 0 ; i--)
	{
		if(chunk->chunk_array[i].start == ptr)
		{
			return(i);
		}
	}
	return(-1);
}

void ChunkListRemove(Chunk_List * chunk,size_t index)
{
	assert(index <= chunk->count);
	for(size_t i = index ; i< chunk->count -1 ; i++)
	{
		chunk->chunk_array[i]= chunk->chunk_array[i+1];
	}
	chunk->count--;
}

void ChunkListInsert(Chunk_List *chunk,void* ptr, size_t size)
{
	Heap_Chunk temp;
	assert(chunk->count <CHUNK_LIST_CAP);
	chunk->chunk_array[chunk->count].start = ptr;
	chunk->chunk_array[chunk->count].size  = size;

	for(int i=chunk->count ;
		i>0 && (chunk->chunk_array[i].start < chunk->chunk_array[i-1].start) ;
		i--)
	{
		temp  = chunk->chunk_array[i];
		chunk->chunk_array[i]  = chunk->chunk_array[i-1];
		chunk->chunk_array[i-1] = temp;
	}

	chunk-> count +=1;
}

