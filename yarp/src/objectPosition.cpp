// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include <yarp/os/all.h>
#include <yarp/dev/ControlBoardInterfaces.h>
#include <yarp/dev/CartesianControl.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/sig/Vector.h>

#include "../include/visualization_msgs_Marker.h"
#include "../include/ar_track_alvar_msgs/AlvarMarker.h" // not sure about this line

#include <stdio.h>
#include <string>
#include <iostream>
#include <gsl/gsl_math.h>



using namespace yarp::os;
using namespace yarp::dev;
using namespace yarp::sig;

using namespace std;

class ControlThread: public RateThread
{

    PolyDriver dd;
    ICartesianControl *icart;

    Vector cRot, cOrt; // current rotation and orientation of robot's joint
    Vector dRot, dOrt; // desired rotation and orientation of robot's joint


    // ROS variables
    yarp::os::Node *node;
    yarp::os::Subscriber<ar_track_alvar_msgs::AlvarMarker> poseSub; //not sure about this line
  //  yarp::os::Publisher<visualization_msgs_Marker> markerPub;
    ar_track_alvar_msgs::AlvarMarker *objectPose; // not sure about this line
  //  visualization_msgs_Marker collisionMarker_1;
    // visualization_msgs_Marker *collisionMarker_1;

public:
    ControlThread(int period):RateThread(period){}

    bool threadInit()
    {
        //initialize here variables
        printf("ControlThread:starting\n");

        Property options("(device cartesiancontrollerclient)");
        options.put("remote","/icubSim/cartesianController/right_arm");
        options.put("local","/cartesian_client/right_arm");

        dd.open(options);

        if (dd.isValid()) {
           dd.view(icart);
           if (!icart){
              return false;
           }
        }


        // get the torso dofs
        Vector newDof, curDof;
        icart->getDOF(curDof);
        newDof=curDof;

        // disable the torso
        newDof[0]=0;
        newDof[1]=0;
        newDof[2]=0;

        // impose some restriction on the torso pitch
        limitTorsoPitch();
        icart->setTrackingMode(true);
        // send the request for dofs reconfiguration
        icart->setDOF(newDof,curDof);

        // icart->setTrajTime(1.0);

        dRot.resize(3);
        dOrt.resize(4);

        // ROS initialization
        node = new yarp::os::Node("/ar_tag/objectPose");
        if (!poseSub.topic("ar_pose_marker")) {
          cerr<< "Failed to subscriber to /ar_pose_marker\n";
          return -1;
          }
  //      if (!markerPub.topic("/icub/collision_markers")) {
    //      cerr<< "Failed to advertise to /icub/collision_markers\n";
      //    return -1;
        //  }


        return true;
    }

    void threadRelease()
    {
        printf("ControlThread:stopping the robot\n");
        poseSub.close();

        icart->stopControl();
        dd.close();

        printf("Done, goodbye from ControlThread\n");
    }

    void run()
    {
      //double tf = 0.60;
      dOrt = 0;

    //  double red = 0.0;
    //  double green = 0.0;

      objectPose = poseSub.read(false);

      if (!objectPose) {
        cout << "object rotation (xyz): no data yet!" << endl;
      }

      else if (objectPose) {
        dRot[0] = objectPose->pose.pose.position.z;
        dRot[1] = objectPose->pose.pose.position.x;
        dRot[2] = objectPose->pose.pose.position.y;
        // cout << jointPose->position.z << " x " << tf << " = "<< dRot[0] << endl;
        // dOrt[0] = jointPose->orientation.z;
        // dOrt[1] = jointPose->orientation.x;
        // dOrt[2] = jointPose->orientation.y;
        // dOrt[3] = jointPose->orientation.w;
        cout << "objectPose dRotation (xyz)[m]: " << dRot.toString().c_str() << endl;
        icart->goToPose(dRot,dOrt);

        icart->getPose(cRot, cOrt);
        cout << "robot palm rotation (xyz)[m]: " << cRot.toString().c_str() << endl;
      }
    //    Vector xdhat, odhat, qdhat;
    //    icart->askForPosition(dRot, xdhat, odhat, qdhat);
    //    std::cout << "                         xdhat:" << xdhat.toString().c_str() << std::endl;
      //  if (-xdhat[0] < 0.14 && xdhat[1] < 0.20) {
        //  // icart->stopControl();
        //  std::cout << "alert: collision detected!" << std::endl;
        //  red = 1.0;
      //    green = 0.0;


        // }
        // else {
        //   icart->goToPose(dRot,dOrt);
        //   red = 0.0;
        //   green = 1.0;
        // }


        // icart->getPose(cRot,cOrt);
        // cout << "robot palm cRotation (xyz)[m]: " << cRot.toString().c_str() << endl;
      }

      // icart->getPose(cRot,cOrt);
      // cout << "robot palm cRotation (xyz)[m]: " << cRot.toString().c_str() << endl;
      // publish collision markers


    //   collisionMarker_1.header.frame_id = "world";
    //   struct timespec currentTime;
    //   clock_gettime(CLOCK_REALTIME, &currentTime);
    //
    //   collisionMarker_1.header.stamp.sec = currentTime.tv_sec;
    //   collisionMarker_1.header.stamp.nsec = currentTime.tv_nsec;
    //   collisionMarker_1.ns = "collision_marker_1";
    //   collisionMarker_1.id = 0;
    //   collisionMarker_1.type = 3;
    //   collisionMarker_1.action = 0;
    //   collisionMarker_1.pose.position.x = 0;
    //   collisionMarker_1.pose.position.y = 0;
    //   collisionMarker_1.pose.position.z = 0.7;
    //   collisionMarker_1.pose.orientation.x = 0.0;
    //   collisionMarker_1.pose.orientation.y = 0.0;
    //   collisionMarker_1.pose.orientation.z = 0.0;
    //   collisionMarker_1.pose.orientation.w = 1.0;
    //   collisionMarker_1.scale.x = 0.25;
    //   collisionMarker_1.scale.y = 0.35;
    //   collisionMarker_1.scale.z = 0.005;
    //   collisionMarker_1.color.a = 1.0; // Don't forget to set the alpha!
    //   collisionMarker_1.color.r = red;
    //   collisionMarker_1.color.g = green;
    //   collisionMarker_1.color.b = 0.0;
    //
    //
    //   markerPub.write(collisionMarker_1);
    //
    //
    // }

    void limitTorsoPitch()
    {
        int axis=0; // pitch joint
        double min, max;
        int MAX_TORSO_PITCH = 30.0;
        // sometimes it may be helpful to reduce
        // the range of variability of the joints;
        // for example here we don't want the torso
        // to lean out more than 30 degrees forward

        // we keep the lower limit
        icart->getLimits(axis,&min,&max);
        icart->setLimits(axis,min,MAX_TORSO_PITCH);
    }


};

int main(int argc, char *argv[])
{
    Network yarp;

    if (!yarp.checkNetwork())
    {
        printf("No yarp network, quitting\n");
        return 1;
    }

    ControlThread ctrlThread(700);

    ctrlThread.start();
    int RUN_TIME = 3600; // seconds
    bool done=false;
    double startTime=Time::now();
    while(!done)
    {
        if ((Time::now()-startTime)>RUN_TIME)
            done=true;
    }

    ctrlThread.stop();

    return 0;
}
