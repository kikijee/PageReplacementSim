#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "pagetable.h"

// Check if an integer is power of 2
bool isPowerOfTwo(unsigned int x)
{
	/* First x in the below expression is for the case when x is 0 */
	return x && (!(x & (x - 1)));
}

int main(int argc, char* argv[]) {
	if (argc < 3) {
		// user does not enter enough parameters
		std::cout << "You have entered too few parameters to run the program.  You must enter" << std::endl
			<< "two command-line arguments:" << std::endl
			<< " - page size (in bytes): between 256 and 8192, inclusive" << std::endl
			<< " - physical memory size (in megabytes): between 4 and 64, inclusive" << std::endl;
		exit(1);
	}
	
	// Page size and Physical memory size 
	// Their values should be read from command-line arguments, and always a power of 2
	unsigned int page_size = atoi(argv[1]);
	if(!isPowerOfTwo(page_size))
	{
		std::cout << "You have entered an invalid parameter for page size (bytes)" << std::endl
			<< "  (must be an power of 2 between 256 and 8192, inclusive)." << std::endl;
		return 1;
	}
	unsigned int phys_mem_size = atoi(argv[2]) << 20; // convert from MB to bytes
	if(!isPowerOfTwo(phys_mem_size))
	{
		std::cout << "You have entered an invalid parameter for physical memory size (MB)" << std::endl
			<< "  (must be an even integer between 4 and 64, inclusive)." << std::endl;
		return 1;
	}

	// calculate number of pages and frames;
	int logic_mem_bits = 27;		// 27-bit logical memory (128 MB logical memory assumed by the assignment)
	int phys_mem_bits = std::log2(phys_mem_size);		// Num of bits for physical memory addresses, calculated from physical memory size, e.g. 24 bits for 16 MB memory
	int page_offset_bits = std::log2(page_size);				// Num of bits for page offset, calculated from page size, e.g. 12 bits for 4096 byte page
	// Number of pages in logical memory = 2^(logic_mem_bits - page_bit)
	int num_pages = 1 << (logic_mem_bits - page_offset_bits);
	// Number of free frames in physical memory = 2^(phys_mem_bits - page_offset_bits)
	int num_frames = 1 << (phys_mem_bits - page_offset_bits);
	
	std::cout << "Page size = " << page_size << " bytes" << std::endl;
	std::cout << "Physical Memory size = " << phys_mem_size << " bytes" << std::endl;
	std::cout << "Number of pages = " << num_pages << std::endl; 
	std::cout << "Number of physical frames = " << num_frames << std::endl;

	// Test 1: Read and simulate the small list of logical addresses from the input file "small_refs.txt"
	std::cout <<"\n================================Test 1==================================================\n";
	// TODO: Add your code here for test 1 that prints out logical page #, frame # and whether page fault for each logical address
	
	PageTable* test; //PageTable pointer for class instances
	test = new PageTable(num_pages,num_frames); //creating new class instance for test 1
	int frame;			// var to hold current frame number of page
	bool fault;			// var to hold status of page faults
	int page;			// var to hold page address
	int address;		// var to hold address from text files
	int num_ref = 0;		// var for counter of references
	int num_faults = 0;		// var for counter of faults
	int num_replace = 0;	// var for counter of replacements 
	int frames_used = 0;	// var for counter of frames 
	std::fstream fin("small_refs.txt");

	while(fin >> address){ 
		num_ref++;	// increase reference counter
		page = address/page_size;
		if(test->check_frame_exist(page,frame)){	// if page exists in primary mem already no fault
			fault = 0;
		}
		else{										// else increase fault and frames counter
			frames_used++;
			num_faults++;
			fault = 1;
		}
		if(frames_used > num_frames){num_replace++;}	// no replacement algorithm just increment counter
		std::cout<<"Logical address: "<<address<<",\tpage number: "<<page<<",\tframe number = "<<frame<<",\tis page fault? "<<fault<<std::endl;
	}
	fin.close();
	delete test;
	
	// print results
	std::cout<<"Number of references:\t\t"<<num_ref<<std::endl;
	std::cout<<"Number of page faults:\t\t"<<num_faults<<std::endl;
	std::cout<<"Number of page replacements:\t"<<num_replace<<std::endl;

	// Test 2: Read and simulate the large list of logical addresses from the input file "large_refs.txt"
	std::cout <<"\n================================Test 2==================================================\n";
	std::cout << "****************Simulate FIFO replacement****************************" << std::endl;
	// TODO: Add your code to calculate number of page faults using FIFO replacement algorithm	
	// TODO: print the statistics and run-time

	// code for timing 
	struct timespec start, end;
	start = (struct timespec){0};
	end = (struct timespec){0};
	clock_gettime(CLOCK_MONOTONIC, &start);

	num_ref = 0;
	num_faults = 0;
	num_replace = 0;
	int oldest = 0;		// var for keeping track of the oldest frame FIFO
	frames_used = 0;
	LinkedList* queue = new LinkedList;	// queue class instance through linked list for FIFO
	test = new PageTable(num_pages,num_frames);	// new PageTable instance 

	std::fstream fin2("large_refs.txt");
	while(fin2 >> address){
		num_ref++;	// increase reference counter
		page = address/page_size;
		if(test->check_frame_exist(page,frame)){ //if page is already in primary memory
			fault = 0;
		}
		else{ //if page is not in primary memory
			queue->add_tail(page); //add page index into the queue for FIFO purposes
			if(frames_used >= num_frames){ //if primary memory is full
				page = queue->remove_head(); //removing the oldest page index from queue
				test->remove_frame(page);
				num_replace++;
				test->set_frame(oldest);
				oldest = (oldest+1)%num_frames+1;
			}
			else{
				frames_used++;
			}
			num_faults++;
		}
	}
	fin2.close();
	delete queue;
	delete test;

	clock_gettime(CLOCK_MONOTONIC, &end);
	double time_taken = (end.tv_sec + end.tv_nsec*1e-9) - (start.tv_sec + start.tv_nsec*1e-9);
	std::cout<<"Number of references:\t\t"<<num_ref<<std::endl;
	std::cout<<"Number of page faults:\t\t"<<num_faults<<std::endl;
	std::cout<<"Number of page replacements:\t"<<num_replace<<std::endl;
	std::cout<<"Elapsed time = "<<time_taken<<" seconds"<<std::endl;

	std::cout << "****************Simulate Random replacement****************************" << std::endl;
	// TODO: Add your code to calculate number of page faults using Random replacement algorithm
	// TODO: print the statistics and run-time
	start = (struct timespec){0};
	end = (struct timespec){0};
	clock_gettime(CLOCK_MONOTONIC, &start);
	num_ref = 0;
	num_faults = 0;
	num_replace = 0;
	frames_used = 0;
	int index;
	int *arr = new int[num_frames];
	test = new PageTable(num_pages,num_frames);
	std::fstream fin3("large_refs.txt");
	srand(time(NULL));
	while(fin3 >> address){
		num_ref++;
		page = address/page_size;
		if(test->check_frame_exist(page,frame)){ //if page is already in primary memory
			fault = 0;
		}
		else{ //if page is not in primary memory
			if(frames_used >= num_frames){ //if primary memory is full
				index = rand() % frames_used;
				test->remove_frame(arr[index]); 
				arr[index] = page;
				num_replace++;
			}
			else{
				arr[frames_used] = page;
				frames_used++;
			}
			num_faults++;
		}
	}
	fin3.close();
	delete [] arr;
	delete test;
	clock_gettime(CLOCK_MONOTONIC, &end);
	time_taken = (end.tv_sec + end.tv_nsec*1e-9) - (start.tv_sec + start.tv_nsec*1e-9);
	std::cout<<"Number of references:\t\t"<<num_ref<<std::endl;
	std::cout<<"Number of page faults:\t\t"<<num_faults<<std::endl;
	std::cout<<"Number of page replacements:\t"<<num_replace<<std::endl;
	std::cout<<"Elapsed time = "<<time_taken<<" seconds"<<std::endl;
	std::cout << "****************Simulate LRU replacement****************************" << std::endl;
	// TODO: Add your code to calculate number of page faults using LRU replacement algorithm
	// TODO: print the statistics and run-time
	start = (struct timespec){0};
	end = (struct timespec){0};
	clock_gettime(CLOCK_MONOTONIC, &start);
	num_ref = 0;
	num_faults = 0;
	num_replace = 0;
	frames_used = 0;
	int old_page;
	Node* node;
	LinkedList* stack = new LinkedList;
	test = new PageTable(num_pages,num_frames);

	std::fstream fin4("large_refs.txt");
	while(fin4 >> address){
		num_ref++;
		page = address/page_size;
		if(test->check_frame_exist(page,frame)){ //if page is already in primary memory
			node = test->check_stack(page);			// function returns node address if page was already in the queue
			stack->remove_node(node);				// removes node from location in the stack
			node = stack->add_head(page);			// adds page to the head of the queue
			test->set_node_location(page,node);		// stores page node location in PageTable class instance
		}
		else{	//if page is not already in the primary memory
			if(frames_used >= num_frames){ //if primary memory is full
				old_page = stack->remove_tail();		// removes least used page via stack
				test->set_node_location(old_page);	// sets page node to NULL
				test->remove_frame(old_page);		// removes frame status of page
				// checks for new page in the stack
				node = test->check_stack(page);		
				stack->remove_node(node);
				node = stack->add_head(page);
				test->set_node_location(page,node);
				num_replace++;
			}
			else{
				// checks for new page in the stack
				node = test->check_stack(page);
				stack->remove_node(node);
				node = stack->add_head(page);
				test->set_node_location(page,node);
				frames_used++;
			}
			num_faults++;
		}
	}
	fin4.close();
	delete stack;
	delete test;
	clock_gettime(CLOCK_MONOTONIC, &end);
	time_taken = (end.tv_sec + end.tv_nsec*1e-9) - (start.tv_sec + start.tv_nsec*1e-9);
	std::cout<<"Number of references:\t\t"<<num_ref<<std::endl;
	std::cout<<"Number of page faults:\t\t"<<num_faults<<std::endl;
	std::cout<<"Number of page replacements:\t"<<num_replace<<std::endl;
	std::cout<<"Elapsed time = "<<time_taken<<" seconds"<<std::endl;
}
