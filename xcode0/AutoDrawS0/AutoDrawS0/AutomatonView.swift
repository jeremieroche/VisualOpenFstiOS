//
//  AutomatonView.swift
//  AutoDrawS0
//
//  Created by Jeremie Roche on 7/8/16.
//  Copyright © 2016 jeremieroche. All rights reserved.
//

import UIKit

let global_radius : Int = 15

class AutomatonView: UIView {
    
    var automaton : AUT
    
    required init?(coder aDecoder: NSCoder) {
        self.automaton = createSampleAut()

        super.init(coder: aDecoder)
    }
    
    //MARK: Drawing
    override func drawRect(rect: CGRect) {
        
        let stateCount : Int32 = nbStates(self.automaton)
        
        let moveType : Int32 = getMoveType(self.automaton)
        let moveStateID : Int32 = getMoveStateID(self.automaton)
        let moveTransID : Int32 = getMoveTransID(self.automaton)
        
        
        for stateID : Int32 in 0..<stateCount{
            
            let moveState : Bool = (moveType == 0)
            let moveTrans : Bool = (moveType == 1)
            
            let stateIdMoveStateEquivalence : Bool = (stateID == moveStateID)
            let stateSelected = moveState && stateIdMoveStateEquivalence
            
            self.drawState(stateID, isState: stateSelected)
            
            
            var transIndex : Int32 = 0
            while hasTrans(self.automaton,stateID,transIndex) {
                
                let transIdMoveTransEquivalnce : Bool = (transIndex == moveTransID)
                let transSelected : Bool = moveTrans && stateIdMoveStateEquivalence && transIdMoveTransEquivalnce
                
                self.drawTrans(from: stateID, using: transIndex, selected: transSelected)
                transIndex++;
            }
            
        }
        
        
    }
    
    
    //MARK: State Action
    func insertState(at point:CGPoint){
        
        let pointX = Float(point.x)
        let pointY = Float(point.y)
        
        addState(self.automaton, pointX, pointY)
        
        
        self.setNeedsDisplay()
    }
    
    
    func retrieveState(from point:CGPoint) -> Int32?{
        
        let pointX = Float(point.x)
        let pointY = Float(point.y)
        
        let stateID : Int32 = getStateId(self.automaton, pointX, pointY)
        
        if stateID == -1{
            return nil
        } else {
            return stateID
        }
        
    }
    
    
    func set_final_action(with_final_weight fWeight : Float32)->Bool {
        let set_valid : Bool = setFinal(self.automaton, fWeight)
        
        if set_valid{
            self.setNeedsDisplay();
        }
        
        return set_valid
    }
    
    func is_state_final() -> Bool {
        return isFinalState(self.automaton)
    }
    
    
    
    //MARK: Drawing State
    private func drawState(stateId: Int32, isState: Bool){
        
        let drawPoint = self.getStateCoor(state: stateId)
        
        self.drawStateCircle(drawPoint, using: stateId, isSelected: isState)
        self.drawID(stateId, at: drawPoint)
        
        
        if stateId == 0{
            self.drawInitialArrow(drawPoint)
            
        }

        
    }
    
    private func getStateCoor(state stateID: Int32) -> CGPoint{
        let floatX : CGFloat = CGFloat(getStatePosX(self.automaton, stateID))
        let floatY : CGFloat = CGFloat(getStatePosY(self.automaton, stateID))
        
        let thePoint = CGPointMake(floatX, floatY)
        
        return thePoint
    }
    
    private func drawStateCircle(centerPoint : CGPoint, using stateID: Int32, isSelected selected: Bool){
        
        
        let path : UIBezierPath = UIBezierPath(arcCenter: centerPoint, radius: CGFloat(global_radius), startAngle: 0.0, endAngle: CGFloat(2.0 * M_PI), clockwise: true)
        
        UIColor.blackColor().setStroke()
        
        path.lineWidth = 1
        path.stroke()
        
        
        // Final Circle (2nd Outer Circle)
        if isFinal(self.automaton, stateID){
            let outerCircle : UIBezierPath = UIBezierPath(arcCenter: centerPoint, radius: CGFloat(global_radius + 3), startAngle: 0.0, endAngle: CGFloat(M_PI * 2.0), clockwise: true)
            outerCircle.lineWidth = 1
            outerCircle.stroke()
        }
        

        if selected
        {
            UIColor.grayColor().setFill()
            path.fill()
                
        }
        

    }
    
