//
//  AutoDraw.cpp
//  AutoDrawFwk
//
//  Created by Jeremie Roche on 7/2/16.
//  Copyright © 2016 jeremieroche. All rights reserved.
//

#include "AutoDraw.hpp"
#include "AutoState.hpp"


AutoDraw::AutoDraw(){
    
    mainFST =new fst::StdVectorFst();
    
    this->mType = None;
    this->moveStateID = -1;
    this->moveTransID = -1;
        
    
}


#define D_INFINITY 100000000

AutoState* AutoDraw::addState(float x, float y){
    
    
    mainFST->AddState();
    
    if (this->numberStates() == 0){
        mainFST->SetStart(0);
    }
    
    AutoState *newState = new AutoState::AutoState(this->numberStates(),x,y);
    
    
    this->arrayState.push_back(newState);
    
    return newState;

    
}

void AutoDraw::setFinalState(AutoState * state, float weight){
    state->set_as_final(weight);
    this->mainFST->SetFinal(state->id, weight);
}

std::vector<AutoState *> AutoDraw::getStates(){
    
    return this->arrayState;
    
}


int AutoDraw::numberStates(){
    return (int)this->arrayState.size();
}

int AutoDraw::numberTrans(){
    int trans_count = 0;
    for (AutoState * state : this->arrayState){
        trans_count += state->numberTrans();
    }
    return trans_count;
}

AutoTrans* AutoDraw::addTransition(int sourceStateId, int destStateId,int iLabel,int oLabel, float weight){
    
    
    AutoState* sourceState = this->arrayState[sourceStateId];
    AutoState* destState = this->arrayState[destStateId];
    AutoTrans* newTrans = new AutoTrans(sourceState,destState,iLabel,oLabel,weight);

    
    sourceState->addTransition(newTrans);
    
    fst::StdArc newArc = newTrans->getFstArc();
    mainFST->AddArc(sourceStateId, newArc);
    
    
    
    return newTrans;

    
    
    
}

int AutoDraw::retrieveStateID(float xCoor, float yCoor){
    
    for (AutoState * state : this->arrayState){
        if (state->has_state_at(xCoor, yCoor)){
            return state->id;
        }
    }
    
    
    
    return -1;
}

std::tuple<int,int,int> AutoDraw::retrieveStateTransID(float x, float y){
    
    // RETURN (stateID, transID, transType)
    
    
    for (AutoState * state : this->arrayState){
        if (state->has_state_at(x, y)){
            return std::make_tuple(state->id,-1,-1);
        }
        
        for (AutoTrans * trans : state->outTrans){
            int getTransType = trans->has_trans(x, y);
            
            if (getTransType == 0){
                this->mType = Label;
                
                return std::make_tuple(state->id,trans->id,getTransType);
                
            } else if (getTransType == 1){
                this->mType = ControlPoint1;
                return std::make_tuple(state->id,trans->id,getTransType);
            } else if (getTransType == 2){
                this->mType = ControlPoint2;
                return std::make_tuple(state->id,trans->id,getTransType);
            }
            
            
        }
    }
    
    return std::make_tuple(-1,-1,-1);
    
}


float AutoDraw::getTransCP1X(int stateIndex,int transIndex){
    
    return this->getTransQuality(stateIndex, transIndex, true, true, true);
    
}

float AutoDraw::getTransCP1Y(int stateIndex,int transIndex){
    
    return this->getTransQuality(stateIndex, transIndex, true, false, true);
}

float AutoDraw::getTransCP2X(int stateIndex,int transIndex){
    
    return this->getTransQuality(stateIndex, transIndex, true, true, false);
}

float AutoDraw::getTransCP2Y(int stateIndex,int transIndex){
    
    return this->getTransQuality(stateIndex, transIndex, true, false, false);
}

float AutoDraw::getInInterX(int stateIndex, int transIndex){
    return this->getTransQuality(stateIndex, transIndex, false, true, true);
}

float AutoDraw::getInInterY(int stateIndex, int transIndex){
    return this->getTransQuality(stateIndex, transIndex, false, false, true);
}

float AutoDraw::getOutInterX(int stateIndex, int transIndex){
    return this->getTransQuality(stateIndex, transIndex, false, true, false);
}

float AutoDraw::getOutInterY(int stateIndex, int transIndex){
    return this->getTransQuality(stateIndex, transIndex, false, false, false);
}


