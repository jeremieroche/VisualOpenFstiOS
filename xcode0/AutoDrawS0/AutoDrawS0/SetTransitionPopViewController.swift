//
//  AdjustTransitionPopViewController.swift
//  AutoDrawS0
//
//  Created by Jeremie Roche on 8/26/16.
//  Copyright © 2016 jeremieroche. All rights reserved.
//

import UIKit

class SetTransitionPopViewController: UIViewController {
    
    
    private var AutoViewController : AutomatonViewController?
    
    func setParent(parent : AutomatonViewController){
        self.AutoViewController = parent
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        
        self.iLabel_status.text = String(iLabel)
        self.oLabel_status.text = String(oLabel)
        self.weight_status.text = String(weight)
        
    }
    
    var iLabel : Int32 = 1
    var oLabel : Int32 = 1
    var weight : Float32 = 1.0
    
    
    @IBOutlet weak var iLabel_status: UILabel!
    @IBOutlet weak var oLabel_status: UILabel!
    @IBOutlet weak var weight_status: UILabel!
    
    
    @IBOutlet weak var change_textFeild: UITextField!
    
     
    @IBAction func set_iLabel_action(sender: UIButton) {
        
        if let change_text : String = self.change_textFeild.text{
            if let newILabel : Float32 = Float32(change_text) {
                let newILabel_int : Int32 = Int32(newILabel)
                self.iLabel_status.text = String(newILabel_int)
                self.iLabel = newILabel_int
            }
        }
        
        
        
    }
    @IBAction func set_oLabel_action(sender: UIButton) {
        if let change_text : String = self.change_textFeild.text{
            if let newOLabel : Float32 = Float32(change_text){
                let newOLabel_int : Int32 = Int32(newOLabel)
                self.oLabel_status.text = String(newOLabel_int)
                self.oLabel = newOLabel_int
            }
            
        }
    }
    @IBAction func set_weight_action(sender: UIButton) {
        if let change_text : String = self.change_textFeild.text{
            if let newWeight : Float32 = Float32(change_text){
                self.weight_status.text = String(newWeight)
                self.weight = newWeight
            }
        }
    }
    
    
    @IBAction func set_transition_action(sender: AnyObject) {
        
        self.AutoViewController!.set_transition(inputLabel: self.iLabel, outputLabel: self.oLabel, weight: self.weight)
        
        self.dismissViewControllerAnimated(false, completion: nil)
        
        
    }
 }
