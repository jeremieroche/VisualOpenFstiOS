//
//  AutoBridge.hpp
//  AutoDrawS0
//
//  Created by Jeremie Roche on 7/7/16.
//  Copyright © 2016 jeremieroche. All rights reserved.
//

#ifndef AutoBridge_hpp
#define AutoBridge_hpp

#include <stdio.h>


typedef struct _aut *AUT;


#ifdef __cplusplus
extern "C" {
#endif

    AUT createSampleAut(void);
    
    void addState(AUT aut,float x, float y);
    int nbStates(AUT aut);
    int getStateId(AUT aut, float xCoor, float yCoor);
    float getStatePosX(AUT aut, int stateIndex);
    float getStatePosY(AUT aut, int stateIndex);
    bool setFinal(AUT aut, float fWeight);
    bool isFinalState(AUT aut);
    bool isFinal(AUT aut, int stateIndex);
    float getFinalWeight(AUT aut, int stateIndex);
    


    
    
    
    void addTransition(AUT aut, int stateIdSource, int stateIdDestination, int iLabel,int oLabel,float weight);
    int nbTrans(AUT aut);
    bool hasTrans(AUT aut, int stateIndex, int transIndex);
    int getTransDestIndex(AUT aut, int stateIndex, int transIndex);
    char * getTransLabel(AUT aut, int stateIndex, int transIndex);

    float getTransCP1X(AUT aut,int stateIndex,int transIndex);
    float getTransCP1Y(AUT aut,int stateIndex,int transIndex);
    float getTransCP2X(AUT aut,int stateIndex,int transIndex);
    float getTransCP2Y(AUT aut,int stateIndex,int transIndex);
    float getTransInInterX(AUT aut,int stateIndex,int transIndex);
    float getTransInInterY(AUT aut,int stateIndex,int transIndex);
    float getTransOutInterX(AUT aut,int stateIndex,int transIndex);
    float getTransOutInterY(AUT aut,int stateIndex,int transIndex);
    float getTransLabelX(AUT aut, int stateIndex, int transIndex);
    float getTransLabelY(AUT aut, int stateIndex, int transIndex);
    void toggle_all_cp(AUT aut);
    
    
    
    
    bool move_action(AUT aut, float xCoor, float yCoor);
    int getMoveType(AUT aut);
    int getMoveStateID(AUT aut);
    int getMoveTransID(AUT aut);
        
    
    AUT getTestAuto();
    
    
    AUT getMinimizedAuto(AUT aut);
    
    void beautify(AUT aut);
        
    
    
    

    
#ifdef __cplusplus
}
#endif

    
#endif /* AutoBridge_hpp */
