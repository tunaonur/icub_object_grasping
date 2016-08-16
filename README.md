# icub_arm_imitator

#### Following Human Arm in iCub Simulation

(Collision development)

### Requirements

1. ROS : http://wiki.ros.org/
2. Yarp : http://www.yarp.it/
3. iCub : http://wiki.icub.org/


### Install:  
1. `cd ~/catkin_ws/src`   
2. `git clone https://github.com/tkelestemur/icub_arm_imitator.git`   
3. `cd ~/catkin_ws`   
4. `catkin_make`
5. `cd ~/catkin_ws/src/icub_arm_imitator/yarp`
6. `mkdir build && cd build`
7. `cmake ../`
8. `make`

### Run:  
1. `roscore`
2. `yarp server --ros`  
3. Run simulator: `iCub_SIM`  
4. `simCartesianControl --no_legs`   
5. `iKinCartesianSolver --context simCartesianControl --part right_arm`   
5. `cd ~/catkin_ws/src/icub_arm_imitator/yarp/build` and `run ./arm_imitator`

7. To get data from Kinect V2 Server run:   
`rosrun kinect_client joints --ip 144.122.225.164`
and `rosrun kinect_client tf_publisher`
8. `rosrun icub_ros joint_transformer`
9. or directly run `roslaunch icub_arm_imitator arm_imitator.launch`
