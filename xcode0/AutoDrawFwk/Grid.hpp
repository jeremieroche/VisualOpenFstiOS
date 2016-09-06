//
//  Grid.hpp
//  AutoDrawFwk
//
//  Created by Jeremie Roche on 7/6/16.
//  Copyright © 2016 jeremieroche. All rights reserved.
//

#ifndef Grid_hpp
#define Grid_hpp

#include <stdio.h>
#include "fst/fst.h"
#include <tuple>
#include "AutoState.hpp"
#include "AutoDraw.hpp"
#include "AutoTrans.hpp"



class Grid{
    
public:
    
    Grid();
    Grid(fst::StdVectorFst * oFST);
    
    
    std::vector<std::vector<std::tuple<float,float>>> coordinate_grid;
            
    
    AutoDraw * autoDraw;
    AutoDraw * returnAutoDraw();
    
    
    AutoState * addState();
    AutoState * addStateFrom(AutoState * originalState, int ilabel, int oLabel, float weight);
    AutoTrans * addTrans(AutoState* sourceState, AutoState* destState, int ilabel, int oLabel, float weight);
    
    
    std::tuple<float,float> get_next_point_from(AutoState* originalState);
    std::tuple<int,int> grid_get_pos(AutoState* originalState);
    void add_column();
    
    void put_in_grid(std::tuple<float,float> graph_point, int column);
    
    
};


std::tuple<float,float> dimension_to_coor_conversion(int xValue, int yValue);



#endif /* Grid_hpp */
