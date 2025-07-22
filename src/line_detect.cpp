#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/text.hpp>
#include <cmath>
#include <Eigen/Dense>
#include <vector>

void detect_line(std::string path){
    // パスから画像を読み込む
    cv::Mat image = cv::imread(path);
    int width = image.cols;
    int height = image.rows;
    std::cout << "Width: " << width << ", Hegiht: " << height <<std::endl;

    cv::imshow("Normal", image);
    cv::waitKey(0);
    cv::destroyAllWindows();

    // グレースケール化
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    cv::imshow("Gray", gray);
    cv::waitKey(0);
    cv::destroyAllWindows();

    // 二値化
    cv::Mat bit,reverse;
    cv::threshold(gray, bit, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    cv::imshow("Bit", bit);
    cv::waitKey(0);
    cv::destroyAllWindows();

    cv::bitwise_not(bit, reverse);
    cv::imshow("reverse", reverse);
    cv::waitKey(0);
    cv::destroyAllWindows();
    std::vector<cv::Vec3f> circles;
    for(int i = 1; i < 100; i++){
        circles.clear();
        int dp = 2;
        double minDist = height/4;
        double param1 = 400/(i);
        double param2 = param1/2;
        int minRadius = int(height / 4);
        int maxRadius = int(height / 2);

        cv::HoughCircles(reverse, circles, cv::HOUGH_GRADIENT, dp, minDist, param1, param2, minRadius, maxRadius);
        if(!circles.empty()){
            std::cout << i<<", " <<param1 << ", " << param2 << std::endl; 
            break;
        }
    }
    if (!circles.empty()) {
        for (size_t i = 0; i < circles.size(); i++) {
            cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);

            cv::circle(image, center, 3, cv::Scalar(0, 255, 0), -1);
            cv::circle(image, center, radius, cv::Scalar(0, 0, 255), 2);
        }
    }
    cv::imshow("Processed", image);
    cv::waitKey(0);
    cv::destroyAllWindows();
    // // メーターの中心を求める
    // int min_dist = width;
    // std::vector<cv::Vec3f> circles;
    // int param1=120;
    // int param2=15;
    // int minRadius=10;
    // int maxRadius=30;
    // // 円検出
    // cv::HoughCircles(bit, circles, cv::HOUGH_GRADIENT, 1, 20, param1, param2, minRadius, maxRadius);
    // cv::Mat img = image;
    // cv::Point center;
    // int radius;
    // for( size_t i = 0; i < circles.size(); i++ )
    // {
    //     cv::Point circle_center_pos(cvRound(circles[i][0]), cvRound(circles[i][1]));
        
    //     double dist_temp = sqrt(std::pow((cx - circle_center_pos.x),2)+std::pow((cy - circle_center_pos.y),2));
    //     if( min_dist >= dist_temp ){ // 画像中心に一番近い円をメーターの中心とする
    //         radius = cvRound(circles[i][2]);
    //         min_dist = dist_temp;
    //         center = circle_center_pos;
    //     }
    // }

    // // 円の中心を描画します．
    // cv::circle( img, center, 3, cv::Scalar(0,255,0), -1, 8, 0 );
    // // 円を描画します．
    // cv::circle( img, center, radius, cv::Scalar(0,0,255), 3, 8, 0 );
    // cv::imshow("circle", img);
    // cv::waitKey(0);
    // cv::destroyAllWindows();

    // // 線検出
    // int rho = 1;
    // double theta = M_PI/180;
    // double threshold = 0 * int(width / 480);
    // double minLineLength = int(width / 2 * width / 480);
    // double maxLineGap = 10* int(width / 480);
    // std::vector<cv::Vec4f> lines;
    // cv::HoughLinesP(bit, lines, rho, theta, threshold, minLineLength, maxLineGap);
    
    // for( size_t i = 0; i < lines.size(); i++ )
    // {
    //     cv::line( img, cv::Point(lines[i][0], lines[i][1]),
    //         cv::Point(lines[i][2], lines[i][3]), cv::Scalar(0,0,255), 1, 1 );
    // }

    // cv::imshow( "Detected Lines", img );
    // cv::waitKey(0);
    // cv::destroyAllWindows();


    // // メーターの中心から円形にくり抜く
    // int CROP_RADIUS = 173;
    // cv::Mat mask(height, width, CV_8UC1, cv::Scalar(0));
    // cv::circle(mask, cv::Point(int(width/2),int(height/2)), CROP_RADIUS, cv::Scalar(255), cv::FILLED);
    // cv::Mat output(height, width, CV_8UC1, cv::Scalar(255));
    // cv::imshow("mask", mask);
    // cv::waitKey(0);
    // cv::destroyAllWindows();
    // bit.copyTo(output, mask);
    // // 画像の反転
    // cv::Mat reverse;
    // // cv::bitwise_and(bit, bit, reverse, mask);
    // cv::bitwise_not(output, reverse);
    // cv::imshow("mask", reverse);
    // cv::waitKey(0);
    // cv::destroyAllWindows();
    // cv::Mat reverse;
    // cv::bitwise_not(bit, reverse);
    // cv::imshow("mask", reverse);
    // cv::waitKey(0);
    // cv::destroyAllWindows();

    // std::vector<cv::Vec3f> circles;
    // int dp = 2;
    // double minDist = 100;
    // double param1=200;
    // double param2=100;
    // int minRadius=int(height / 3);
    // int maxRadius=int(width / 2);
    // 円検出
    // cv::HoughCircles(reverse, circles, cv::HOUGH_GRADIENT, dp, minDist, param1, param2, minRadius, maxRadius);
    // cv::Mat img = image;
    // for( size_t i = 0; i < circles.size(); i++ )
    // {
    //     cv::Point circle_center_pos(cvRound(circles[i][0]), cvRound(circles[i][1]));
    //     int radius = cvRound(circles[i][2]);
    //     // 円の中心を描画します．
    //     cv::circle( img, circle_center_pos, 3, cv::Scalar(0,255,0), -1, 8, 0 );
    //     // 円を描画します．
    //     cv::circle( img, circle_center_pos, radius, cv::Scalar(0,0,255), 3, 8, 0 );
    //     // double dist_temp = sqrt(std::pow((cx - circle_center_pos.x),2)+std::pow((cy - circle_center_pos.y),2));
    //     // if( min_dist >= dist_temp ){ // 画像中心に一番近い円をメーターの中心とする
    //     //     radius = cvRound(circles[i][2]);
    //     //     min_dist = dist_temp;
    //     //     center = circle_center_pos;
    //     // }
    // }

    
    // cv::imshow("circle", img);
    // cv::waitKey(0);
    // cv::destroyAllWindows();
    // 線検出
    // int rho = 1;
    // double theta = M_PI/180;
    // double threshold = 100;
    // double minLineLength = int(width / 2);
    // double maxLineGap = 10;
    // std::vector<cv::Vec4f> lines;
    // cv::HoughLinesP(reverse, lines, rho, theta, threshold, minLineLength, maxLineGap);
    
    // for( size_t i = 0; i < lines.size(); i++ )
    // {
    //     cv::line( image, cv::Point(lines[i][0], lines[i][1]),
    //         cv::Point(lines[i][2], lines[i][3]), cv::Scalar(0,0,255), 1, 1 );
    // }

    // cv::imshow( "Detected Lines", image );
    // cv::waitKey(0);
    // cv::destroyAllWindows();

    // OCRインスタンス生成（tessdataのパスは環境に応じて修正）
    // auto ocr = cv::text::OCRTesseract::create(
    //     nullptr,              // tessdataパス（環境に合わせて指定可）
    //     "eng",                // 使用言語
    //     "0123456789.-",       // ← ここがホワイトリスト（数字と小数点、マイナス符号）
    //     cv::text::OEM_CUBE_ONLY, // OCRエンジンモード  cv::text::OEM_TESSERACT_ONLY,cv::text::OEM_CUBE_ONLY,cv::text::OEM_TESSERACT_CUBE_COMBINED,cv::text::OEM_DEFAULT
    //     6                     // PSMモード（3 = 自動）
    // );

	// std::string text;
	// std::vector<cv::Rect> boxes;
	// std::vector<std::string> words;
	// std::vector<float> confidences;

	// // OCR実行
	// ocr->run(reverse, text, &boxes, &words, &confidences);

    // // 結果出力
	// std::cout << "全文テキスト:\n" << text << "\n";
	// std::cout << " 文字      | 位置       | 大きさ     | 信頼度   | 数値変換\n";
	// std::cout << "-----------+------------+------------+----------+------------\n";

	// for (size_t i = 0; i < boxes.size(); i++)
	// {
	// 	float number = 0.0;
	// 	bool is_number = false;
	// 	try {
	// 		number = std::stof(words[i]);  // 数字に変換できる場合のみ
	// 		is_number = true;
	// 	}
	// 	catch (...) {
	// 		// 数字変換できない文字列
	// 	}

	// 	printf("%-10s | (%3d, %3d) | (%3d, %3d) | %8.3f | ",
	// 		words[i].c_str(),
	// 		boxes[i].x, boxes[i].y,
	// 		boxes[i].width, boxes[i].height,
	// 		confidences[i]);

	// 	if (is_number)
	// 		printf("%10.3f", number);
	// 	else
	// 		printf("  (非数値)");

	// 	printf("\n");
    //     cv::rectangle(reverse, boxes[i],cv::Scalar(255,0,0),1,8,0);
	// }
    // cv::imshow("mask", reverse);
    // cv::waitKey(0);
    // cv::destroyAllWindows();
    
}

int main(int argc, char *argv[]){
    std::string path;
    if(argc != 2) std::cerr << "please input file path";
    else path = "../cropped/"+std::string(argv[1]);

    std::cout << "Read path: " << path << std::endl;

    detect_line(path);

    return 0;
}