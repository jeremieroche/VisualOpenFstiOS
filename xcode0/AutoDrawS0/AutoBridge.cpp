//
//  AutoBridge.cpp
//  AutoDrawS0
//
//  Created by Jeremie Roche on 7/7/16.
//  Copyright © 2016 jeremieroche. All rights reserved.
//

#include "AutoBridge.hpp"

#include "AutoDrawFwk/AutoDraw.hpp"


typedef struct _aut{
    AutoDraw *autoDraw;
    
    char *buff;
    int size_buff;
} *AUT;

AutoState* getState(AUT aut, int stateIndex);


// ######## BRIDGE FUNCTIONS (can be called from Swift)


AUT createSampleAut(void){
    
    AUT aut =(AUT)malloc(sizeof(struct _aut));
    memset(aut,0,sizeof(struct _aut));
    
    AutoDraw *autoDraw=new AutoDraw();
    
    aut->autoDraw=autoDraw;
    
    aut->buff=(char *)malloc(sizeof(char)*1000);
    aut->size_buff=1000;
    
    return aut;
    
}

void addState(AUT aut,float x, float y){
    aut->autoDraw->addState(x, y);
    
}

int nbStates(AUT aut){
    return aut->autoDraw->numberStates();
}

int nbTrans(AUT aut){
    return aut->autoDraw->numberTrans();
}

// puts into p_x and p_y the position of state <stateIndex>. Return 1 if successful and 0 otherwise (for instance if stateIndex>=nbStates)
float getStatePosX(AUT aut, int stateIndex){
    
    if (!aut)
        return 0.0;
    
    AutoState* theState = getState(aut, stateIndex);
    
    if (!theState)
        return 0.0;
    
    return theState->x;
}

float getStatePosY(AUT aut, int stateIndex){
    
    if (!aut)
        return 0.0;
    
    AutoState* theState = getState(aut, stateIndex);
    
    if (!theState)
        return 0.0;
    
    return theState->y;
}

void addTransition(AUT aut, int stateIdSource, int stateIdDestination, int iLabel,int oLabel,float weight){
    aut->autoDraw->addTransition(stateIdSource, stateIdDestination, iLabel,oLabel,weight);
}

int getStateId(AUT aut, float xCoor, float yCoor){
    
    return aut->autoDraw->retrieveStateID(xCoor, yCoor);
    
    
}


std::vector<int> getTransIndecies(AUT aut, int stateIndex){
    AutoState *state = aut->autoDraw->getStates()[stateIndex];
    
    std::vector<AutoTrans *> outTrans = state->outTrans;
    
    std::vector<int> indecies = {};
    for (AutoTrans*  trans : outTrans){
        indecies.push_back(trans->id);
    }
    
    return indecies;
    
}

bool hasTrans(AUT aut, int stateIndex, int transIndex){
    return aut->autoDraw->arrayState[stateIndex]->hasTransIndex(transIndex);
}

float getTransCP1X(AUT aut,int stateIndex,int transIndex){
    return aut->autoDraw->getTransCP1X(stateIndex, transIndex);
}

float getTransCP1Y(AUT aut, int stateIndex, int transIndex){
    return aut->autoDraw->getTransCP1Y(stateIndex, transIndex);
}

float getTransCP2X(AUT aut,int stateIndex,int transIndex){
    return aut->autoDraw->getTransCP2X(stateIndex, transIndex);
}

float getTransCP2Y(AUT aut, int stateIndex, int transIndex){
    return aut->autoDraw->getTransCP2Y(stateIndex, transIndex);
}

float getTransInInterX(AUT aut,int stateIndex,int transIndex){
    return aut->autoDraw->getInInterX(stateIndex, transIndex);
}
float getTransInInterY(AUT aut,int stateIndex,int transIndex){
    return aut->autoDraw->getInInterY(stateIndex, transIndex);
}

float getTransOutInterX(AUT aut,int stateIndex,int transIndex){
    return aut->autoDraw->getOutInterX(stateIndex, transIndex);
}
float getTransOutInterY(AUT aut,int stateIndex,int transIndex){
    return aut->autoDraw->getOutInterY(stateIndex, transIndex);
}

int getTransDestIndex(AUT aut, int stateIndex, int transIndex){
    return aut->autoDraw->getTransDestIndex(stateIndex, transIndex);
}

float getTransLabelX(AUT aut, int stateIndex, int transIndex){
    return aut->autoDraw->getTransLabelX(stateIndex, transIndex);
}

float getTransLabelY(AUT aut, int stateIndex, int transIndex){
    return aut->autoDraw->getTransLabelY(stateIndex, transIndex);
}

bool setFinal(AUT aut, float fWeight){
    return aut->autoDraw->setFinal(fWeight);
}

bool isFinalState(AUT aut){
    return aut->autoDraw->isFinalState();
}

bool isFinal(AUT aut, int stateIndex){
    return aut->autoDraw->getStateIdFinal(stateIndex);
}

float getFinalWeight(AUT aut, int stateIndex){
    return aut->autoDraw->getStateIdFinalWeight(stateIndex);
}

char * getTransLabel(AUT aut, int stateIndex, int transIndex){
    std::string label = aut->autoDraw->getTransLabel(stateIndex, transIndex);
    
    char * c_label = (char *)label.c_str();
    
    if (!c_label || !c_label[0])
        return NULL;
    
    if (strlen(c_label)>=aut->size_buff){
        if (aut->buff)
            free(aut->buff);
        aut->buff=(char *)malloc(sizeof(char)*(strlen(c_label)+1));
        aut->size_buff=(int)(strlen(c_label)+1);
    }
    
    strcpy(aut->buff,c_label);
    
    return  aut->buff;
}

void toggle_all_cp(AUT aut){
    aut->autoDraw->toggle_all_cp();
}

bool move_action(AUT aut, float xCoor, float yCoor){
    return aut->autoDraw->moveAction(xCoor, yCoor);
}


int getMoveType(AUT aut){
    return aut->autoDraw->getMoveType();
}

int getMoveStateID(AUT aut){
    return aut->autoDraw->getMoveStateID();
}

int getMoveTransID(AUT aut){
    return aut->autoDraw->getMoveTransID();
}




AUT getTestAuto(){
    AutoDraw * autoDraw = returnTestAuto();
    
    AUT aut =(AUT)malloc(sizeof(struct _aut));
    memset(aut,0,sizeof(struct _aut));
    
    aut->autoDraw=autoDraw;
    
    aut->buff=(char *)malloc(sizeof(char)*1000);
    aut->size_buff=1000;
    
    return aut;

}



AUT getMinimizedAuto(AUT aut){
    AutoDraw* newDraw = aut->autoDraw->getMinimizedVersion();
    
    AUT newAut =(AUT)malloc(sizeof(struct _aut));
    memset(newAut,0,sizeof(struct _aut));
    
    newAut->autoDraw=newDraw;
    
    newAut->buff=(char *)malloc(sizeof(char)*1000);
    newAut->size_buff=1000;
    
    return newAut;
}


void beautify(AUT aut){
    aut->autoDraw->beautify();
}


// ######## END OF BRIDGE FUNCTIONS (can be called from Swift)

AutoState* getState(AUT aut, int stateIndex){
    
    std::vector<AutoState *> stateArray = aut->autoDraw->getStates();
        
    AutoState* theState = stateArray[stateIndex];
    
    return theState;
    
}









