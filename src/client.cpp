/** @file client.cpp
 *  @version 3.1.8
 *  @date July 29th, 2016
 *
 *  @brief
 *  All the exampls for ROS are implemented here. 
 *
 *  @copyright 2016 DJI. All rights reserved.
 *
 */


#include <ros/ros.h>
#include <stdio.h>
#include <dji_sdk/dji_drone.h>
#include <cstdlib>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>

using namespace DJI::onboardSDK;

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "sdk_client");
    ROS_INFO("locate_ustc_client");
    ros::NodeHandle nh;
    DJIDrone *drone = new DJIDrone(nh);

    //virtual RC test data
    uint32_t virtual_rc_data[16];
    // request contral
    ros::spinOnce();

    drone->request_sdk_permission_control();
    //take off
    ros::spinOnce();

    drone->takeoff();

    ros::spinOnce();
    //initialize
    virtual_rc_data[0] = 1024; //0-> roll     	[1024-660,1024+660]
    virtual_rc_data[1] = 1024; //1-> pitch    	[1024-660,1024+660]
    virtual_rc_data[2] = 1024; //2-> throttle 	[1024-660,1024+660]
    virtual_rc_data[3] = 1024; //3-> yaw      	[1024-660,1024+660]
    virtual_rc_data[4] = 1684; //4-> gear		{1684(UP), 1324(DOWN)}
    virtual_rc_data[6] = 1024; //6-> mode     	{1552(P), 1024(A), 496(F)}

    while (1)
    {
        //[2] -> Z
        ros::spinOnce();
        //read input
        std::cout << "input gear\n";
        int mode = 0;
        std::cin >> mode;
        if (mode != 0)
        {
            virtual_rc_data[1] = 1024 + 600;
        }
        else
        {
            virtual_rc_data[1] = 1024 - 600;
        }
        //virtual rc begin
        drone->virtual_rc_enable();
        usleep(20000);

        for (int i = 0; i < 100; i++)
        {
            drone->virtual_rc_control(virtual_rc_data);
            usleep(20000);
        }

        //RC end
        drone->virtual_rc_disable();
    }
    drone->release_sdk_permission_control();
    return 0;
}
