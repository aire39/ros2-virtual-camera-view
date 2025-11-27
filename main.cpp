#include <iostream>
#include <csignal>
#include <rclcpp/rclcpp.hpp>
#include "ImageSubscriberWrapper.hpp"

bool isRunning = false;

void handle_sigabrt(int signal) {
    std::cerr << "SIGINT received. Stopping the program...\n";
    isRunning = false;
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    std::signal(SIGINT, handle_sigabrt);
    ImageSubscriberWrapper image_wrapper;
    isRunning = true;

    while(isRunning)
    {
        cv::Mat image_data = image_wrapper.GetFrame();

        if (!image_data.empty())
        {
            // run detector code here...

            std::cout << "M received data!" << std::endl;
        }
    }

    rclcpp::shutdown();

    return 0;
}
