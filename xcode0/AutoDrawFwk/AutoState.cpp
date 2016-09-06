//
//  AutoState.cpp
//  AutoDrawFwk
//
//  Created by Jeremie Roche on 7/2/16.
//  Copyright © 2016 jeremieroche. All rights reserved.
//

#include "AutoState.hpp"
#include "AutoTrans.hpp"



#define FINAL_NULL -29472.8f
AutoState::AutoState(int id, float x, float y){
    this->id = id;
    this->finalWeight = FINAL_NULL;
    this->x = x;
    this->y = y;
    
    this->outTrans = {};
    
};


bool AutoState::isFinal(){
    bool isFinal = (this->finalWeight != FINAL_NULL);
    return isFinal;
}

void AutoState::addTransition(AutoTrans *newTrans){
    
    outTrans.push_back(newTrans);
    
    update_end_state_count(newTrans);
    
}

int AutoState::numberTrans(){
    return (int) this->outTrans.size();
}


void AutoState::update_end_state_count(AutoTrans *trans){
    int destStateID = trans->destStateID;
    
    
    this->end_state_count[destStateID]++;
    
}

bool AutoState::hasTransIndex(int id){
    
    
    
    
    if (this->outTrans.size() > id) {
        return true;
    } else {
        return false;
    }
    
    
    
}

bool AutoState::has_state_at(int xCoor, int yCoor){
    
    float radius = float(15);
    
    bool point_below_upper_x = xCoor < this->x + radius;
    bool point_above_lower_x = xCoor > this->x - radius;
    bool point_left_right_y  = yCoor < this->y + radius;
    bool point_right_left_y  = yCoor > this->y - radius;
    
    return point_below_upper_x and point_above_lower_x and point_left_right_y and point_right_left_y;
    
}

void AutoState::set_as_final(float fWeight){
    this->finalWeight = fWeight;
}

std::__1::string AutoState::getLabelTransID(int transID){
    
    AutoTrans * trans = this->outTrans[transID];
    
    return trans->getFullLabel();
    
}

void AutoState::toggle_all_cp(){
    for(AutoTrans * trans : this->outTrans){
        trans->toggle_show_cp();
    }
}


bool AutoState::has_dest_state(AutoState * otherState){
    
    if (this->end_state_count[otherState->id] > 0 || otherState->end_state_count[this->id] > 0) {
        return true;
    }
    
    return false;
}





