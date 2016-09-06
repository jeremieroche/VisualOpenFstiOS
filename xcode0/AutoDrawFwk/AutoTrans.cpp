//
//  AutoTrans.cpp
//  AutoDrawFwk
//
//  Created by Jeremie Roche on 7/2/16.
//  Copyright © 2016 jeremieroche. All rights reserved.
//

#include "AutoTrans.hpp"
#include <math.h>


std::tuple<float,float,float,float> compute_control_points(AutoState * sourceState, AutoState * destState, int transID);
std::tuple<float,float,float,float> compute_intersections(AutoState * inState, AutoState * outState, float cp1X, float cp1Y, float cp2X, float cp2Y);
std::tuple<float,float> compute_label_position(float t,float startX, float startY, float endX, float endY, float cp1X, float cp1Y, float cp2X, float cp2Y);

AutoTrans::AutoTrans(AutoState *inState,AutoState* outState, int inputLabel, int outputLabel, float weight){
    
    
    this->id = (int)inState->outTrans.size();
    
    int destinationStateID = outState->id;
    
    this->destStateID = destinationStateID;
    this->show_controlPoints = false;
    

    
    this->fstArc = fst::StdArc(inputLabel, outputLabel,weight,destinationStateID);
    
    int transID = (int) inState->outTrans.size();
    std::tuple<float,float,float,float> controlPoint_coordinates = compute_control_points(inState, outState,transID);
    float cp1X = std::get<0>(controlPoint_coordinates);
    float cp1Y = std::get<1>(controlPoint_coordinates);
    float cp2X = std::get<2>(controlPoint_coordinates);
    float cp2Y = std::get<3>(controlPoint_coordinates);
    
    this->controlPoint1 = std::make_tuple(cp1X,cp1Y);
    this->controlPoint2 = std::make_tuple(cp2X,cp2Y);
    
    
    std::tuple<float,float,float,float> intersection_coordinates = compute_intersections(inState, outState, cp1X, cp1Y, cp2X, cp2Y);
    float inInterX = std::get<0>(intersection_coordinates);
    float inInterY = std::get<1>(intersection_coordinates);
    float outInterX = std::get<2>(intersection_coordinates);
    float outInterY = std::get<3>(intersection_coordinates);
    
    this->inIntersection  = std::make_tuple(inInterX,inInterY);
    this->outIntersection = std::make_tuple(outInterX,outInterY);
    
    
    std::tuple<float, float> label_coordinates = compute_label_position(0.5, inInterX, inInterY, outInterX, outInterY, cp1X, cp1Y, cp2X, cp2Y);
    float labelX = std::get<0>(label_coordinates);
    float labelY = std::get<1>(label_coordinates);
    
    this->label_position = std::make_tuple(labelX,labelY);
    
    
}


std::tuple<float,float,float,float> compute_control_points(AutoState * sourceState, AutoState * destState, int transID){
    
    
    float cp1X;
    float cp1Y;
    float cp2X;
    float cp2Y;
    
    int revised_index = 0;
    for (AutoTrans * trans : sourceState->outTrans) {
        
        if (trans->id == transID) {
            break;
        }
        
        if (trans->destStateID == destState->id) {
            revised_index++;
        }
        
    }
    
    if (sourceState->id == destState->id){
        float delta = 10;
        float displacement = 4;
        cp1X = sourceState->x - (delta * (revised_index + displacement));
        cp2X = sourceState->x + (delta * (revised_index + displacement));
        
        float yCoor = sourceState->y - (delta * (revised_index + displacement));
        cp1Y = yCoor;
        cp2Y = yCoor;
    }
    else if (revised_index == 0){
        
        float midX = (sourceState->x + destState->x)/2;
        float midY = (sourceState->y + destState->y)/2;
        
        cp1X = midX;
        cp1Y = midY;
        cp2X = midX;
        cp2Y = midY;

    } else {
        
        // Returns (1) ControlPoint1 X; (2) ControlPoint1 Y; (3) ControlPoint2 X; (4) ConrolPoint2 Y
        
        float x = destState->x - sourceState->x;
        float y = destState->y - sourceState->y;
        
        float d = sqrtf(x * x + y * y);
        
        float out = float(100);
        
        int index = revised_index-1;
        
        if (index % 2 == 0) {
            out *= float(index + 1) /float(4);
        } else {
            out *= (-1) * float(index) / float(4);
        }
        
        float modifier = float(0.4);
        
        cp1X = sourceState->x + (modifier) * x + (out/d) * (-1) * y;
        cp1Y = sourceState->y + (modifier) * y + (out/d) * x;
        
        cp2X = sourceState->x + (1-modifier) * x + (out/d) * (-1) * y;
        cp2Y = sourceState->y + (1-modifier) * y + (out/d) * x;
    }
    
    
    
    return std::make_tuple(cp1X,cp1Y,cp2X,cp2Y);
    
}

