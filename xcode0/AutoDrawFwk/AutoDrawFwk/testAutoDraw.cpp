//
//  testAutoDraw.cpp
//  AutoDrawFwk
//
//  Created by Jeremie Roche on 7/2/16.
//  Copyright © 2016 jeremieroche. All rights reserved.
//

#include "testAutoDraw.hpp"

#include "fst/fstlib.h"

void testFunc1(void){
    
    fst::StdVectorFst fst;
    
    fst.AddState();
    
    fst.SetStart(0);
    
    fst.AddArc(0, fst::StdArc(1, 1, 0.5, 1));
    fst.AddArc(0, fst::StdArc(2, 2, 1.5, 1));
    
    fst.AddState();
    fst.AddArc(1, fst::StdArc(3, 3, 2.5, 2));
    
    fst.AddState();
    fst.SetFinal(2, 3.5);
    
    fst.Write("/Users/jeremieroche/Documents/openFST_trial/cpp_test0/binary2.fst");
}

