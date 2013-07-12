roboskel_git
============

1)cord_pub 

  Description : Transmits the cordinates of the tracked person's head in [X,Y,Z] form, as well as calculating 
  the distance from the Kinect sensor.
  
  To run :  
  -	rocore  
  -	roslaunch openni_launch openni.launch  
  -	rosrun openni_tracker openni_tracker  
  -	rosrun cord_pub cord_pub
   
2) simpub3

  Description : Calculates the distances of the tracke user's head and uses them to issue tilt and turn commands 
  to the Kinect tilt motor and mast servo so it can track a moving user's head. This is done by also 
  trying to minimize movement due to the problematic nature of Kinect tracker when the sensor is also moving
  
  To run :  
  - roslaunch simpub3 tracker2.launch
  
3) gamepad_control 

  Description : Receives input from a gamepad/joystick and translates it to a message that will be used by two SDC2130 
  controlled motors. Current configuration uses a PS3-Dualshock like controller. The gamepad node translates the gamepad
  message to a "gp_functions" message which the gamepad_listener node can listen to and manipulate it to send different
  commands to the SDC controller node. At the moment the gamepad_listener is redundant as it simply relays the message
  but as functionality is added to the robot it will become more important. The gpsim is a test node, which can be used 
  to test keyboard input.
  
  Currently supported : 2 movement spots for controlling the two motors' speed (analog sticks and D-pad)
  and 12 available function spots (buttons and analog stick when pushed in), without taking into consideration combined
  button presses.
  
  To run :  
  -	roscore  
  -	rosrun joy joy_node  
  -	rosrun gamepad_control gamepad / gpsim  
  -	rosrun gamepad_control gamepad_listener
   
4) sdc2130

  Description : A ROS wrapper for the Linux C++ API provided by Roboteq for its controllers. It issues movement commands
  to two motors controlled by and SDC2130. Currently provides a maximum speed of 200, with it being controlled by the
  analog sticks (the further the analog stick is pushed, the more the corresponding motor's speed is increased, with a 
  maximum of 200) to simulate acceleration.
  
  Supported Functions :  
  	+ Independent Movement to the two motors via the analog sticks  
  	+ Using the D-pad overrides the analog sticks enabling maximum speed forward and backwards movement as well as in place
    turns  
    + Immediate Shutdown by pressing the Start Button  
    + 5-Second Halt Using the O Button, during which all movement orders transmited via gamepad are ignored  
    + Pressing the triangle button stops the robot, ignoring all movement orders, until triangle is pressed again  
    + Basic Odometry Functionality : By pressing the X button odometry is initialized, setting the position of the vehicle
      at 0,0 and an angle of 0 degrees. From that point on, each time the X button is pressed, the vehicle's position is recorded
      in a text file named roboskel_log.txt located in the ~/.ros directory.

  To run :   
  -	roslaunch sdc2130 gremote.launch  
    
5) recorder 

  Description : Multi aspect recording nodes using the Kinect. The project is comprised of 4 different nodes that can be run individually or  
    in any combination the user needs. These are :  
  - record_skel : Records the joint positions (X,Y,Z) returned by the openni_tracker and writes them in a file named  
    skel.txt in the following format : Timestam<space><joint1_x><space><joint1_y><space>...<last_jointz><endl>.  
    The recordings are about 1/10 second apart.
    
  - record_hok : Records the readings of a UTM-30LX Hokuyo laser scanner and writes the in a file named hok.txt in the 
    following format <Timestamp><first_reading><space>...<last_reading><endl>. The recordings are about 1/10 apart.  
      
  - listener_s.py : Records audio from an the selected audio recording device at intervals that can be combined to a singe  
    stream. The files are writen in the ./rec/audio directory which must be empty before the script is started and abide to   
    the following format <timestamp>.wav  
    
  - rec3 : Records rgb images and depth data from the Kinect at intervals of about 1/10 second using the opencv libraries.  
    The files are saved as rgb<timestamp>.png for the rgb images and dpt<timestamp>.bin for the depth data  
    
    To run :
    Every node can be run individualy using the corresponding .launch file. For the record_skel and re3 nodes oppeni_launch  
    must be launched successfully before hand as if it fails to launch (as it quite often does) the whole node will be rendered useless.  
    
    record_skel:  
    - roslaunch openni_launch openni.launch
    - roslaunch recorder skel.launch
    User presses START to start recording and the X button to stop it  

    record_hok:  
    - roslaunch openni_launch openni.launch
    - roslaunch recorder hok.launch  
    User presses START to start recording and the X button to stop it  
    
    listener.py
    - roslaunch audio.launch
    User presses START to start recording and the X button to stop it  
    
    rec3
    - roslaunch openni_launch openni.launch
    - roslaunch recorder rec3.launch
    User presses START to start recording and the X button to stop it  

    
    ALL the nodes
    - roslaunch openni_launch openni.launch  
    - roslaunch recorder full.launch
    User presses START to start recording and the X button to stop it  
    
6) face_rec  

  Description : A ROS wrapper for a face tracking program developed by Alexandros Loulemes. The program uses opencv to   
  capture images from a camera and detect faces, and annotate them with a circle of different colour for each face detected.  
  The ROS node records the id of each face, its coordinates and the radius of the circle around it. It then proceeds to   
  publish a message containing this information for each face detected in every frame it processes. The message published is  
  a vector of messages each containing the above information  
  
  To run :
  -  roslaunch openni_launch openni.launch
  -  roslaunch face_rec fr.launch
  User presses START to start tracking and sending messages and the X button to quit the program

    
  
