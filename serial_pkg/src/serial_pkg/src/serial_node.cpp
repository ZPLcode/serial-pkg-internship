#include <ros/ros.h>
#include <serial/serial.h>
#include <iostream>

using namespace std;
int duty_cycle;
uint8_t motor_dir,light_state;

int serial_write(serial::Serial &ser, std::string &serial_msg)
{
    ser.write(serial_msg);
    return 0;
}

int serial_read(serial::Serial &ser, std::string &result)
{
    result = ser.read(ser.available());
    return 0;
}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "serial_node");
    ros::NodeHandle nh;
    ros::Rate loop_rate(60);
    serial::Serial ser;
    static uint8_t send_buf[6];
    try
    {
        ser.setPort("/dev/ttyUSB0");
        ser.setBaudrate(115200);
        serial::Timeout to = serial::Timeout::simpleTimeout(1000);
        ser.setTimeout(to);
        ser.open();
    }
    catch (serial::IOException &e)
    {
        ROS_ERROR_STREAM("Unable to open port n");
        return -1;
    }

    if (ser.isOpen())
    {
        ROS_INFO_STREAM("Serial Port initialized.\n");
    }
    else
    {
        return -1;
    }
    //data package
    duty_cycle=280;
    motor_dir=1;
    light_state=0;
    send_buf[0]=0xA5;
    send_buf[1]=motor_dir;
    send_buf[2]=light_state;
    send_buf[3]=duty_cycle>>8;
    send_buf[4]=duty_cycle;
    send_buf[5]=0X7B;
    //data package
    while (ros::ok())
    {
       
        //串口写数据
        ser.write(send_buf, sizeof(send_buf));
        ROS_INFO("send: motor_dir= %d,light_state=%d,duty_cycle=%d",send_buf[1],send_buf[2],duty_cycle);
        loop_rate.sleep();
    }

    ser.close();
    return 0;
}
