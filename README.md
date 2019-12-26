# Introduction
This is the final work for Basic Programming course (2019 Fall).  
This project is a LOGO language interpreter, which reads "fake" LOGO language (an educational program language for kids) and executes it by C++.  
The project is constructed by Qt Creator, so we have both Windows and MacOS version.    
# Demo Video  
https://www.bilibili.com/video/av80567732/  
# Highlights
The highlights of our program is that we can display the animation of traces on the window for you!  
Also, our program has some additional commands, which enables you to draw many beautiful but complex images with more convenience.  
Enjoy yourself!  
# Usage
Tips: You may replace the content in "[" and "]" (include "[" and "]) below by a number (integer or decimal) or a defined value.  
You can add a single negative sign before the number or value, such as "-3" or "-a". We support it. But you can't write an expression.  
## Basic
### Head datas
@SIZE [W] [H]  
@BACKGROUND [R] [G] [B]  
@POSITION [X] [Y]	//(0, 0) is at the upper left corner.  
### Basic commands
DEF [Name] [Value]  
ADD [Name] [Value]	//e.g. ADD A 1 refers to A += 1  
MOVE [Step]  
COLOR [R] [G] [B]  
CLOAK  
### Control commands
LOOP [LOOP_TIMES]  
    ...  
END LOOP  

FUNC [Name]([Para 1],[Para 2],...,[Para n])	//define a function before the main commands  
	...  
END FUNC  

CALL [FUNC_Name]([Para 1],[Para 2],...,[Para n])  

## Advanced
SET_WIDTH [WIDTH] //set the width of the trace line  

    IF [VALUE] EQUAL [VALUE]  
or: IF [VALUE] GREATER_THAN [VALUE]  
or: IF [VALUE] LESS_THAN [VALUE]  
	...  
END IF  

MINUS [Name] [VALUE]	//[Name] -= [VALUE]  
MULTIPLY [Name] [VALUE]	//[Name] *= [VALUE]  
DIVIDE [Name] [VALUE]	//[Name] /= [VALUE]  

SET_PIXEL [X] [Y] [R] [G] [B]	//set the color of pixel at (X, Y) as color (R, G, B)  