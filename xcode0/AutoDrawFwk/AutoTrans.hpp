//
//  AutoTrans.hpp
//  AutoDrawFwk
//
//  Created by Jeremie Roche on 7/2/16.
//  Copyright © 2016 jeremieroche. All rights reserved.
//

#ifndef AutoTrans_hpp
#define AutoTrans_hpp

#include <stdio.h>
#include "AutoState.hpp"
#include "fst/fst.h"
#include <iomanip>      // std::setprecision




class AutoTrans{
public:
    
    // Variables
    int id;
    int destStateID;
    fst::StdArc fstArc;
    
    bool show_controlPoints;
    std::tuple<float,float> controlPoint1;
    std::tuple<float,float> controlPoint2;
    
    std::tuple<float,float> inIntersection;
    std::tuple<float,float> outIntersection;
    
    std::tuple<float,float> label_position;
    
    
    
    
    AutoTrans(AutoState *inState,AutoState* outState, int inputLabel, int outputLabel, float weight);

    
    fst::StdArc getFstArc();
    std::__1::string getFullLabel();
    
    void toggle_show_cp();
    
    
    int  has_trans(float xCoor, float yCoor);
    bool has_trans_control_point(int controlPointNumber, float xCoor, float yCoor);
    bool has_trans_label(float xCoor, float yCoor);
    
    
    bool moveControlPoint(int number, float xCoor, float yCoor);
    bool moveLabelPosition(float xCoor, float yCoor);
    
    
    bool recompute_transitionPoints(AutoState * inState, AutoState* outState);
    bool recompute_controlPoints(AutoState * inState, AutoState* outState);
    bool recompute_intersection(AutoState * inState, AutoState* outState);
    bool recompute_labelPoint();
    
    
};

#endif /* AutoTrans_hpp */
