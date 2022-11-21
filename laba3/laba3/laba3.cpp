#include <iostream>
#include <fstream>
#include <thread>
#include <semaphore>
#include <mutex>
#include <memory>
#include <chrono>
#include <ctime>
#include "Point.h"

void WriteResult(std::shared_ptr<Point>, int);
void WriteLog(double, std::shared_ptr<Point>, int);
void ClearFiles();

std::counting_semaphore<2> signalToFileWriters(0);

int main()
{ 
    srand(time(0));
    unsigned int numberOfOperations;
    double startTime;
    ClearFiles();
    std::cout << "Enter the number of operations: "; std::cin >> numberOfOperations;
    auto start = clock() / 1000.0;
    for (int i = 0; i < numberOfOperations; i++)
    {
        //std::cout << "Main thread started work, its id " << std::this_thread::get_id() << std::endl;
        //std::this_thread::sleep_for(std::chrono::seconds(2));
        auto point = new Point();
        std::shared_ptr<Point> ptrPoint(point);
        ptrPoint->SetX(rand() % 100);
        startTime = clock() / 1000.0;
        ptrPoint->SetY(ptrPoint->Func());
        //std::cout << "Main thread finished work, its id " << std::this_thread::get_id() << std::endl;
        //std::this_thread::sleep_for(std::chrono::seconds(1));
        WriteLog(startTime, ptrPoint, i);
        WriteResult(ptrPoint, i);
        signalToFileWriters.release(2);
        std::thread log(WriteLog, startTime, ptrPoint, i);
        std::thread write(WriteResult, ptrPoint, i);
        log.join();
        write.join();
    }
    std::cout << clock() / 1000.0 - start;
    return 0;
}

void WriteResult(std::shared_ptr<Point> point, int numberOfOperation)
{

    signalToFileWriters.acquire();
    //std::cout << "FileWriter thread started work, its id " << std::this_thread::get_id() << std::endl;
    //std::this_thread::sleep_for(std::chrono::seconds(2));
    std::ofstream fileWriter;
    fileWriter.open("D:\\121702\\Zayats\\laba3\\files\\results.txt", std::ios::app);
    fileWriter << numberOfOperation + 1 <<". \tX = " << point -> GetX() << "\tY = " << point->GetY() << std::endl;
    fileWriter.close();
    //std::cout << "FileWriter thread finished work, its id " << std::this_thread::get_id() << std::endl;
    //std::this_thread::sleep_for(std::chrono::seconds(1));

}

void WriteLog(double startTime, std::shared_ptr<Point> point, int numberOfOperation)
{
    signalToFileWriters.acquire();
    //std::cout << "Logger thread started work, its id " << std::this_thread::get_id() << std::endl;
    //std::this_thread::sleep_for(std::chrono::seconds(2));
    std::ofstream fileLogger;
    fileLogger.open("D:\\121702\\Zayats\\laba3\\files\\log.txt", std::ios::app);
    fileLogger << numberOfOperation + 1 << ". \t(" << point->GetX()<< ", "
               << point->GetY() << ")" << " Operation start time: "
               << startTime << ", operation end time: " << clock()/1000.0 << std::endl;
    fileLogger.close();
    //std::cout << "Logger thread started work, its id " << std::this_thread::get_id() << std::endl;
    //std::this_thread::sleep_for(std::chrono::seconds(1));
}

void ClearFiles()
{
    std::ofstream file;
    file.open("D:\\121702\\Zayats\\laba3\\files\\results.txt");
    file.close();
    file.open("D:\\121702\\Zayats\\laba3\\files\\log.txt");
    file.close();
}