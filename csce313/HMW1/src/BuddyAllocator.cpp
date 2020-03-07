#include "BuddyAllocator.h"
#include <iostream>
using namespace std;

 unsigned next_pow2(unsigned x)
{
	/*
	* provided at locklessinc.com/articles/next_pow2/
	* optimal function to compute next power of 2
	* 
	*/
	x -= 1;
	x |= (x >> 1);
	x |= (x >> 2);
	x |= (x >> 4);
	x |= (x >> 8);
	x |= (x >> 16);
	return x + 1;
}
BuddyAllocator::BuddyAllocator (int _basic_block_size, int _total_memory_length){
	this->basic_block_size = next_pow2(_basic_block_size);
	this->total_memory_size = next_pow2(_total_memory_length);
	
	int free_list_size = int(log2(this->total_memory_size/this->basic_block_size)) + 1;
	for(uint  i = 0; i < free_list_size; i++)
	{
		FreeList.push_back(LinkedList());
	}
	
	BlockHeader* head = (BlockHeader*)(new BlockHeader[int(this->total_memory_size)]);		
	head->isAvailable = true;
	head->block_size = this->total_memory_size;
	head->next = NULL;
	FreeList.at(free_list_size - 1).insert(head);		

	root = head;	
}

BuddyAllocator::~BuddyAllocator (){
	FreeList.clear();
}

void* BuddyAllocator::alloc(int length) {
	uint neededSize = length + sizeof(BlockHeader);
	uint neededBlockSize = next_pow2(neededSize);
	if (neededSize > this->total_memory_size) return NULL;
	if(neededBlockSize < this->basic_block_size) neededBlockSize = this->basic_block_size;
	
	
	uint indexOfNeededBlock = log2(neededBlockSize/this->basic_block_size);
	if(indexOfNeededBlock > FreeList.size() - 1) indexOfNeededBlock = FreeList.size() - 1;

	if(FreeList.at(indexOfNeededBlock).sz > 0)
	{
		BlockHeader* block = FreeList.at(indexOfNeededBlock).head;
		block->isAvailable = false;
		block->next = NULL;
		FreeList.at(indexOfNeededBlock).remove(block);
		return  (void*)(block + sizeof(BlockHeader));
	}
	else
	{
		while(FreeList.at(indexOfNeededBlock).sz == 0 )
		{
			indexOfNeededBlock++;
			if (indexOfNeededBlock >= FreeList.size()) return NULL;
		}
	}

	int currBlockSize = FreeList.at(indexOfNeededBlock).head->block_size;	
	//seg faults love me -- next line useful
	//cout <<"Initial indexOfNeededBlock::" << indexOfNeededBlock<<"::Current Block Size::"<<currBlockSize<<"::Needed Block Size::"<<neededBlockSize << endl;
	BlockHeader* partition;
	while(currBlockSize != neededBlockSize)
	{
		BlockHeader* block = (BlockHeader*)FreeList.at(indexOfNeededBlock).head;
		partition = this->split(block);
		block = partition;
		indexOfNeededBlock--;
		currBlockSize /= 2;
	}	
	FreeList.at(indexOfNeededBlock).remove(partition);
	return (void*)(partition + sizeof(BlockHeader));
}

void BuddyAllocator::free(void* a) {
	BlockHeader* block = (BlockHeader*)((BlockHeader*)a - sizeof(BlockHeader));
	uint index = log2(block->block_size/this->basic_block_size);
	FreeList.at(index).insert(block);
	
	while(true)
	{
		BlockHeader * buddy = getbuddy(block);
		
		if(arebuddies(block,buddy))
		{
			block = merge(block,buddy);
		}
		else break;	
	}

}
BlockHeader* BuddyAllocator::getbuddy(BlockHeader* addr)
{	
	return root + ((addr - root) ^ addr->block_size);
}

bool BuddyAllocator::arebuddies(BlockHeader* block1, BlockHeader* block2)
{
	return (block1 -> block_size == block2 -> block_size) && block2->isAvailable;
}

BlockHeader* BuddyAllocator::merge(BlockHeader* block1, BlockHeader* block2)
{
	BlockHeader* first;
	BlockHeader* second;

	if(block1 < block2)
	{
		first = block1;
		second = block2;
	}
	else
	{
		first = block2;
		second = block1;
	}
	uint index = log2(first->block_size/this->basic_block_size);
	FreeList.at(index).remove(first);
	FreeList.at(index).remove(second);

	uint mergedSize = first->block_size * 2;
	uint mergedIndex = log2(mergedSize / this->basic_block_size);
	FreeList.at(mergedIndex).insert(first);
	first->block_size = mergedSize;
	return first;	
}
BlockHeader* BuddyAllocator::split(BlockHeader* block)
{
	if (block == NULL)return NULL;
	uint blockSize = block->block_size;
	uint halfBlock = blockSize/2;

	BlockHeader* leftHalf = block;
	BlockHeader* rightHalf = block + halfBlock;
	
	FreeList.at(log2(blockSize/this->basic_block_size)).remove(leftHalf);
	
	leftHalf->block_size = halfBlock;
	rightHalf->block_size = halfBlock;
	//seg fault love me -- next line useful
	//cout<<"split(BlockHeader * block)::"<< block << "::block size::"<<blockSize<< "::splitBlockLeft::"<<leftHalf<<"::splitBlockLeftSize::"<<leftHalf->block_size<<endl;
	

	uint indexToInsert = log2( halfBlock/this->basic_block_size);
	FreeList.at(indexToInsert).insert(rightHalf);
	FreeList.at(indexToInsert).insert(leftHalf);

	return leftHalf;
		

}
void BuddyAllocator::printlist (){
  cout << "Printing the Freelist in the format \"[index] (block size) : # of blocks\"" << endl;
  int64_t total_free_memory = 0;
  for (int i=0; i<FreeList.size(); i++){
    int blocksize = ((1<<i) * this->basic_block_size); // all blocks at this level are this size
    cout << "[" << i <<"] (" << blocksize << ") : ";  // block size at index should always be 2^i * bbs
    int count = 0;
    BlockHeader* b = FreeList [i].head;
    // go through the list from head to tail and count
    while (b){
      total_free_memory += blocksize;
      count ++;
      // block size at index should always be 2^i * bbs
      // checking to make sure that the block is not out of place
      if (b->block_size != blocksize){
        cerr << "ERROR:: Block is in a wrong list" << endl;
        exit (-1);
      }
      b = b->next;
    }
    cout << count << endl;
    cout << "Amount of available free memory: " << total_free_memory << " byes" << endl;  
  }
}