std::tuple<float,float,float,float> compute_intersections(AutoState * inState, AutoState * outState, float cp1X, float cp1Y, float cp2X, float cp2Y)
{
    
    
    // Intersections
    float radius = float(15);
    
    // In Intersections
    float new_vecX = cp1X - inState->x;
    float new_vecY = cp1Y - inState->y;
    
    float comp1 = pow(new_vecX, 2);
    float comp2 = pow(new_vecY, 2);
    float norm = sqrtf(comp1 + comp2);
    float normVecX = new_vecX/norm * radius;
    float normVecY = new_vecY/norm * radius;
    
    float inInterX = normVecX + inState->x;
    float inInterY = normVecY + inState->y;
    
    
    //Out Intersections
    
    new_vecX = cp2X - outState->x;
    new_vecY = cp2Y - outState->y;
    
    comp1 = pow(new_vecX, 2);
    comp2 = pow(new_vecY, 2);
    norm = sqrtf(comp1 + comp2);
    normVecX = new_vecX/norm * radius;
    normVecY = new_vecY/norm * radius;
    
    float outInterX = normVecX + outState->x;
    float outInterY = normVecY + outState->y;
    
    return std::make_tuple(inInterX,inInterY,outInterX,outInterY);

}

std::tuple<float,float> compute_label_position(float t,float startX, float startY, float endX, float endY, float cp1X, float cp1Y, float cp2X, float cp2Y){
    
    float x0 = startX; float x1 = cp1X; float x2 = cp2X; float x3 = endX;
    
    float segment0 = x0;
    float segment1 = 3 * t * (x1 - x0);
    float segment2 = 3 * pow(t,2) * (x0 + x2 - 2 * x1);
    float segment3 = pow(t,2)* (x3 - x0 + 3 * x1 - 3 * x2);
    
    float xCoor = segment0 + segment1 + segment2 + segment3;
    
    float y0 = startY; float y1 = cp1Y; float y2 = cp2Y; float y3 = endY;
    
    float segmentA = y0;
    float segmentB = 3 * t * (y1 - y0);
    float segmentC = 3 * pow(t,2) * (y0 + y2 - 2 * y1);
    float segmentD = pow(t,3) * (y3 - y0 + 3 * y1 - 3 * y2);
    
    float yCoor = segmentA + segmentB + segmentC + segmentD;
    
    return std::make_tuple(xCoor,yCoor);
}



fst::StdArc AutoTrans::getFstArc(){
    return this->fstArc;
}

std::__1::string AutoTrans::getFullLabel(){
    
    std::stringstream readable_weight;
    readable_weight << std::fixed << std::setprecision(2) << this->fstArc.weight.Value();
//    readable_weight << std::fixed << std::setprecision(2) << this->weight;
    
    
    
    string full_label = std::to_string(this->fstArc.ilabel) + ":" +  std::to_string(this->fstArc.olabel) + "/" +  readable_weight.str();
    
    return full_label;
}

bool AutoTrans::recompute_transitionPoints(AutoState * inState, AutoState* outState){
    
    int transID = this->id;
   
    
    // Control Points
    std::tuple<float,float,float,float> controlPoint_coordinates = compute_control_points(inState, outState,transID);
    float cp1X = std::get<0>(controlPoint_coordinates);
    float cp1Y = std::get<1>(controlPoint_coordinates);
    float cp2X = std::get<2>(controlPoint_coordinates);
    float cp2Y = std::get<3>(controlPoint_coordinates);
    
    this->controlPoint1 = std::make_tuple(cp1X,cp1Y);
    this->controlPoint2 = std::make_tuple(cp2X,cp2Y);
    
    
    // Intersection Points
    std::tuple<float,float,float,float> intersection_coordinates = compute_intersections(inState, outState, cp1X, cp1Y, cp2X, cp2Y);
    float inInterX = std::get<0>(intersection_coordinates);
    float inInterY = std::get<1>(intersection_coordinates);
    float outInterX = std::get<2>(intersection_coordinates);
    float outInterY = std::get<3>(intersection_coordinates);
    
    this->inIntersection  = std::make_tuple(inInterX,inInterY);
    this->outIntersection = std::make_tuple(outInterX,outInterY);
    
    // Label Position
    recompute_labelPoint();
    
    
    return true;
}

bool AutoTrans::recompute_intersection(AutoState * inState, AutoState* outState){
    
    float cp1X = std::get<0>(this->controlPoint1);
    float cp1Y = std::get<1>(this->controlPoint1);
    float cp2X = std::get<0>(this->controlPoint2);
    float cp2Y = std::get<1>(this->controlPoint2);
    
    std::tuple<float,float,float,float> intersection_coordinates = compute_intersections(inState, outState, cp1X, cp1Y, cp2X, cp2Y);
    float inInterX = std::get<0>(intersection_coordinates);
    float inInterY = std::get<1>(intersection_coordinates);
    float outInterX = std::get<2>(intersection_coordinates);
    float outInterY = std::get<3>(intersection_coordinates);
    
    this->inIntersection  = std::make_tuple(inInterX,inInterY);
    this->outIntersection = std::make_tuple(outInterX,outInterY);
    
    return true;
    
}

