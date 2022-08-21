#pragma once
#include <cstddef>

struct Node{
	int page_num;
	Node* next;
	Node* prev;
};
// A page table entry
class PageEntry
{
public:
	// location for stack
	Node* location = NULL;
	// Physical frame number for a given page
	int frame_num = -1;
	// valid bit represents whether a page is in the physical memory
	bool valid = false;
	// dirty bit represents whether a page is changed
	// bool dirty = false;
	PageEntry(){}
};


class PageTable
{
	// TODO: Add your implementation of the page table here
	private:
		int num_page_;
		int num_frame_;
		int current_frame_ = 0;
		PageEntry* arr_;
	public:
		PageTable(int num_page, int num_frame):num_page_(num_page),num_frame_(num_frame){
			arr_ = new PageEntry[num_page_];
		}
		~PageTable(){delete [] arr_;}
		void set_frame(int num){current_frame_ = num;}
		//checks if page address given through parameters exists already in arr_
		bool check_frame_exist(int,int &);
		//removes from primary memory
		void remove_frame(int);
		//displays given index 
		void display_page(int);
		// checks if page is in the stack
		Node* check_stack(int);
		// setter of node location in arr_
		void set_node_location(int,Node* = NULL);
		//destructor
};


class LinkedList{
	private:
		Node* head_;
		Node* tail_;
	public:
		LinkedList():head_(NULL),tail_(NULL){}
		void add_tail(int num){
			Node* node = new Node;
			node->page_num = num;
			node->next = NULL;
			node->prev = NULL;
			if(head_ == NULL){
				head_ = node;
				tail_ = node;
			}
			else{
				tail_->next = node;
				node->prev = tail_;
				tail_ = node;
			}
		}
		Node* add_head(int num){
			Node* node = new Node;
			node->page_num = num;
			node->next = NULL;
			node->prev = NULL;
			if(head_ == NULL){
				head_ = node;
				tail_ = node;
			}
			else{
				node->next = head_;
				head_->prev = node;
				head_ = node;
			}
			return node;
		}
		int remove_head(){
			if(head_ == NULL){return 0;}
			else if(head_->next == NULL){
				int num = head_->page_num;
				delete head_;
				head_ = NULL;
				tail_ = NULL;
				return num;
			}
			else{
				int num = head_->page_num;
				Node* p = head_;
				head_ = head_->next;
				delete p;
				head_->prev = NULL;
				return num;
			}
		}
		int remove_tail(){
			int num;
			if(head_ == NULL){return 0;}
			else if(tail_ == head_){
				num = head_->page_num;
				delete head_;
				head_ = NULL;
				tail_ = NULL;
			}
			else{
				num = tail_->page_num;
				Node* p = tail_;
				tail_ = tail_->prev;
				delete p;
				tail_->next = NULL;
			}
			return num;
		}
		void remove_node(Node* node){
			if(node == NULL){return;}
			else{
				if(node == head_){remove_head();}
				else if(node == tail_){remove_tail();}
				else{
					node->prev->next = node->next;
					node->next->prev = node->prev;
					delete node;
				}
			}
		}
		~LinkedList(){
			while(head_ != NULL){remove_head();}
		}
};