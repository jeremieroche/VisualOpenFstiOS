//
//  AutomatonTableViewController.swift
//  AutoDrawS0
//
//  Created by Jeremie Roche on 8/20/16.
//  Copyright © 2016 jeremieroche. All rights reserved.
//

import UIKit

class AutomatonTableViewController: UITableViewController {
    
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        self.tableView.registerClass(AutomatonTableViewCell.self, forCellReuseIdentifier: "cell")
        
        
    }
    
    override func viewDidAppear(animated: Bool) {
        self.tableView.reloadData()
        
    }
    
    
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        if let avc = segue.destinationViewController as? AutomatonViewController{
            if let identifier = segue.identifier{
                switch identifier{
                    case "form_auto":
                        
                        
                        avc.insertAuto = senderAuto!
                        self.senderAuto = nil
                        break
                default: break
                }
            }
        }
    }
    
    //MARK: Table Cell
    
    override func numberOfSectionsInTableView(tableView: UITableView) -> Int {
        return 1;
    }
    
    override func tableView(tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        let model : AutomatonCollectionModel = AutomatonCollectionModel.sharedMegaCollAutoModel()
        return model.megaAuto.count
    }
    
    override func tableView(tableView: UITableView, cellForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCell{
        let cell = tableView.dequeueReusableCellWithIdentifier("cell", forIndexPath: indexPath) as! AutomatonTableViewCell
        
        let model = AutomatonCollectionModel.sharedMegaCollAutoModel()
        
        let auto : AUT = model.megaAuto[indexPath.row]
        
        let state_count : Int32 = nbStates(auto)
        let trans_count : Int32 = nbTrans(auto)
        
        var cellText : String = ""
        
        cellText.appendContentsOf(String(format: "Automaton %d: %d States, %d Transitions", indexPath.row, state_count,trans_count))
        
        cell.textLabel!.text = (cellText == "") ? "N/A" : cellText
        
        cell.openFST = auto
        
        return cell
    }
    
    
    var senderAuto : AUT?
    
    override func tableView(tableView: UITableView, didSelectRowAtIndexPath indexPath: NSIndexPath){
        let model = AutomatonCollectionModel.sharedMegaCollAutoModel()
        
        let row_num : Int = indexPath.row
        
        let auto : AUT = model.megaAuto[row_num]
            
        senderAuto = auto
        self.performSegueWithIdentifier("form_auto", sender: nil)
        
    }
    
}
