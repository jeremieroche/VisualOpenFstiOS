//
//  SetFinalPopViewController.swift
//  AutoDrawS0
//
//  Created by Jeremie Roche on 8/28/16.
//  Copyright © 2016 jeremieroche. All rights reserved.
//

import UIKit

class SetFinalPopViewController: UIViewController {
    
    
    var AutoViewController : AutomatonViewController?
    
    func setParent(parent : AutomatonViewController){
        self.AutoViewController = parent
    }
    
    
    @IBOutlet weak var set_weight_field: UITextField!
    
    @IBAction func set_action(sender: AnyObject) {
        
        if let weight_string : String = self.set_weight_field.text{
            
            if let weight_float : Float32 = Float32(weight_string){
                
                self.AutoViewController!.set_final_weight(weight_float)
                self.dismissViewControllerAnimated(false, completion: nil)

            }
            
        }
        
    }

}
