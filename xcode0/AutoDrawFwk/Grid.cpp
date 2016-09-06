//
//  Grid.cpp
//  AutoDrawFwk
//
//  Created by Jeremie Roche on 7/6/16.
//  Copyright © 2016 jeremieroche. All rights reserved.
//

#include "Grid.hpp"
#include <tuple>
#include "AutoState.hpp"
#include "AutoTrans.hpp"
#include <math.h>


Grid::Grid(){
    this->coordinate_grid = {};
    this->autoDraw = new AutoDraw();
    
}

#define D_INFINITY 100000000
Grid::Grid(fst::StdVectorFst * oFST){
    
    this->coordinate_grid = {};
    this->autoDraw = new AutoDraw();
    
    // BFS (Breadth First Search)
    std::vector<long> d_array = std::vector<long>(oFST->NumStates()); // Distances
    std::vector<int>  p_array = std::vector<int>(oFST->NumStates()); // Parents
    
    for (fst::StateIterator<fst::StdVectorFst > siter(*oFST);
         !siter.Done();
         siter.Next()){
        
        int state_index=(int)siter.Value();
        
        d_array[state_index] = D_INFINITY; // Infinity
        
    }
    
    d_array[0] = 0;
    p_array[0] = -1;
    
    
    
    
    
    
    std::queue<int> *state_queue = new std::queue<int>();
    
    
    int start_id = oFST->Start();
    
    state_queue->push(start_id);
    
    std::vector<AutoState *> state_array = std::vector<AutoState *>(d_array.size());
    
    AutoState * firstState = this->addState();
    
    fst::TropicalWeight weight = oFST->Final(start_id);
    if (weight != fst::TropicalWeight::Zero()) {
        firstState->set_as_final(weight.Value());
    }
    
    state_array[0] = firstState;
    
    while (!state_queue->empty()) {
        int current = state_queue->front();
        state_queue->pop();
        
        for (fst::ArcIterator<fst::StdVectorFst> aiter(*oFST,current); !aiter.Done(); aiter.Next()){
            fst::StdArc arc = aiter.Value();
            fst::StdArc::Label iLabel = arc.ilabel;
            fst::StdArc::Label oLabel = arc.olabel;
            fst::TropicalWeight weight = arc.weight;
            fst::StdVectorFst::StateId n = arc.nextstate;
            
            if (d_array[n] == D_INFINITY) {
                d_array[n] = d_array[current] + 1;
                p_array[n] = current;
                state_queue->push(n);
                
                AutoState* newState = this->addStateFrom(state_array[current], iLabel, oLabel, weight.Value());
                
                fst::TropicalWeight weight = oFST->Final(n);
                if (weight != fst::TropicalWeight::Zero()) {
                    this->autoDraw->setFinalState(newState, weight.Value());
                }
                
                state_array[n] = newState;
                
                
            } else {
                this->addTrans(state_array[current], state_array[n], iLabel, oLabel, weight.Value());
            }
        }
    }
    

}


AutoState * Grid::addState(){
    
    
    std::tuple<float,float> first_point = this->get_next_point_from(NULL);
    float fp_x = std::get<0>(first_point);
    float fp_y = std::get<1>(first_point);
    
    
    AutoState * newState = this->autoDraw->addState(fp_x, fp_y);
    
    return newState;
}


AutoState * Grid::addStateFrom(AutoState* originalState, int ilabel, int oLabel, float weight){
    
    std::tuple<float,float> next_point = this->get_next_point_from(originalState);
    float np_x = std::get<0>(next_point);
    float np_y = std::get<1>(next_point);
    
    AutoState * addedState = this->autoDraw->addState(np_x, np_y);
    
    
    this->addTrans(originalState, addedState, ilabel, oLabel, weight);
        
    return addedState;
    
}

AutoTrans * Grid::addTrans(AutoState* sourceState, AutoState* destState, int ilabel, int oLabel, float weight){
    
    AutoTrans* newTrans = this->autoDraw->addTransition(sourceState->id, destState->id, ilabel, oLabel, weight);
    
    return newTrans;
    
    
}

std::tuple<float,float> Grid::get_next_point_from(AutoState* originalState){
    
    int nState_x;
    int nState_y;
    
    
    if (originalState != NULL) {
        
        std::tuple<int,int> position = this->grid_get_pos(originalState);
        float oState_x = std::get<0>(position);
        
        nState_x = oState_x + 1;
        if (nState_x >= this->coordinate_grid.size()) {
            this->add_column();
        }
        
        nState_y = (int) this->coordinate_grid[nState_x].size();
        
        
    } else {
        nState_x = 0;
        if (this->coordinate_grid.size() == 0) {
            nState_y = 0;
        } else {
            nState_y = (int) this->coordinate_grid[0].size();
        }
    }
    
    
    
    std::tuple<float,float> graph_point = dimension_to_coor_conversion(nState_x, nState_y);
    
    
    
    this->put_in_grid(graph_point, nState_x);
    
    return graph_point;
    
}

void Grid::add_column(){
    this->coordinate_grid.push_back({});
}


std::tuple<int,int> Grid::grid_get_pos(AutoState* originalState){
    
    
    
    for (int i = 0; i < this->coordinate_grid.size(); i++){
        std::vector<std::tuple<float,float>> vec_states = this->coordinate_grid[i];
        
        for (int j = 0; j < vec_states.size(); j++) {
            std::tuple<float,float> state_loc = vec_states[j];
            
            float sl_x = std::get<0>(state_loc);
            float sl_y = std::get<1>(state_loc);
            
            bool xEqual = (sl_x == originalState->x);
            bool yEqual = (sl_y == originalState->y);
            
            
            bool points_equal = (xEqual && yEqual);
            
            if (points_equal) {
                return std::make_tuple(i,j);
            }

        }
        
    }
    
    return NULL;
    
}





std::tuple<float,float> dimension_to_coor_conversion(int xValue, int yValue){
    
    
    int jump = 100;
    
    
    // X coordinate
    float xCoor = (float) jump * xValue + jump/2;
    
    // Y coordinate
    int wing = 2;
    int span = (wing + 1) * jump;
    
    float yCoor;
    if (yValue < span){
        float control_point = wing * jump + jump/2;
        float adjusted_point = pow(-1, yValue) * (jump * ((yValue + 1)/2));
        
        yCoor = (float) (control_point + adjusted_point);
    } else {
        float adjusted_y = span * jump + (yValue - span);
        yCoor = (float) (adjusted_y + jump/2);
    }
    
    return std::make_tuple(xCoor,yCoor);
    
    
    
}



void Grid::put_in_grid(std::tuple<float,float> graph_point, int column){
    
    if (this->coordinate_grid.size() <= 0) {
        this->add_column();
    }
    
    this->coordinate_grid[column].push_back(graph_point);
    
}

AutoDraw * Grid::returnAutoDraw(){
    return this->autoDraw;
}



