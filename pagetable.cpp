#include "pagetable.h"

bool PageTable::check_frame_exist(int i,int &frame){
    if(arr_[i].frame_num == -1){
        arr_[i].frame_num = current_frame_;
        frame = current_frame_;
        current_frame_++;
        arr_[i].valid = true;
        return false;
    }
    frame = arr_[i].frame_num;
    return true;
}
void PageTable::remove_frame(int page){
    arr_[page].frame_num = -1;
    arr_[page].valid = false;
}
Node* PageTable::check_stack(int page){
    return arr_[page].location;
}
void PageTable::set_node_location(int page,Node* node){
    arr_[page].location = node;
}