    private func drawInitialArrow(relativePoint : CGPoint){
        
        let end_point : CGPoint = CGPointMake(relativePoint.x - CGFloat(global_radius * 3), relativePoint.y)
        let tip : CGPoint = CGPointMake(relativePoint.x - CGFloat(global_radius), relativePoint.y)
        
        let modifier : Int = 25
        
        let side1 : CGPoint = rotate_vector(by: CGFloat(180 - modifier), relative: tip, rotating_point: relativePoint)
        let side2 : CGPoint = rotate_vector(by: CGFloat(180 + modifier), relative: tip, rotating_point: relativePoint)
        
        let path_direction : UIBezierPath = UIBezierPath()
        path_direction.moveToPoint(end_point)
        path_direction.addLineToPoint(tip)
        path_direction.lineWidth = 1
        
        let path_arrow : UIBezierPath = UIBezierPath()
        path_arrow.moveToPoint(side1)
        path_arrow.addLineToPoint(tip)
        path_arrow.addLineToPoint(side2)
        path_arrow.lineWidth = 1
        
        path_direction.stroke()
        path_arrow.stroke()
        
        
    }
    
    private func drawID(stateID : Int32, at labelPoint:CGPoint){
        
        let final : Bool = isFinal(self.automaton, stateID)
        
        
        
        let idString :String
        if final{
            let fWeight : Float32 = getFinalWeight(self.automaton, stateID)
            idString = "\(stateID)/\(fWeight)"
        } else {
            idString = String(stateID)
        }
        
        let idLabel  = NSAttributedString(string: idString)
        
        let labelWidth : CGFloat = idLabel.size().width/2
        let labelHeight : CGFloat = idLabel.size().height/2
        let updatedLabelPoint : CGPoint = CGPointMake(labelPoint.x - labelWidth, labelPoint.y - labelHeight)
        
        
        idLabel.drawAtPoint(updatedLabelPoint)
    }
    
    
    //MARK: Transition Action
    func transitionAction(at point: CGPoint, inputLabel iLabel : Int32, outputLabel oLabel : Int32, weight : Float32) -> Bool{
        let retrieved_number : Int32 = getStateId(self.automaton, Float(point.x), Float(point.y))
        
        if (retrieved_number != -1){
            
            if let sourceStateID = sourceStateID{
                let destinationStateID = retrieved_number;
                
                addTransition(self.automaton, sourceStateID, destinationStateID, iLabel, oLabel, weight)
                self.sourceStateID = nil;
                self.setNeedsDisplay();
                
                return true // Complete
                
            } else {
                self.sourceStateID = retrieved_number;
                return false // Incomplete
            }
            
            
        } else {
            return false
        }
        
        
    }
    
    //MARK: Drawing Transition
    
    private func drawTrans(from sourceStateID : Int32, using transID : Int32, selected transSelect : Bool){
        
        // Path
        let path : UIBezierPath = UIBezierPath()
        
        
        let (_,destPoint,cp1,cp2,inInter,outInter) : (CGPoint,CGPoint,CGPoint,CGPoint,CGPoint,CGPoint) = self.getLineInfo(from: sourceStateID, using: transID)

        self.drawLine(path, cp1: cp1, cp2: cp2, inIntersection: inInter, outIntersection: outInter, selected: transSelect)
        self.drawArrow(destPoint, outIntersection: outInter, controlPoint2: cp2)
        self.draw_label(startID: sourceStateID, trans: transID)
        
        if (self.show_control_points){
            self.draw_control_points(controlPoint1: cp1, controlPoint2: cp2, inIntersection: inInter, outIntersection: outInter)
            
        }
        
        
    }
    