float AutoDraw::getTransQuality(int stateIndex, int transIndex, bool controlPoint, bool xValue,bool first){
    
    // If 'controlPoint' true, then search control points, else look at the intersections
    // 'first' for controlPoints is Control Point 1, and for intersection is inIntersection
    
    AutoState * state = this->arrayState[stateIndex];
    AutoTrans * trans = state->outTrans[transIndex];
    
    //Transition Caught
    std::tuple<float,float> tupleOfInterest;
    if (controlPoint) {
        if (first) {
            tupleOfInterest = trans->controlPoint1;
        } else {
            tupleOfInterest = trans->controlPoint2;
        }
    } else {
        if (first) {
            tupleOfInterest = trans->inIntersection;
        } else {
            tupleOfInterest = trans->outIntersection;
        }
    }
    float floatOfInterest;
    if (xValue){
        floatOfInterest = std::get<0>(tupleOfInterest);
    } else{
        floatOfInterest = std::get<1>(tupleOfInterest);
    }
    return  floatOfInterest;
    
}

float AutoDraw::getTransLabelX(int stateIndex, int transIndex){
    AutoState * state = this->arrayState[stateIndex];
    AutoTrans * trans = state->outTrans[transIndex];
    
    float labelX = std::get<0>(trans->label_position);
    return labelX;
}

float AutoDraw::getTransLabelY(int stateIndex, int transIndex){
    AutoState * state = this->arrayState[stateIndex];
    AutoTrans * trans = state->outTrans[transIndex];
    
    float labelY = std::get<1>(trans->label_position);
    return labelY;
}

int AutoDraw::getTransDestIndex(int stateIndex, int transIndex){
    AutoState * state = this->arrayState[stateIndex];
    AutoTrans *trans = state->outTrans[transIndex];
    
    return trans->destStateID;
}

bool AutoDraw::isFinalState(){
    if (this->mType == State) {
        return true;
    } else {
        return false;
    }
}


bool AutoDraw::setFinal(float fWeight){
    
    this->mainFST->SetFinal(this->moveStateID, fWeight);
    
    AutoState * state = this->arrayState[this->moveStateID];
    state->set_as_final(fWeight);
    
    
    
    
    this->mType = None;
    this->moveStateID = -1;
    this->moveTransID = -1;
    
    return true;
    
}


bool AutoDraw::getStateIdFinal(int stateIndex){
    
    AutoState * state = this->arrayState[stateIndex];
    
    return state->isFinal();
    
}

float AutoDraw::getStateIdFinalWeight(int stateIndex){
    AutoState * state = this->arrayState[stateIndex];
    
    return state->finalWeight;
}

std::__1::string AutoDraw::getTransLabel(int stateIndex, int transIndex){
    
    AutoState * state = this->arrayState[stateIndex];
    
    return state->getLabelTransID(transIndex);
    
}

void AutoDraw::toggle_all_cp(){
    for (AutoState * state : this->arrayState){
        state->toggle_all_cp();
    }
}



bool AutoDraw::moveStateTo(int stateID, float x, float y){
    
    AutoState * movedState = this->arrayState[stateID];
    
    movedState->x = x;
    movedState->y = y;
    
    // StateOfInterest out Transitions
    for (AutoTrans * trans : movedState->outTrans){
        AutoState * outState = this->arrayState[trans->destStateID];
        trans->recompute_transitionPoints(movedState, outState);
        
    }
    
    // If StateOfInterest has transitions pointing towards it
    for (AutoState * inState : this->arrayState){
        if (inState->id == stateID){
            continue;
        }
        
        if (inState->end_state_count[stateID] >= 1) {
            for (AutoTrans * trans : inState->outTrans){
                if (trans->destStateID == stateID){
                    trans->recompute_transitionPoints(inState, movedState);
                }
            }
        }
        
        
    }
    
    return true;
    
}


bool AutoDraw::moveAction(float x, float y){
    
    MoveType moveType = this->mType;
    
    
    switch (moveType) {
        case None:{
            
            
            std::tuple<int,int,int> retrieved_object = this->retrieveStateTransID(x, y);
            
            int retrivedState = std::get<0>(retrieved_object);
            int retrievedTrans = std::get<1>(retrieved_object);
            int transTypeRetrieved = std::get<2>(retrieved_object);
            
            bool stateRetrieved = (retrivedState != -1 && retrievedTrans == -1 && retrievedTrans == -1);
            
            
            if (stateRetrieved) {
                this->mType = State;
                this->moveStateID = retrivedState;
                this->moveTransID = -1;
            } else if (transTypeRetrieved  == 0){
                this->mType = Label;
                this->moveStateID = retrivedState;
                this->moveTransID = retrievedTrans;
            } else if (transTypeRetrieved == 1){
                this->mType = ControlPoint1;
                this->moveStateID = retrivedState;
                this->moveTransID = retrievedTrans;
            } else if (transTypeRetrieved == 2) {
                this->mType = ControlPoint2;
                this->moveStateID = retrivedState;
                this->moveTransID = retrievedTrans;
            } else { // Nothing Found
                this->mType = None;
                this->moveStateID = -1;
                this->moveTransID = -1;
            }
            
            break;
            
        }
            
        case State:{
            
            this->moveStateTo(this->moveStateID, x, y);
            
            this->mType = None;
            this->moveStateID = -1;
            this->moveTransID = -1;
            
            break;
        }
        case Label:{
            
            AutoState * state = this->arrayState[this->moveStateID];
            AutoTrans * trans = state->outTrans[this->moveTransID];
            
            trans->moveLabelPosition(x, y);
            
            this->mType = None;
            this->moveStateID = -1;
            this->moveTransID = -1;
            
            break;
        }
        case ControlPoint1:{
            
            AutoState * state = this->arrayState[this->moveStateID];
            AutoTrans * trans = state->outTrans[this->moveTransID];
            
            trans->moveControlPoint(1, x, y);
            
            this->mType = None;
            this->moveStateID = -1;
            this->moveTransID = -1;
            
            break;
        }
        case ControlPoint2:{
            
            AutoState * state = this->arrayState[this->moveStateID];
            AutoTrans * trans = state->outTrans[this->moveTransID];
            
            trans->moveControlPoint(2, x, y);
            
            this->mType = None;
            this->moveStateID = -1;
            this->moveTransID = -1;
            
            break;
        }
            
            
            
        default:
            break;
    }
    
    
    
    return true;
}



