
#include <opencv2/opencv.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <cstring>
using boost::asio::ip::udp;


int main() {
    try {
        boost::asio::io_context io_context;
        // socket.open(udp::v4());

        // Increase the send buffer size to 1 MB
        int send_buffer_size = 1048576; // 1 MB
        //socket.set_option(socket_base::send_buffer_size(send_buffer_size));

        cv::VideoCapture cap(0); // open default camera
        if (!cap.isOpened()) { // check if camera is successfully opened
            std::cout << "Error opening camera" << std::endl;
            return -1;
        }


        cv::namedWindow("Camera Feed", cv::WINDOW_NORMAL); // create a window to display camera feed
        cv::resizeWindow("Camera Feed", 640, 480); // resize window to 640x480

        udp::socket socket(io_context, udp::endpoint(udp::v4(), 1235));
        std::cout << "Check";
        std::cout << "server running on port" << socket.local_endpoint().port() << std::endl;

        while (true) {
            cv::Mat frame;
            cap >> frame; // get a new frame from camera
            if (frame.empty()) { // check if frame is empty
                std::cout << "Error capturing frame" << std::endl;
                break;
            }
            cv::resize(frame, frame, cv::Size(600, 400));
            cv::imshow("Camera Feed", frame); // display frame in window
            std::cout << "Check";
            if (cv::waitKey(1) == 27) { // wait for 1ms and check if 'Esc' key is pressed
                break;
            }
            std::vector<uchar> buffer;
            cv::imencode(".jpg", frame, buffer);
            size_t size = sizeof(buffer);

            std::cout << "Size of buffer: " << size << " bytes" << std::endl;
            udp::endpoint remote_endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 1234);
            socket.send_to(boost::asio::buffer(buffer), remote_endpoint);

            boost::asio::steady_timer timer(io_context, boost::asio::chrono::milliseconds(50));
            timer.wait();


        }

        cap.release(); // release camera
        cv::destroyAllWindows(); // destroy all windows
    }
    catch (std::exception& e) {
        std::cerr << "Exception" << e.what() << std::endl;
    }

    return 0;
}