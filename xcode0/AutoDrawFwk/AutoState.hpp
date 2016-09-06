//
//  AutoState.hpp
//  AutoDrawFwk
//
//  Created by Jeremie Roche on 7/2/16.
//  Copyright © 2016 jeremieroche. All rights reserved.
//

#ifndef AutoState_hpp
#define AutoState_hpp

#include <stdio.h>
#include "fst/fst.h"

class AutoTrans;

class AutoState{
    
public:
    // Variables
    int id;
    float finalWeight;
    
    float x;
    float y;
    
    
    
    std::vector<AutoTrans *> outTrans;
    std::map<int,int> end_state_count;
    
    
    // Initializers
    AutoState();
    AutoState(int id, float x, float y);
    
    
    
    
    bool is_first(void);
    bool isFinal();
    void set_as_final(float fWeight);
    
    
    
    
    // Transitions
    void addTransition(AutoTrans *trans);
    int numberTrans();
    bool hasTransIndex(int id);
    bool has_dest_state(AutoState * otherState);
    void update_end_state_count(AutoTrans * trans);
    std::__1::string getLabelTransID(int transID);
    
    
    bool has_state_at(int xCoor, int yCoor);
    void toggle_all_cp();
    
    
        
    
};


#endif /* AutoState_hpp */
