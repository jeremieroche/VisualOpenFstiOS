//
//  TestAut.hpp
//  AutoDrawFwk
//
//  Created by Jeremie Roche on 7/8/16.
//  Copyright © 2016 jeremieroche. All rights reserved.
//

#ifndef TestAut_hpp
#define TestAut_hpp

#include <stdio.h>

#include "fst/fstlib.h"


class TestAut : public fst::StdVectorFst {
    float x;
    float y;
    
public:
    void addNewState(float x, float y);

};

#endif /* TestAut_hpp */
