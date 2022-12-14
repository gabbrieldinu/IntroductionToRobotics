# IntroductionToRobotics

This repository is dedicated to materials and homeworks regarding an introductory robotics class from the University of Bucharest

# Homework 0
 
 Create a git repository with a brief description.
 
 # Homework 1
 
   Connect three potentiometers to a RGB Led, each potentiometer controlling the intensity of a color. To achieve this I used a breadboard, Arduino UNO board, 3 potentiometers, RGB Led, 3 resistors (330 Ohms) and connectivity wires.
      
   The solution along with a picture of the boards and a video showcasing the functionality are situated in the Laboratory 1 branch ( here is also a link to the video https://youtu.be/YtodR4_9IK0)
   
 # Homework 2:
  
   Recreate a traffic light intersection using LEDS and a button. To accomplish this, we need 3 LEDS ( Red, Green, Yellow) for the cars and 2 LEDS ( Red and Green) for the pedestrians and there will be four states:
      - First State: red for pedestrians, green for cars (rest of LEDS turned off) 
      - Second State : red for pedestrians, yellow for cars (rest of LEDS turned off)
      - Third State : green for pedestrians, red for cars (rest of LEDS turned off)
      - Fourth State : blinking green for pedestrians, red for cars (rest of LEDS turned off)
      
   A pedestrian may change the Traffic Light's state by pressing the button ( it can only change from state 1 to state 2, pressing the button in any other state won't have any effect).
   
   The solution along with a picture of the boards and a video showcasing the functionality are situated in the Laboratory 2 branch ( here is also a link to the video (https://youtu.be/1S1jZKbuubU)
   
   # Homework 3:
   
   On a digit 7-segment, use a joystick to move from one segment to another and be able to turn on/off the current  ( from neight to neightbor using the table down below to check legal moves).
   There are two interchangeable states:
    - First one: Using the joystick, move from one segment to another, with the current position blinking on and off. Pressing the joystick button will trigger the second state.
      -- Allowed moves:
     Current segment    UP     DOWN    LEFT    RIGHT
           a           N/A      g       f        b
           b            a       g       f       N/A 
           c            g       d       e        DP
           d            g      N/A      e        c
           e            g       d      N/A       c
           f            a       g      N/A       b
           g            a       d      N/A      N/A 
           DP          N/A      g       c       N/A
           
   - Second State: The current segment stops blinking and now moving the joystick on a chosen axis will turn it on or off. Pressing the joystick button will save the current stae of the segment led (on/off) and return to the first state.
   
   Aditionally, long pressing the joystick button in state 1 will reset the configuration, turning off all segment and the current position will be on DP.
   
   The solution along with a picture of the boards and a video showcasing the functionality are situated in the Laboratory 3 branch ( here is also a link to the video https://www.youtube.com/watch?v=hPHrosC8ZJo ).
   
   # Homework 4:
   
   On a 4 digit 7-segment display, with all displays showing 0, using the X axis of a joystick, move from one DP to another, and using the Y axis change the digit displayed. Long pressing the joystick button will reset all numbers to 0.
   
   There are two states:
     - First one: Cycle through the DPs using the X axis. Pressing the joystick button will lock the current DP and enable the second state.
     - Second one: Cycle through 0-f using the Y axis. Pressing the joystick button will lock the current digit and return to the first state.
     
   The solution along with a picture of the boards and a video showcasing the functionality are situated in the Laboratory 4 branch ( here is also a link to the video https://www.youtube.com/shorts/YqDgysah26s ).
   
   # Homework 5:
   
   Create an interactive menu on an LCD Display. The menu should contain the following options:
    - start game 
    - settings
    - about section
    - how to play section
   In the settings, the user may change the following:
    - lcd brightness
    - lcd contrast
    - matrix brightness
    - sound on / off
    The solution along with a picture of the boards and a video showcasing the functionality are situated in the Laboratory 5 branch ( here is also a link to the video  https://www.youtube.com/watch?v=UHEjxcRh5lQ ).
    