    private func getLineInfo(from sourceStateID: Int32, using transID : Int32)->(CGPoint,CGPoint,CGPoint,CGPoint,CGPoint,CGPoint){
        
        let sourcePoint : CGPoint = self.getStateCoor(state: sourceStateID)
        
        let destStateID: Int32 = getTransDestIndex(self.automaton, sourceStateID, transID)
        let destPoint : CGPoint = self.getStateCoor(state: destStateID)

        let cp1X : CGFloat = CGFloat(getTransCP1X(self.automaton, sourceStateID, transID))
        let cp1Y : CGFloat = CGFloat(getTransCP1Y(self.automaton, sourceStateID, transID))
        let cp1 : CGPoint = CGPointMake(cp1X, cp1Y)
        
        let cp2X : CGFloat = CGFloat(getTransCP2X(self.automaton, sourceStateID, transID))
        let cp2Y : CGFloat = CGFloat(getTransCP2Y(self.automaton, sourceStateID, transID))
        let cp2 : CGPoint = CGPointMake(cp2X, cp2Y)
        
        let inInterX : CGFloat = CGFloat(getTransInInterX(self.automaton, sourceStateID, transID))
        let inInterY : CGFloat = CGFloat(getTransInInterY(self.automaton, sourceStateID, transID))
        let inInter : CGPoint = CGPointMake(inInterX, inInterY)
        
        let outInterX : CGFloat = CGFloat(getTransOutInterX(self.automaton, sourceStateID, transID))
        let outInterY : CGFloat = CGFloat(getTransOutInterY(self.automaton, sourceStateID, transID))
        let outInter : CGPoint = CGPointMake(outInterX, outInterY)
        
        return (sourcePoint,destPoint,cp1,cp2,inInter,outInter)
    }
    
    private func drawLine(path: UIBezierPath, cp1: CGPoint, cp2: CGPoint, inIntersection: CGPoint, outIntersection: CGPoint, selected transSelect : Bool){
        path.moveToPoint(inIntersection)
        path.addCurveToPoint(outIntersection, controlPoint1: cp1, controlPoint2: cp2)
        
        path.lineWidth = 1
        
        if transSelect{
            UIColor.redColor().setStroke()
        } else {
            UIColor.blackColor().setStroke()

        }
        
        
        path.stroke()
    }
    
    private func drawArrow(destPoint:CGPoint, outIntersection: CGPoint, controlPoint2: CGPoint){
        
        let v1 : CGFloat = controlPoint2.x - destPoint.x
        let v2 : CGFloat = controlPoint2.y - destPoint.y
        
        let norm : CGFloat = sqrt(pow(v1,2) + pow(v2,2))
        
        let radius : CGFloat = CGFloat(15)
        
        let u1 : CGFloat = radius/norm * v1
        let u2 : CGFloat = radius/norm * v2
        
        let pX : CGFloat = outIntersection.x + u1
        let pY : CGFloat = outIntersection.y + u2
        let p : CGPoint = CGPointMake(pX, pY)
        
        let side1Point : CGPoint = self.rotate_vector(by: 30, relative: outIntersection, rotating_point: p)
        let side2Point : CGPoint = self.rotate_vector(by: -30, relative: outIntersection, rotating_point: p)
        
        let rotated_points = [side1Point,side2Point]
        
        let path : UIBezierPath = UIBezierPath()
        path.moveToPoint(rotated_points[0])
        path.addLineToPoint(outIntersection)
        path.addLineToPoint(rotated_points[1])
        
        path.lineWidth = 1
        path.stroke()
        
    }
    
    var sourceStateID : Int32?
    
    
    private func draw_label(startID startID : Int32, trans transID: Int32){
        
        
        let label_string : String = String.fromCString(getTransLabel(self.automaton, startID, transID))!
        let label : NSAttributedString = NSAttributedString(string: label_string)
        
        let labelWidth : CGFloat = label.size().width

        
        let labelX : CGFloat = CGFloat(getTransLabelX(self.automaton, startID, transID))
        let labelXCorrected : CGFloat = labelX - labelWidth/2
        let labelY : CGFloat = CGFloat(getTransLabelY(self.automaton, startID, transID))
        let label_position : CGPoint = CGPointMake(labelXCorrected, labelY)
        
        
        
        label.drawAtPoint(label_position)
        
    }
    
    
    //MARK: ControlPoints
    
    var show_control_points : Bool = false
    
    func toggleControlPoints(){
        self.show_control_points = !self.show_control_points
        
        toggle_all_cp(self.automaton);
        
        self.setNeedsDisplay()
    }
    
