#include <iostream>
#include <csignal>
#include <rclcpp/rclcpp.hpp>
#include "ImageSubscriberWrapper.hpp"

bool isRunning = false;

void handle_sigint(int signal) {
    std::cerr << "SIGINT received. Stopping the program...\n";
    isRunning = false;
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    std::signal(SIGINT, handle_sigint);
    ImageSubscriberWrapper image_wrapper;
    cv::Mat stopSignTemplate;
    isRunning = true;
    bool loaded = false;

    std::cout << "project root: " << PROJECT_ROOT << std::endl;

    while(isRunning)
    {
        /*
        ImageSubscriberWrapper::GetFrame()
        ----------
        This version adds STOP SIGN DETECTION to the incoming frame.

        Steps:
            1. Get frame from ImageSubscriber (OpenCV only)
            2. Load stop sign template (stop.jpeg)
            3. Perform template matching
            4. Draw bounding box + label
            5. Save detected frame to PNG
        */

        //1. Get frame from ImageSubscriber (OpenCV only)
        cv::Mat frame = image_wrapper.GetFrame();

        if (!frame.empty())
        {
            // 2. Load stop sign template once
            if (!loaded)
            {
                stopSignTemplate = cv::imread(PROJECT_ROOT + std::string("/object_detection/stop.jpeg"), cv::IMREAD_COLOR);

                if (stopSignTemplate.empty())
                {
                    std::cerr << "[StopDetector] ERROR: stop.jpeg failed to load.\n";
                    std::cerr << PROJECT_ROOT + std::string("/object_detection/stop.jpeg") << std::endl;
                }
                else
                {
                    std::cout << "[StopDetector] stop.jpeg loaded.\n";
                    loaded = true;
                }
            }

            if (stopSignTemplate.empty())
            {
                std::cerr << "No template — nothing to match!" << std::endl;
                continue;
            }

            // 3. Make sure template fits the frame
            if (stopSignTemplate.cols > frame.cols || stopSignTemplate.rows > frame.rows)
            {
                double sx = (double)frame.cols / stopSignTemplate.cols;
                double sy = (double)frame.rows / stopSignTemplate.rows;
                double scale = std::min(sx, sy) * 0.5;

                if (scale > 0)
                {
                    cv::resize(stopSignTemplate, stopSignTemplate, cv::Size(), scale, scale);
                }
            }

            // 4. Template match
            cv::Mat result;
            cv::matchTemplate(frame, stopSignTemplate, result, cv::TM_CCOEFF_NORMED);

            double minVal, maxVal;
            cv::Point minLoc, maxLoc;
            cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);

            double threshold = 0.65;

            if (maxVal > threshold)
            {
                // Draw detection box
                cv::Rect box(maxLoc.x, maxLoc.y,
                            stopSignTemplate.cols, stopSignTemplate.rows);

                // Clip to frame
                box &= cv::Rect(0, 0, frame.cols, frame.rows);

                cv::rectangle(frame, box, cv::Scalar(0,0,255), 3);

                cv::putText(frame, "STOP SIGN",
                            cv::Point(box.x, box.y - 10),
                            cv::FONT_HERSHEY_SIMPLEX,
                            0.9, cv::Scalar(0,0,255), 2);

                // 5. Save image for assignment proof
                static int id = 0;
                std::string out = PROJECT_ROOT + std::string("/object_detection/detected_") + std::to_string(id++) + ".png";

                if (cv::imwrite(out, frame))
                {
                    std::cout << "[StopDetector] Saved detection: " << out << "\n";
                }
                else
                {
                    std::cerr << "[StopDetector] Failed to save " << out << "\n";
                }
            }

        }
    }

    rclcpp::shutdown();

    return 0;
}
