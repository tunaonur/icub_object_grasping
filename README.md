# icub_object_grasping

#### Recognition of Objects using AR Tags and Moving iCub Arm to Grasp The Object


### Requirements

1. ROS : http://wiki.ros.org/
2. Yarp : http://www.yarp.it/
3. iCub : http://wiki.icub.org/


### Install:  
1. `cd ~/catkin_ws/src`   
2. `git clone https://github.com/tunaonur/icub_object_grasping.git
3. `cd ~/catkin_ws`   
4. `catkin_make`
5. `cd ~/catkin_ws/src/icub_object_grasping/yarp`
6. `mkdir build && cd build`
7. `cmake ../`
8. `make`

### Run:  
1. `roscore`
2. `yarp server --ros`  
3. Run simulator: `iCub_SIM`  
4. `simCartesianControl --no_legs`   
5. `iKinCartesianSolver --context simCartesianControl --part right_arm`   
6. `roslaunch openni_launch openni.launch`
7. `roslaunch ar_track_alvar pr2_indiv.launch`
8. `cd ~/catkin_ws/src/icub_object_grasping/yarp/build` and `run ./yarp_pose`
9. `rosrun icub_object_grasping pose_transformer`

