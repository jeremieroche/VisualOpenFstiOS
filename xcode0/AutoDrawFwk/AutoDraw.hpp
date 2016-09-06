//
//  AutoDraw.hpp
//  AutoDrawFwk
//
//  Created by Jeremie Roche on 7/2/16.
//  Copyright © 2016 jeremieroche. All rights reserved.
//

#ifndef AutoDraw_hpp
#define AutoDraw_hpp

#include <stdio.h>
#include "AutoState.hpp"
#include "AutoTrans.hpp"
//#include "Grid.hpp"

#include "fst/fstlib.h"


enum MoveType{
    None, State, ControlPoint1,ControlPoint2,Label
};


class AutoDraw{
    string name;
    
    
public:
    std::vector<AutoState *> arrayState;

    
    fst::StdVectorFst *mainFST;
    
    MoveType mType;
    int moveStateID;
    int moveTransID;

    
    AutoDraw();
    
    
    
    std::tuple<int,int,int> retrieveStateTransID(float x, float y);
    
    // State
    AutoState * addState(float x, float y);
    void setFinalState(AutoState * state, float weight);
    int numberStates();
    std::vector<AutoState *> getStates();
    
    bool isFinalState();
    bool setFinal(float fWeight);
    bool getStateIdFinal(int stateIndex);
    float getStateIdFinalWeight(int stateIndex);
    
    int retrieveStateID(float xCoor, float yCoor);

    
    
    // Transitions
    AutoTrans* addTransition(int sourceStateId, int destStateId,int iLabel,int oLabel, float weight);
    int numberTrans();
    int getTransDestIndex(int stateIndex, int transIndex);
    
    void toggle_all_cp();
    std::__1::string getTransLabel(int stateIndex, int transIndex);
    
    float getTransQuality(int stateIndex, int transIndex, bool controlPoint, bool xValue,bool first);
    float getTransCP1X(int stateIndex,int transIndex);
    float getTransCP1Y(int stateIndex,int transIndex);
    float getTransCP2X(int stateIndex,int transIndex);
    float getTransCP2Y(int stateIndex,int transIndex);
    float getInInterX(int stateIndex, int transIndex);
    float getInInterY(int stateIndex, int transIndex);
    float getOutInterX(int stateIndex,int transIndex);
    float getOutInterY(int stateIndex,int transIndex);
    float getTransLabelX(int stateIndex, int transIndex);
    float getTransLabelY(int stateIndex, int transIndex);
    
    
    
    // Moving
    bool moveStateTo(int stateID, float x, float y);
    bool moveAction(float x, float y);
    int getMoveType();
    int getMoveStateID();
    int getMoveTransID();
    
    AutoDraw * getMinimizedVersion();
    
    // Beautify
    void beautify();
    
    
    
    

};


fst::StdVectorFst * duplicateOFST(fst::StdVectorFst * orignialOFST);

AutoDraw* returnTestAuto();


#endif /* AutoDraw_hpp */
