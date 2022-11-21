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
void Calculating(std::shared_ptr<Point>, double);
void ClearFiles();

std::mutex mtx;

std::counting_semaphore<1> signalToCalculator(0);
std::counting_semaphore<2> signalToFileWriters(0);

int main()
{ 
    srand(time(NULL));
    unsigned int numberOfOperations;
    double startTime, randX;
    ClearFiles();
    std::cout << "Enter the number of operations: "; std::cin >> numberOfOperations;
    auto start = clock() / 1000.0;
    for (int i = 0; i < numberOfOperations; i++)
    {
        auto point = new Point();
        std::shared_ptr<Point> ptrPoint(point);
        startTime = clock() / 1000.0;
        randX = rand() % 100;
        /*Calculating(ptrPoint, randX);
        WriteLog(startTime, ptrPoint, i);
        WriteResult(ptrPoint, i);*/
        signalToCalculator.release();
        std::thread calculator(Calculating, ptrPoint, randX);
        std::thread log(WriteLog, startTime, ptrPoint, i);
        std::thread write(WriteResult, ptrPoint, i);
        calculator.join();
        log.join();
        write.join();
    }
    std::cout << clock() / 1000.0 - start;
    return 0;
}

void Calculating(std::shared_ptr<Point> point, double randX)
{
    //signalToCalculator.acquire();
    std::cout << "Calculator started work, his id " << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));
    point->SetX(randX);
    point->SetY(point->Func());
    std::cout << "Calculator finished work, his id " << std::this_thread::get_id() << std::endl;
    //signalToFileWriters.release(2);
}

void WriteLog(double startTime, std::shared_ptr<Point> point, int numberOfOperation)
{
    //signalToFileWriters.acquire();
    std::cout << "Logger started work, his id " << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::ofstream fileLogger;
    fileLogger.open("D:\\121702\\Zayats\\laba3\\files\\log.txt", std::ios::app);
    fileLogger << numberOfOperation + 1 << ". \t(" << point->GetX() << ", "
        << point->GetY() << ")" << " Operation start time: "
        << startTime << ", operation end time: " << clock() / 1000.0 << std::endl;
    fileLogger.close();
    std::cout << "Logger finished work, his id " << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void WriteResult(std::shared_ptr<Point> point, int numberOfOperation)
{
    
    //signalToFileWriters.acquire();
    std::cout << "ResultWriter started work, his id " << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::ofstream fileWriter;
    fileWriter.open("D:\\121702\\Zayats\\laba3\\files\\results.txt", std::ios::app);
    fileWriter << numberOfOperation + 1 <<". \tX = " << point -> GetX() << "\tY = " << point->GetY() << std::endl;
    fileWriter.close();
    std::cout << "ResultWriter finished work, his id " << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void ClearFiles()
{
    std::ofstream file;
    file.open("D:\\121702\\Zayats\\laba3\\files\\results.txt");
    file.close();
    file.open("D:\\121702\\Zayats\\laba3\\files\\log.txt");
    file.close();
}