#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>

class ImageSubscriber : public rclcpp::Node
{
    public:
      ImageSubscriber() : Node("image_subscriber")
      {
        sub = this->create_subscription<sensor_msgs::msg::Image>(
            "/camera/camera/image_raw"
           ,rclcpp::SensorDataQoS()
           ,std::bind(&ImageSubscriber::image_callback, this, std::placeholders::_1)
        );
      }

      cv::Mat GetFrame()
      {
        std::lock_guard<std::mutex> lock(imageLock);

        cv::Mat image;

        if (!imageQueue.empty())
        {
          image = imageQueue.front();
          imageQueue.pop();
        }

        return image;
      }

    private:
      rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr sub;
      std::queue<cv::Mat> imageQueue;
      std::mutex imageLock;

      void image_callback(const sensor_msgs::msg::Image::SharedPtr msg)
      {
        //std::cout << "received image data!" << std::endl;
        cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(msg, msg->encoding);

        cv::Mat img = cv_ptr->image;
        
        // create protective lock around pushing data to the queue
        // This prevents the queue from not being able to actually push the data onto the queue
        // If a pop occurs simultaneously

        {
          std::lock_guard<std::mutex> lock(imageLock);
          imageQueue.push(img);
        }

        //cv::imshow("camera", img);
        //cv::waitKey(1);
      }
};