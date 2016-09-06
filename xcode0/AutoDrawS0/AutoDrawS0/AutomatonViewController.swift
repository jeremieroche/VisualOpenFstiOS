//
//  ViewController.swift
//  AutoDrawS0
//
//  Created by Jeremie Roche on 7/6/16.
//  Copyright © 2016 jeremieroche. All rights reserved.
//

import UIKit

class AutomatonViewController: UIViewController , UIPopoverPresentationControllerDelegate{
    
    var insertAuto : AUT?
    override func viewDidLoad() {
        super.viewDidLoad()
        
        if let iAuto = self.insertAuto{
            self.AutoView.automaton = iAuto
            self.insertAuto = nil
            
        } else {
            let collection : AutomatonCollectionModel = AutomatonCollectionModel.sharedMegaCollAutoModel()
            
            collection.megaAuto.append(self.AutoView.automaton)
        }
        
        
        
        self.enable_all_buttons(true)
        self.set_final_button.enabled = false
        
        self.AutoView.setNeedsDisplay()
        
        
        
    }
    
    @IBOutlet weak var AutoView: AutomatonView!
    
    @IBAction func tap_action(sender: UITapGestureRecognizer) {
        
        let tapPoint : CGPoint = sender.locationInView(self.AutoView)
        
        if enable_add_state{
            
            self.AutoView.insertState(at: tapPoint)
            
            self.enable_add_state = false
            self.enable_all_buttons(true)
            
            
            return
        }
        
        if enable_add_trans{
            let complete = self.AutoView.transitionAction(at: tapPoint, inputLabel: self.set_transition_tuple!.0, outputLabel: self.set_transition_tuple!.1, weight: self.set_transition_tuple!.2)
            
            if complete{
                self.set_transition_tuple = nil
                self.enable_add_trans = false
                self.enable_all_buttons(true)
            } else {
                self.info_label.text = "Select 2nd State"
            }
            
            return
            
        }
        
        self.AutoView.moveAction(at: tapPoint)
        
        self.set_final_button.enabled = self.AutoView.is_state_final()
        
    }
    
    
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        
        if segue.identifier == "set_transition"
        {
            let vc = segue.destinationViewController as! SetTransitionPopViewController
            vc.setParent(self)
            
            let popover = vc.popoverPresentationController
            
            if popover != nil{
                vc.preferredContentSize = CGSizeMake(300, 200)
                popover!.delegate = self
            }
            return
        }
        
        if segue.identifier == "set_final"
        {
            let vc = segue.destinationViewController as! SetFinalPopViewController
            vc.setParent(self)
            
            let popover  = vc.popoverPresentationController
            
            if popover != nil{
                vc.preferredContentSize = CGSizeMake(180, 120)
                popover!.delegate = self
            }
            return
        }
        
    }
    func adaptivePresentationStyleForPresentationController(controller: UIPresentationController) -> UIModalPresentationStyle {2
        return .None
    }
    
    
    
    //MARK: Buttons

    @IBOutlet weak var add_state_button: UIButton!
    @IBOutlet weak var add_trans_button: UIButton!
    @IBOutlet weak var set_final_button: UIButton!
    @IBOutlet weak var toggle_control_point_button: UIButton!
    
    var enable_add_state : Bool = false
    var enable_add_trans : Bool = false
    var adjust_transition : Bool = false
    
    private func enable_all_buttons(enable:Bool){
        
        
        if enable{
            
            let state_count : Int32 = nbStates(self.AutoView.automaton)
            let trans_count : Int32 = nbTrans(self.AutoView.automaton)
            
            add_state_button.enabled = true
            add_trans_button.enabled = (state_count >= 1)
            toggle_control_point_button.enabled = true
            beautify_button.enabled = (trans_count >= 1)
            minimize_button.enabled = (trans_count >= 1)
            
            self.info_label.text = ""
            
            
        } else {
            add_state_button.enabled = false
            add_trans_button.enabled = false
            toggle_control_point_button.enabled = false
            beautify_button.enabled = false
            minimize_button.enabled = false
        }
        
        
        
    }
    
    @IBAction func add_state_action(sender: UIButton) {
        
        self.enable_add_state = true
        
        self.enable_all_buttons(false)
        
        self.info_label.text = "Insert State"
        
    }
    
    @IBAction func add_trans_action(sender: UIButton) {
        
    }
    
    @IBAction func set_final_action(sender: UIButton) {
        
        self.enable_all_buttons(false)
        
    }
    
    @IBAction func toggle_control_point_action(sender: UIButton) {
        self.AutoView.toggleControlPoints()
    }
        
    @IBOutlet weak var info_label: UILabel!
    
    func set_final_weight(weight : Float32){
        self.AutoView.set_final_action(with_final_weight: weight)
        
        self.enable_all_buttons(true)
        self.set_final_button.enabled = false
    }
    
    var set_transition_tuple : (Int32, Int32,Float32)?
    func set_transition(inputLabel iLabel : Int32, outputLabel oLabel : Int32, weight : Float32){
        self.set_transition_tuple = (iLabel,oLabel,weight)
        
        self.enable_add_trans = true
        
        self.enable_all_buttons(false)
        
        self.info_label.text = "Select 1st State"

    }
    
    @IBOutlet weak var minimize_button: UIButton!
    @IBAction func function_action(sender: AnyObject) {
        self.minimize_automaton()
    }
    
    func minimize_automaton(){
        self.AutoView.minimize_automaton()
        
    }
    
    @IBOutlet weak var beautify_button: UIButton!
    @IBAction func beautify_action(sender: UIButton) {
        self.AutoView.beautify_auto()
    }
    
    
    
}

