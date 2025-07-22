#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/text.hpp>
#include <cmath>
#include <Eigen/Dense>
#include <vector>
#include <fstream>
#include <thread>
#include <chrono>
// 検出処理をフレームごとに適用
void process_frame(cv::Mat& frame, int& image_count, std::ofstream& csv) {
    if (frame.empty()) return;

    int width = frame.cols;
    int height = frame.rows;

    cv::Mat gray, bit, reverse;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    cv::threshold(gray, bit, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);
    cv::bitwise_not(bit, reverse);

    std::vector<cv::Vec3f> circles;
    int dp = 2;
    double minDist = width/2;
    double param1 = 200;
    double param2 = param1/2;
    int minRadius = int(width / 3);
    int maxRadius = int(width / 2);

    cv::HoughCircles(reverse, circles, cv::HOUGH_GRADIENT, dp, minDist, param1, param2, minRadius, maxRadius);

    if (!circles.empty()) {
        for (size_t i = 0; i < circles.size(); i++) {
            cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);

            cv::circle(frame, center, 3, cv::Scalar(0, 255, 0), -1);
            cv::circle(frame, center, radius, cv::Scalar(0, 0, 255), 2);

            // // ファイル名生成＆保存
            // char filename[64];
            // std::snprintf(filename, sizeof(filename), "../results/output_%03d.jpg", image_count);
            // cv::imwrite(filename, frame);

            // // CSVへ記録
            // csv << filename << "," << center.x << "," << center.y << "," << radius << "\n";

            image_count++;  // 次のファイル名へ
        }
    }

    cv::imshow("Processed", frame);
}


int main() {
    cv::VideoCapture cap("/dev/video4");
    if (!cap.isOpened()) {
        std::cerr << "カメラが開けませんでした。" << std::endl;
        return -1;
    }

    std::ofstream csv("result.csv");
    if (!csv.is_open()) {
        std::cerr << "CSVファイルが開けませんでした。" << std::endl;
        return -1;
    }
    csv << "filename,center_x,center_y,radius\n";

    std::cout << "カメラ映像処理を開始します（ESCキーで終了）" << std::endl;

    int image_count = 0;
    cv::Mat frame;
    
    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        process_frame(frame, image_count, csv);
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 検出時だけスリープ
        if (cv::waitKey(30) == 27) break;  // ESCキー
    }

    cap.release();
    cv::destroyAllWindows();
    csv.close();
    return 0;
}
