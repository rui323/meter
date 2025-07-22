#include <opencv2/opencv.hpp>
#include <opencv2/text.hpp>
#include <iostream>
#include <cstdlib>   // std::stof
#include <iomanip>   // std::setprecision

int main(int argc, char *argv[])
{
    std::string path;
    if (argc != 2){
        std::cerr << "画像パスを書いてください\n";
    }
    else {
        path = "../cropped/"+std::string(argv[1]);
    }
	// 画像読み込み
	auto image = cv::imread(path);
	if (image.empty()) {
		std::cerr << "画像が読み込めません\n";
		return -1;
	}

	// グレースケール化
	cv::Mat gray;
	cv::cvtColor(image, gray, cv::COLOR_RGB2GRAY);

	// OCRインスタンス生成（tessdataのパスは環境に応じて修正）
    auto ocr = cv::text::OCRTesseract::create(
        nullptr,              // tessdataパス（環境に合わせて指定可）
        "eng",                // 使用言語
        "01256.-",       // ← ここがホワイトリスト（数字と小数点、マイナス符号）
        cv::text::OEM_CUBE_ONLY, // OCRエンジンモード  cv::text::OEM_TESSERACT_ONLY,cv::text::OEM_CUBE_ONLY,cv::text::OEM_TESSERACT_CUBE_COMBINED,cv::text::OEM_DEFAULT
        6                     // PSMモード（3 = 自動）
    );

	std::string text;
	std::vector<cv::Rect> boxes;
	std::vector<std::string> words;
	std::vector<float> confidences;

	// OCR実行
	ocr->run(gray, text, &boxes, &words, &confidences);

	// 結果出力
	std::cout << "全文テキスト:\n" << text << "\n";
	std::cout << " 文字      | 位置       | 大きさ     | 信頼度   | 数値変換\n";
	std::cout << "-----------+------------+------------+----------+------------\n";

	for (size_t i = 0; i < boxes.size(); i++)
	{
		float number = 0.0;
		bool is_number = false;
		try {
			number = std::stof(words[i]);  // 数字に変換できる場合のみ
			is_number = true;
		}
		catch (...) {
			// 数字変換できない文字列
		}

		printf("%-10s | (%3d, %3d) | (%3d, %3d) | %8.3f | ",
			words[i].c_str(),
			boxes[i].x, boxes[i].y,
			boxes[i].width, boxes[i].height,
			confidences[i]);

		if (is_number)
			printf("%10.3f", number);
		else
			printf("  (非数値)");

		printf("\n");
	}

	return 0;
}