int AutoDraw::getMoveType(){
    
    // Return (0) if State; (1) if Transition; (-1) if nothing
    switch (this->mType) {
        case None:
            return -1;
        case State:
            return 0;
        case Label:
            return 1;
        case ControlPoint1:
            return 1;
        case ControlPoint2:
            return 1;
        default:
            return -1;
    }
}

int AutoDraw::getMoveStateID(){
    return this->moveStateID;
}

int AutoDraw::getMoveTransID(){
    return this->moveTransID;
}




#include "Grid.hpp"

AutoDraw * oFST_to_auto(fst::StdVectorFst *oFST){

    Grid * newGrid = new Grid(oFST);
    return newGrid->returnAutoDraw();
}

AutoDraw* testAuto1(){
    fst::StdVectorFst * newOFST = new fst::StdVectorFst();
    
    newOFST->AddState();
    newOFST->SetStart(0);
    
    newOFST->AddArc(0, fst::StdArc(1,1,0.5,1));
    newOFST->AddArc(0, fst::StdArc(2,2,1.5,1));
    
    newOFST->AddState();
    newOFST->AddArc(1, fst::StdArc(3,3,2.5,2));
    
    newOFST->AddState();
    newOFST->SetFinal(2, 3.5);
    
    
    AutoDraw* testAuto = oFST_to_auto(newOFST);
    
    
    return testAuto;
    
}

AutoDraw* testAuto2(){
    fst::StdVectorFst * newOFST = new fst::StdVectorFst();
    
    newOFST->AddState();
    newOFST->SetStart(0);
    newOFST->AddState();
    newOFST->AddState();
    newOFST->AddState();
    newOFST->AddState();
    
    newOFST->SetFinal(3, 1.0);
    newOFST->SetFinal(4, 3.0);
    
    newOFST->AddArc(0, fst::StdArc(1,1,2,1));
    newOFST->AddArc(0, fst::StdArc(2,2,2,1));
    newOFST->AddArc(0, fst::StdArc(3,3,3,1));
    newOFST->AddArc(0, fst::StdArc(4,4,3,2));
    newOFST->AddArc(0, fst::StdArc(6,6,1,2));
    
    newOFST->AddArc(1, fst::StdArc(6,6,3,3));
    newOFST->AddArc(1, fst::StdArc(7,7,2,3));
    
    newOFST->AddArc(2, fst::StdArc(6,6,5,4));
    newOFST->AddArc(2, fst::StdArc(7,7,4,4));
    
    
    AutoDraw * newAuto = oFST_to_auto(newOFST);
    
    return newAuto;
    
}

AutoDraw* testAuto3(){
    fst::StdVectorFst * newOFST = new fst::StdVectorFst();
    
    newOFST->AddState();
    newOFST->SetStart(0);
    newOFST->AddState();
//    newOFST->AddState();
    
    newOFST->AddArc(0, fst::StdArc(1,2,5,1));
    newOFST->AddArc(0, fst::StdArc(3,1,4,1));
//    newOFST->AddArc(0, fst::StdArc(2,4,1,2));
//    newOFST->AddArc(1, fst::StdArc(5,0,9,2));
    
    newOFST->SetFinal(1, 0.1);
    
    AutoDraw * newAuto = oFST_to_auto(newOFST);
    
    return newAuto;
}


AutoDraw* returnTestAuto(){
    
    return testAuto2();
}




AutoDraw * AutoDraw::getMinimizedVersion(){
    
    fst::StdVectorFst * duplicatedOFST = duplicateOFST(this->mainFST);
    
    fst::Minimize(duplicatedOFST);
    
    AutoDraw* newAuto = oFST_to_auto(duplicatedOFST);
    
    return newAuto;
    
    
    
}