bool AutoTrans::recompute_labelPoint(){
    
    float cp1X = std::get<0>(this->controlPoint1);
    float cp1Y = std::get<1>(this->controlPoint1);
    float cp2X = std::get<0>(this->controlPoint2);
    float cp2Y = std::get<1>(this->controlPoint2);
    
    float inInterX = std::get<0>(this->inIntersection);
    float inInterY = std::get<1>(this->inIntersection);
    float outInterX = std::get<0>(this->outIntersection);
    float outInterY = std::get<1>(this->outIntersection);
    
    
    
    std::tuple<float, float> label_coordinates = compute_label_position(0.5, inInterX, inInterY, outInterX, outInterY, cp1X, cp1Y, cp2X, cp2Y);
    float labelX = std::get<0>(label_coordinates);
    float labelY = std::get<1>(label_coordinates);
    
    this->label_position = std::make_tuple(labelX,labelY);
    
    return true;
    
    
    
}

void AutoTrans::toggle_show_cp(){
    this->show_controlPoints = !this->show_controlPoints;
}




int AutoTrans::has_trans(float xCoor, float yCoor){
    
    // Return Values:
    // 0: Label
    // 1: Control Point 1
    // 2: Control Point 2
    // -1: <<Transition Not Found>>
    
    
    if (this->has_trans_label(xCoor, yCoor)){
        return 0;
    } else if (this->show_controlPoints && this->has_trans_control_point(1, xCoor, yCoor)){
        return 1;
    } else if (this->show_controlPoints && this->has_trans_control_point(2, xCoor, yCoor)){
        return 2;
    } else {
        return -1; // Transition not found
    }
    
    
}

bool within_bound(int xCoor, int yCoor, int compXCoor, int compYCoor){
    float window = 15.0;
    
    bool point_below_upper_x = xCoor < compXCoor + window;
    bool point_above_lower_x = xCoor > compXCoor - window;
    bool point_left_right_y  = yCoor < compYCoor + window;
    bool point_right_left_y  = yCoor > compYCoor - window;
    
    bool withinBound = point_below_upper_x && point_above_lower_x && point_left_right_y && point_right_left_y;
    
    return withinBound;
    
}

bool AutoTrans::has_trans_control_point(int controlPointNumber, float xCoor, float yCoor){
    
    bool inBound;
    if (controlPointNumber == 1){
        float cp1X = std::get<0>(this->controlPoint1);
        float cp1Y = std::get<1>(this->controlPoint1);
        inBound = within_bound(xCoor, yCoor, cp1X, cp1Y);
    } else { // controlPointNumber == 2
        float cp2X = std::get<0>(this->controlPoint2);
        float cp2Y = std::get<1>(this->controlPoint2);
        inBound = within_bound(xCoor, yCoor, cp2X, cp2Y);
    }
    
    return inBound;
}

bool AutoTrans::has_trans_label(float xCoor, float yCoor){
    
    float labelX = std::get<0>(this->label_position);
    float labelY = std::get<1>(this->label_position);
    bool inBound = within_bound(xCoor, yCoor, labelX, labelY);
    
    return inBound;
    
}




bool AutoTrans::moveControlPoint(int number, float xCoor, float yCoor){
    
    if (number == 1){
        this->controlPoint1 = std::make_tuple(xCoor,yCoor);
        
        this->recompute_labelPoint();
        return true;

    } else if (number == 2){
        this->controlPoint2 = std::make_tuple(xCoor,yCoor);
        this->recompute_labelPoint();
        return true;
    } else {
        return false;
    }
    
    
}

bool AutoTrans::moveLabelPosition(float xCoor, float yCoor){
    
    float labelX = std::get<0>(this->label_position);
    float labelY = std::get<1>(this->label_position);
    float deltaX = xCoor - labelX;
    float deltaY = yCoor - labelY;
    
    float cp1X = std::get<0>(this->controlPoint1);
    float cp1Y = std::get<1>(this->controlPoint1);
    float cp2X = std::get<0>(this->controlPoint2);
    float cp2Y = std::get<1>(this->controlPoint2);
    
    this->controlPoint1 = std::make_tuple(cp1X + deltaX,cp1Y + deltaY);
    this->controlPoint2 = std::make_tuple(cp2X + deltaX,cp2Y + deltaY);
    
    
    this->recompute_labelPoint();
    
    
    
    
    return true;
}













