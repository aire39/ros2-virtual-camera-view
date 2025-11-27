#include "ImageSubscriber.hpp"

class ImageSubscriberWrapper
{
    public:
      ImageSubscriberWrapper()
      {
        imageSub = std::make_shared<ImageSubscriber>();
        frameThread = std::thread([this]() {run_task();});
      }

      ~ImageSubscriberWrapper()
      {
        if (frameThread.joinable())
        {
          frameThread.join(); 
        }
      }

      cv::Mat GetFrame()
      {
        return imageSub->GetFrame();
      }

    private:
      std::shared_ptr<ImageSubscriber> imageSub;
      std::thread frameThread;
      
      void run_task()
      {
        rclcpp::spin(imageSub);
      }
};