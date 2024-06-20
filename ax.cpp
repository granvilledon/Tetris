#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>

// 初始化计数变量和条件变量
int countA = 0;
int countB = 0;
std::mutex mutexA;
std::mutex mutexB;
std::condition_variable cvA;
std::condition_variable cvB;
std::mutex mutex;

void pedestrianA() {
	std::unique_lock<std::mutex> lockA(mutexA);
	if (countA == 0) {
		mutex.lock();
	}
	countA++;
	std::cout << "A方向行人进入隧道, countA: " << countA << std::endl;
	lockA.unlock();
	cvA.notify_one();
	
	// 模拟通过隧道
	std::this_thread::sleep_for(std::chrono::seconds(1));
	
	lockA.lock();
	countA--;
	std::cout << "A方向行人离开隧道, countA: " << countA << std::endl;
	if (countA == 0) {
		mutex.unlock();
	}
	lockA.unlock();
	cvA.notify_one();
}

void pedestrianB() {
	std::unique_lock<std::mutex> lockB(mutexB);
	if (countB == 0) {
		mutex.lock();
	}
	countB++;
	std::cout << "B方向行人进入隧道, countB: " << countB << std::endl;
	lockB.unlock();
	cvB.notify_one();
	
	// 模拟通过隧道
	std::this_thread::sleep_for(std::chrono::seconds(1));
	
	lockB.lock();
	countB--;
	std::cout << "B方向行人离开隧道, countB: " << countB << std::endl;
	if (countB == 0) {
		mutex.unlock();
	}
	lockB.unlock();
	cvB.notify_one();
}

int main() {
	// 创建线程模拟行人
	std::thread pedestriansA[10];
	std::thread pedestriansB[10];
	
	for (int i = 0; i < 10; ++i) {
		pedestriansA[i] = std::thread(pedestrianA);
		pedestriansB[i] = std::thread(pedestrianB);
	}
	
	for (int i = 0; i < 10; ++i) {
		pedestriansA[i].join();
		pedestriansB[i].join();
	}
	
	return 0;
}

