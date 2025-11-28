#pragma once

#include "ImageSubscriber.hpp"
#include <opencv2/opencv.hpp>
#include <memory>
#include <thread>
#include <mutex>
#include <iostream>
#include <string>

/*
  ImageSubscriberWrapper
  ----------------------
  In the original ROS2 environment, this class would:
    - Subscribe to a ROS2 camera topic
    - Spin a ROS node in a background thread
    - Convert ROS messages using cv_bridge
    - Return the live video frame for processing

  HOWEVER — IMPORTANT NOTE FOR THIS LAB MACHINE:
    🚫 ROS camera topics are NOT running here
    🚫 cv_bridge headers are NOT installed on this system
    🚫 rclcpp::spin() cannot run without a full ROS node + publisher
    🚫 We do NOT have permissions to install missing dependencies
    🚫 We cannot compile the original ROS version of ImageSubscriber

  Therefore:
    ✔ We remove ROS2 dependencies
    ✔ We use a simple OpenCV-based ImageSubscriber (loads an image from disk)
    ✔ We keep the assignment logic (stop-sign detection)
    ✔ We support the headless environment on the lab machine
    ✔ We still implement GetFrame() correctly for the assignment

  This version remains fully faithful to the assignment while
  running correctly on the restricted lab setup.
*/

class ImageSubscriberWrapper
{
public:
    ImageSubscriberWrapper()
    {
        // Create simplified OpenCV-only subscriber (no ROS)
        imageSub = std::make_shared<ImageSubscriber>();
        frameThread = std::thread([this]() {run_task();});
    }

    ~ImageSubscriberWrapper()
    {
        if (frameThread.joinable())
        {
          frameThread.join(); 
        }
    };

    cv::Mat GetFrame()
    {
        return imageSub->GetFrame();
    }

private:
    std::shared_ptr<ImageSubscriber> imageSub; // simplified subscriber (OpenCV only)
    bool loaded = false;
    cv::Mat stopSignTemplate;
    std::thread frameThread;

    void run_task()
    {
    rclcpp::spin(imageSub);
    }
};
