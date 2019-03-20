#include <Arduino.h>

#include "servo.h"
#include "filter.h"
#include "vec3.h"
#include "mat33.h"
#include "quat.h"

#include "platform.h"

Platform platform;

#include "ArduinoHardware.h"
#include "ros.h"
#include "geometry_msgs/Pose.h"
#include "std_msgs/Float32MultiArray.h"

ros::NodeHandle nh;

uint32_t loop_duration_us = 0;
void callback(const geometry_msgs::Pose& msg)
{
    uint32_t t_start = micros();

    Vec3 pos = Vec3(msg.position.x, msg.position.y, msg.position.z);
    Quat quat = Quat(msg.orientation.x, msg.orientation.y, msg.orientation.z, msg.orientation.w);
    Mat33 rot;
    rot.setQuat(quat);
    platform.applyTranslationAndRotation(pos, rot);
    loop_duration_us = micros() - t_start;
}

std_msgs::Float32MultiArray arm_msg;
ros::Subscriber<geometry_msgs::Pose> pose_sub("/target_pose", callback);
ros::Publisher arm_length_pub("/arm_lengths", &arm_msg);

uint32_t loop_start_time = 0;
float loop_rate = 100.0;
const uint32_t loop_time_us = 1e6 / loop_rate;

void spin_once()
{
    while (micros() - loop_start_time < loop_time_us);

    loop_start_time = micros();
}

void setup() 
{
    const uint8_t msg_lenth = 7;
	const uint8_t arm_msg_size = sizeof(float);
    
    arm_msg.data = (float*)malloc(arm_msg_size * msg_lenth);
    arm_msg.data_length = msg_lenth;

    nh.getHardware()->setBaud(1000000);
	nh.initNode();
    
    nh.subscribe(pose_sub);
    nh.advertise(arm_length_pub);

    while (!nh.connected())
    {
        nh.spinOnce();
    }

    platform.init_servos();
}

void loop()
{
    uint32_t time_us = micros();
    ros::Time t;
    t.sec = time_us / 1000000UL;
    t.nsec = (time_us * 1000UL) % 1000000000UL;
    
    arm_msg.data[0] = platform.servo_pulse_widths[0];
    arm_msg.data[1] = platform.servo_pulse_widths[1];
    arm_msg.data[2] = platform.servo_pulse_widths[2];
    arm_msg.data[3] = platform.servo_pulse_widths[3];
    arm_msg.data[4] = platform.servo_pulse_widths[4];
    arm_msg.data[5] = platform.servo_pulse_widths[5];
    arm_msg.data[6] = (float)loop_duration_us;
    
    arm_length_pub.publish(&arm_msg);

    nh.spinOnce();
    spin_once();
}