fst::StdVectorFst * duplicateOFST(fst::StdVectorFst * orignialOFST){
    
    fst::StdVectorFst * newFST = new fst::StdVectorFst();
    
    // State
    for (fst::StateIterator<fst::StdVectorFst> siter(*orignialOFST); !siter.Done(); siter.Next()) {
        int id = siter.Value();
        
        newFST->AddState();
        if (id == 0) {
            newFST->SetStart(0);
        }
        
        
        fst::TropicalWeight weight = orignialOFST->Final(id);
        if (weight != fst::TropicalWeight::Zero()) {
            newFST->SetFinal(id, weight);
        }
        
        
        
        
        
        
    }
    
    // Transition
    for (fst::StateIterator<fst::StdVectorFst> siter(*orignialOFST); !siter.Done(); siter.Next()) {
        int id = siter.Value();
        for (fst::ArcIterator<fst::StdVectorFst> aiter(*orignialOFST,id); !aiter.Done(); aiter.Next()){
            fst::StdArc arc = aiter.Value();
            fst::StdArc::Label iLabel = arc.ilabel;
            fst::StdArc::Label oLabel = arc.olabel;
            fst::TropicalWeight weight = arc.weight;
            fst::StdVectorFst::StateId n = arc.nextstate;
            
            newFST->AddArc(id, fst::StdArc(iLabel,oLabel,weight,n));
            
        }
    }
    
    
    return newFST;
}

#define OPTIMAL_DIST 100

std::tuple<float,float> cost_connect(AutoState * state0, AutoState * state1){
    
    float diffX = state1->x - state0->x;
    float diffY = state1->y - state0->y;
    float aggro = powf(diffX,2) + powf(diffY,2);
    
    float distance = sqrtf(aggro);
    
    float D = OPTIMAL_DIST;
    
    float change = 2 * (distance - D);
    
    float costX = change * diffX/distance;
    float costY = change * diffY/distance;
    
    
    return std::make_tuple(costX,costY);
    
    
}

std::tuple<float,float> cost_non_connect(AutoState * state0, AutoState * state1){
    float diffX = state1->x - state0->x;
    float diffY = state1->y - state0->y;
    float aggro = powf(diffX,2) + powf(diffY,2);
    
    float distance = sqrtf(aggro);
    
    float D = OPTIMAL_DIST;
    
    float change;
    if (distance < D) {
        change = 2 * (distance - D);
    } else {
        change = 0;
    }
    
    float costX = change * diffX/distance;
    float costY = change * diffY/distance;
    
    
    return std::make_tuple(costX,costY);
}

void AutoDraw::beautify(){
    float scalar = 0.1;
    int threshold = 1;
    float min_change = 5.0;
    
    
    int array_size = (int)this->arrayState.size();
    
    
    
    for (int count = 0; count < threshold; count++) {
        std::vector<float> changeX = std::vector<float>(array_size);
        std::vector<float> changeY = std::vector<float>(array_size);
        for (AutoState * state : this->arrayState){
            
            float aggro_x = 0;
            float aggro_y = 0;
            
            for (AutoState * otherState : this->arrayState){
                
                if (state->id == otherState->id) {
                    continue;
                }
                
                std::tuple<float,float> cost_tuple;
                if (state->has_dest_state(otherState)) {
                    cost_tuple = cost_connect(state, otherState);
                    
                } else {
                    cost_tuple = cost_non_connect(state, otherState);
                }
                float costX = std::get<0>(cost_tuple);
                float costY = std::get<1>(cost_tuple);
                
                aggro_x += costX;
                aggro_y += costY;
                
            }
            
            changeX[state->id] = scalar * aggro_x;
            changeY[state->id] = scalar * aggro_y;
            
            
        }
        
        float max_delta = 0;
        
        
        for (int i = 0; i < array_size; i++) {
            float deltaX = changeX[i];
            float deltaY = changeY[i];
            
            
            
            if (deltaX > max_delta) {
                max_delta = deltaX;
            }
            
            if (deltaY > max_delta) {
                max_delta = deltaY;
            }
            
            
            

            
        }
        
        if (max_delta < min_change){
            // All Changes are insignificant
            break;
        }
        
        
        for (int i = 0; i < array_size; i++) {
            AutoState * state = this->arrayState[i];
            float deltaX = changeX[i];
            float deltaY = changeY[i];
            
            state->x += deltaX;
            state->y += deltaY;
        }
        

    }
    
    
    // Recompute Transition Points
    
    for (AutoState * state : this->arrayState){
        for (AutoTrans * trans : state->outTrans){
            trans->recompute_transitionPoints(state, this->arrayState[trans->destStateID]);
        }
    }

    
    
    
    
}


