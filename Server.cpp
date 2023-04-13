
#include <opencv2/opencv.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <cstring>
using boost::asio::ip::udp;

int main() {
    try {
        boost::asio::io_context io_context;
        udp::socket socket(io_context, udp::endpoint(udp::v4(), 1234));
        std::cout << "Client running on port " << socket.local_endpoint().port() << std::endl;

        cv::namedWindow("Received Frame", cv::WINDOW_NORMAL);
        cv::resizeWindow("Received Frame", 640, 480);

        while (true) {
            std::vector<uchar> buffer(1024 * 1024); // allocate 1MB buffer for receiving data
            udp::endpoint sender_endpoint;
            size_t length = socket.receive_from(boost::asio::buffer(buffer), sender_endpoint);

            cv::Mat frame = cv::imdecode(buffer, cv::IMREAD_COLOR);
            if (frame.empty()) {
                std::cout << "Error decoding frame" << std::endl;
                continue;
            }

            cv::imshow("Received Frame", frame);
            if (cv::waitKey(1) == 27) { // wait for 1ms and check if 'Esc' key is pressed
                break;
            }
        }

        cv::destroyAllWindows(); // destroy all windows
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
