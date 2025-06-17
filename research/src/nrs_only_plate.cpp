#include <cnoid/SimpleController>
#include <cnoid/Joystick>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <iostream>
#include <cmath>
#include <string>
#include <sstream>

#include <cnoid/SimulatorItem>
#include <cnoid/SceneView>

#include <opencv2/opencv.hpp>
#include <filesystem>

#include <sys/stat.h>
#include <sys/types.h>

namespace fs = std::filesystem;
using namespace cnoid;
class NrsiwataController : public cnoid::SimpleController {
    cnoid::Link *t_link, *b_link, *crawler;
    cnoid::SimpleControllerIO* io;
    cnoid::SimulatorItem* simulator;
    int frameCount;
    double fps;
    std::vector<std::string> imageList;
    std::ofstream ofs1,ofs2;
    double dt;
    cnoid::Joystick joystick;
    // bool isRecording = false;
    // bool flag = false;
    bool isRecording = true;
    bool flag = true;
    // float Velocity =0.001;
    // float Velocity =0.01;
    float Velocity = 0.01; // 0.0 ,0.1, 0.01, 0.001
    std::string mu = "0.3";//  0.3 , 0.5, 1.0
    std::string incident = "75";// 0 , 30 ,60, (15,45,75)
    std::ostringstream oss;
    std::string outputDir; 

    std::string filename_pos,filename_vw;

    std::chrono::high_resolution_clock::time_point start_time;

   public:
    virtual bool initialize( cnoid::SimpleControllerIO* r_io ) override {  
        io = r_io;
        b_link = io->body( )->rootLink( );
        io->enableInput( b_link, cnoid::Link::LinkPosition | cnoid::Link::LinkTwist );
        t_link = io->body( )->link( "Mass" );
        io->enableInput( t_link, cnoid::Link::LinkPosition | cnoid::Link::LinkTwist );
        crawler = io->body()->link("Plate");
        crawler->setActuationMode(cnoid::Link::JOINT_SURFACE_VELOCITY);
        io->enableOutput(crawler);

        start_time = std::chrono::high_resolution_clock::now();

        /*simulator = dynamic_cast<cnoid::SimulatorItem*>(io->body()->parentItem());
        if (!simulator)
        {
            io->os() << "SimulatorItem not found!" << std::endl;
            return false;
        }
        outputDir = "/home/choreonoid_ros2/choreonoid_videos";
        frameCount = 0;
        fps = 30.0; // 動画のフレームレート
        if (!fs::exists(outputDir))
        {
            fs::create_directories(outputDir);
        }*/
        return true;
    }

    virtual bool start( ) override {
        std::cout << "start" << std::endl;
        oss << std::fixed << std::setprecision(3)<<Velocity;
        std::string folder_v = "v_" + oss.str();

        filename_pos = "simulate_model_"+mu+"/"+ "incident_"+ incident+"/" + folder_v + "/position_" + folder_v + ".csv";
        filename_vw = "simulate_model_"+mu+"/" + "incident_"+ incident+"/" + folder_v + "/vw_" + folder_v + ".csv";

        /*ofs1 = std::ofstream( filename_pos );
        ofs2 = std::ofstream( filename_vw );

        if (!ofs1.is_open()) {
            std::cerr << "Error: Could not open file " << filename_pos << std::endl;
        }
        if (!ofs2.is_open()) {
            std::cerr << "Error: Could not open file " << filename_vw << std::endl;
        }

        if (!joystick.isReady()) {
            std::cerr << "Joystick is not ready!" << std::endl;
            return false;
        }*/
        return true;
    }

    virtual bool control( ) override {
        joystick.readCurrentState();
        /*SceneView* sceneView = SceneView::instance();
        if (sceneView)
        {
            auto image = sceneView->image();
            if (image)
            {
                std::string filename = outputDir + "/frame_" + std::to_string(frameCount) + ".png";
                cv::Mat frame(image->height(), image->width(), CV_8UC3, image->pixels());
                cv::cvtColor(frame, frame, cv::COLOR_RGB2BGR);
                cv::imwrite(filename, frame);
                imageList.push_back(filename);
                frameCount++;
            }
        }*/
        // Aボタンが押されたら座標取得、記録、走行を開始
        // if (joystick.getButtonState(0)) {  // ボタン0がAボタンに対応
        //     isRecording = true;
        //     flag = true;
        // }

        // // Bボタンが押されたら停止
        // if (joystick.getButtonState(1)) {  // ボタン1がBボタンに対応
        //     isRecording = false;
        // }
        if (isRecording) {
            // Base link
            cnoid::Isometry3 b_T = b_link->position();
            cnoid::Vector3 b_t = b_T.translation();
            cnoid::Matrix3 b_R = b_T.rotation();
            // Target link
            cnoid::Isometry3 t_T = t_link->position();
            cnoid::Vector3 t_t = t_T.translation();
            cnoid::Matrix3 t_R = t_T.rotation();
            cnoid::Vector3 t_dv = t_link->v();
            cnoid::Vector3 t_dw = t_link->w();

            // // RPYを計算
            // cnoid::Vector3 t_rpy = rotationMatrixToRPY(t_R);

            // 現在の時刻（プログラム開始からの秒数）を計算
            auto now_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(now_time - start_time);
            double current_time = duration.count();  // 経過時間（秒）

           /*
            // データを出力
            ofs1 << std::fixed << std::setprecision(6)
                <<" "<< t_t.x() << "," << t_t.y() << "," << t_t.z()
                << "," << t_R(0,0) << "," << t_R(0,1) << "," << t_R(0,2) 
                << "," << t_R(1,0) << "," << t_R(1,1) << "," << t_R(1,2) 
                << "," << t_R(2,0) << "," << t_R(2,1) << "," << t_R(2,2) 
                << "," << current_time << std::endl;
                
            ofs2 << std::fixed << std::setprecision(6)
                << " " << t_dv.x() << "," << t_dv.y() << "," << t_dv.z() 
                << "," << t_dw.x() << "," << t_dw.y() << "," << t_dw.z() 
                << "," << current_time << std::endl;
            */
            crawler->dq_target() = Velocity;
            
        } else {
            crawler->dq_target() = 0.0;
           
        }
        return true;
    }

    virtual void stop( ) override { 
        /*ofs1.close( ); 
        ofs2.close( );*/
        if (flag == true){
            std::cout << "Saving file: " << std::endl;
            std::cout << filename_pos << std::endl;
            std::cout << filename_vw << std::endl;
        }
        // else std::cout << "Not Saving" <<std::endl;
        /*std::string videoFilename = outputDir + "/simulation_record.mp4";
        createVideo(videoFilename);*/
    }

    /*void createVideo(const std::string& outputPath)
    {
        if (imageList.empty())
        {
            std::cerr << "No images saved." << std::endl;
            return;
        }

        cv::Mat firstImage = cv::imread(imageList[0]);
        int width = firstImage.cols;
        int height = firstImage.rows;

        cv::VideoWriter videoWriter(outputPath, cv::VideoWriter::fourcc('m', 'p', '4', 'v'), fps, cv::Size(width, height));

        for (const auto& imagePath : imageList)
        {
            cv::Mat frame = cv::imread(imagePath);
            if (!frame.empty())
            {
                videoWriter.write(frame);
            }
        }

        videoWriter.release();
        std::cout << "Video saved: " << outputPath << std::endl;
    }*/
};

CNOID_IMPLEMENT_SIMPLE_CONTROLLER_FACTORY( NrsiwataController )