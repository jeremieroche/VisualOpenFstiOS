//
//  AutomatonCollectionModel.swift
//  AutoDrawS0
//
//  Created by Jeremie Roche on 8/20/16.
//  Copyright © 2016 jeremieroche. All rights reserved.
//

import UIKit

class AutomatonCollectionModel: NSObject {
    
    static var instance : AutomatonCollectionModel!
    
    var megaAuto = [AUT]()
    
    class func sharedMegaCollAutoModel() -> AutomatonCollectionModel!{
        
        
        if (self.instance == nil){
            let newModel = AutomatonCollectionModel()
            
            let minizableAuto : AUT = getTestAuto()
            
            
            
            newModel.megaAuto = [minizableAuto]
            
            self.instance = newModel
        }
        
        
        return self.instance
        
    }

}