    private func draw_control_points(controlPoint1 cp1:CGPoint, controlPoint2 cp2:CGPoint,
        inIntersection inInter: CGPoint, outIntersection outInter: CGPoint){
            
            let circle1 = UIBezierPath(arcCenter: cp1, radius: 5, startAngle: 0, endAngle: CGFloat(M_PI * 2.0), clockwise: true)
            let circle2 = UIBezierPath(arcCenter: cp2, radius: 5, startAngle: 0, endAngle: CGFloat(M_PI * 2.0), clockwise: true)
            
            let circle_line_combo = [
                (circle1,cp1,inInter),
                (circle2,cp2,outInter)
            ]
            
            for (circle,center,intersection) in circle_line_combo{
                circle.lineWidth = 1
                UIColor.blackColor().setStroke()
                UIColor.yellowColor().setFill()
                circle.fill()
                circle.stroke()
                
                let path = UIBezierPath()
                path.moveToPoint(center)
                path.addLineToPoint(intersection)
                path.lineWidth = 1
                UIColor.orangeColor().setStroke()
                path.stroke()
            }
        
    }
    
    //MARK: Moving Objects
    
    func moveAction(at point: CGPoint){
        let floatX : Float = Float(point.x)
        let floatY : Float = Float(point.y)
        
        move_action(self.automaton, floatX, floatY)
        self.setNeedsDisplay()
        
    }
    
  
    
    //MARK: Minimization
    
    func minimize_automaton(){
        
        let newMinimizedAuto : AUT = getMinimizedAuto(self.automaton)
        
        let model = AutomatonCollectionModel.sharedMegaCollAutoModel()
        model.megaAuto.append(newMinimizedAuto)

        self.automaton = newMinimizedAuto
        self.setNeedsDisplay()
    }
    
    //MARK: Beautify
    func beautify_auto(){
        beautify(self.automaton)
        self.setNeedsDisplay()
    }
    
    
    
    //MARK: Math
    private func rotate_vector(by degree: CGFloat, relative: CGPoint, rotating_point: CGPoint) -> CGPoint{
        
        // Linear Algebra: Rotate by Matrix Rotation Function
        let rRadians : Double = Double(degree * CGFloat(M_PI / 180))
        
        var rMatrix = Matrix(rows: 2, columns: 2)
        rMatrix[0,0] = cos(rRadians)
        rMatrix[0,1] = -sin(rRadians)
        rMatrix[1,0] = sin(rRadians)
        rMatrix[1,1] = cos(rRadians)
        
        
        let constX : CGFloat = CGFloat(rMatrix[0,0]) * rotating_point.x + CGFloat(rMatrix[0,1]) * rotating_point.y
        let constY : CGFloat = CGFloat(rMatrix[1,0]) * rotating_point.x + CGFloat(rMatrix[1,1]) * rotating_point.y
        let rotated : CGPoint = CGPointMake(constX, constY)
        
        
        
        var adjRMat : Matrix = Matrix(rows: 2, columns: 2)
        adjRMat[0,0] = 1 - rMatrix[0,0]
        adjRMat[0,1] = -rMatrix[0,1]
        adjRMat[1,0] = -rMatrix[1,0]
        adjRMat[1,1] = 1 - rMatrix[1,1]
        
        let place1 : CGFloat = CGFloat(adjRMat[0,0]) * relative.x + CGFloat(adjRMat[0,1]) * relative.y
        let place2 : CGFloat = CGFloat(adjRMat[1,0]) * relative.x + CGFloat(adjRMat[1,1]) * relative.y
        let placeHolder : CGPoint = CGPointMake(place1,place2)
        
        
        let adjPoint : CGPoint = CGPointMake(rotated.x + placeHolder.x, rotated.y + placeHolder.y)
        
        return adjPoint
        
    }
    
}

struct Matrix{
    let rows : Int, columns : Int
    var grid : [Double]
    
    init(rows:Int,columns:Int){
        self.rows = rows
        self.columns = columns
        self.grid = Array(count: rows*columns, repeatedValue: 0.0)
    }
    
    func indexIsValidForRow(r:Int, c:Int) -> Bool{
        return  r >= 0 && r < rows && c >= 0 && c < columns
    }
    
    subscript(r:Int,c:Int) -> Double{
        get{
            assert(indexIsValidForRow(r, c: c),"Index is out of Range")
            return grid[(r*columns) + c]
        }
        
        set {
            assert(indexIsValidForRow(r, c: c), "Index out of Range")
            grid[(r * columns) + c] = newValue
            
        }
        
    }
}
