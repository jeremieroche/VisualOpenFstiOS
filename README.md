# VisualOpenFstiOS

Open Source Project on Visual Finite-State Transducers. Programmed Primarily in Swift and C++.

Using the OpenFST Library to use their transducers (specifically “StdVectorFst” and “StdArc”)

http://www.openfst.org/

## Run Program

1. Clone Repository
2. Open Project (In "xcode0" directory) in XCode
3. Run Simulator

## Overview

App contains 4 View Controllers:

* ATVC: AutomatonTableViewController (starting View Controller on Launch)
* AVC: AutomatonViewController (Main Automaton Functions)
* STPVC: SetTransitionPopViewController
* SFPVC: SetFinalPopViewController


## AutomatonViewController (AVC)

![alt tag](https://github.com/jeremieroche/VisualOpenFstiOS/blob/master/Automaton_Pics/Blank_AVC.png)



* Contains a View, where the Automaton is visually represented. View should be blank when a new Automaton is created (b/c the automaton has no state nor transition)

View Contains 4 modes
* Normal Mode: Tapping on a State/Transition will cause it to be selected. Next tap will move the corresponding object to the tapped point. 
  * Note: For transitions, one can move the Label position or either Control Point Positions
* Add State Mode (ASM): Tap point on view, and circular state appears (then ASM ends)
  * Note: If the state is the first in the Automaton, it is an initial state; hence small arrow points to it
* Add Transition Mode (ATM): Tap location of 2 states, and transition from first tapped state to second tapped state is formed (then ATM ends)
* Set Final Mode (SFM): Tap State, then state is a final state with the weight (then SFM ends)

Automaton buttons:
* State: Begins ASM
* Transition: Brings up a STPVC (after Transition variables set, ATM begins) 
* Final: Brings up a SFPVC (after final weight set, SFM begins)
* Control Points: Toggles on/off automaton showing its control points

Minimize Button:
* Creates Minimized version of the Automaton using the OpenFST library (http://www.openfst.org/twiki/bin/view/FST/MinimizeDoc)



## SetTransitionPopViewController (STPVC):
(Popped after tapping on the “Transition” Button in AVC) 

* Type in a Number (Int/Float only) on the text field, and then tap the “Input”/“Output”/“Weight” button to assign the field’s value to the corresponding variable


## SetFinalPopViewController (SFPVC):
(Popped after tapping on the “Final” Button in AVC)

* Type in a Number (Int/Float only) on the text field to set a final Weight

## Demonstartion

###### Test Automaton

![alt tag](https://github.com/jeremieroche/VisualOpenFstiOS/blob/master/Automaton_Pics/Sample_AVC.png)

###### Minimized Automaton (After Pressing Minimize Button)

![alt tag](https://github.com/jeremieroche/VisualOpenFstiOS/blob/master/Automaton_Pics/Minimized_Automaton.png)

###### Moving Automaton: Tap State

![alt tag](https://github.com/jeremieroche/VisualOpenFstiOS/blob/master/Automaton_Pics/State_Selection.png)

###### Moving Automaton: Tap New State Location

![alt tag](https://github.com/jeremieroche/VisualOpenFstiOS/blob/master/Automaton_Pics/State_Moved.png)

###### Moving Automaton: Tap Transition

![alt tag](https://github.com/jeremieroche/VisualOpenFstiOS/blob/master/Automaton_Pics/Trans_Selected.png)

###### Moving Automaton: Tap New Transition Location (w/ respect to Label)

![alt tag](https://github.com/jeremieroche/VisualOpenFstiOS/blob/master/Automaton_Pics/Trans_Moved.png)

###### Added State (after pressing “State” button, and then tapping on designated location)

![alt tag](https://github.com/jeremieroche/VisualOpenFstiOS/blob/master/Automaton_Pics/New_State.png)

###### Set Transition Pop View Controller (after pressing "Transition" button)

![alt tag](https://github.com/jeremieroche/VisualOpenFstiOS/blob/master/Automaton_Pics/SetTransPVC.png)

###### Automaton with new Transition (after tapping 1st and 2nd State)

![alt tag](https://github.com/jeremieroche/VisualOpenFstiOS/blob/master/Automaton_Pics/New_Trans.png)

###### Set Final Weight Pop View Controller (after tapping on State, and then pressing “Final)

![alt tag](https://github.com/jeremieroche/VisualOpenFstiOS/blob/master/Automaton_Pics/SetFinalPVC.png)

###### Automaton with newly set Final State (after tapping State)

![alt tag](https://github.com/jeremieroche/VisualOpenFstiOS/blob/master/Automaton_Pics/Final_Set.png)

###### Beautified Automaton (after pressing the “Beautify” button several times)

![alt tag](https://github.com/jeremieroche/VisualOpenFstiOS/blob/master/Automaton_Pics/Beautfied.png)

###### Automaton Showing the Control Points (after pressing “Control Point” Button)

![alt tag](https://github.com/jeremieroche/VisualOpenFstiOS/blob/master/Automaton_Pics/Control_Points.png)

###### Beautified Demonstration: Before

![alt tag](https://github.com/jeremieroche/VisualOpenFstiOS/blob/master/Automaton_Pics/Beautify_Before.png)

###### Beautified Demonstration: After (pressing "Beautified" several times)

![alt tag](https://github.com/jeremieroche/VisualOpenFstiOS/blob/master/Automaton_Pics/Beautify_After.png)


## AutomatonTableViewController (ATVC):
(Starting View Controller when app launches)

![alt tag](https://github.com/jeremieroche/VisualOpenFstiOS/blob/master/Automaton_Pics/ATVC.png)

* Each cell stores information of an Automaton, which contains an OpenFST “StdVectorFst” 
* The title of each Automaton is "Automaton (count): (s_count) States, (t_count) Transitions“, where (count) is the order in which Automaton was created, (s_count) is the number of states in the automaton, and (t_count) is the number of transitions. (eg "Automaton 8: 4 States, 6 Transitions“)
  * Note: (count) starts at 0
* When a cell is tapped an AVC will appear with a Automaton visualized
* Tap “+” button (Top Right Hand Corner) to create brand new empty Automaton in an AVC.


