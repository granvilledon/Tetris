#include <semaphore.h>
#include <pthread.h>
#include <iostream>

sem_t AB; // 用于方向A到B的信号量
sem_t BA; // 用于方向B到A的信号量
sem_t mutex; // 用于互斥的信号量

void* A_direction(void* arg) {
	sem_wait(&AB); // P(AB)
	sem_wait(&mutex); // P(mutex)
	std::cout << "A方向的行人正在通过隧道..." << std::endl;
	// 通过隧道...
	sem_post(&mutex); // V(mutex)
	sem_post(&BA); // V(BA)
}

void* B_direction(void* arg) {
	sem_wait(&BA); // P(BA)
	sem_wait(&mutex); // P(mutex)
	std::cout << "B方向的行人正在通过隧道..." << std::endl;
	// 通过隧道...
	sem_post(&mutex); // V(mutex)
	sem_post(&AB); // V(AB)
}

int main() {
	sem_init(&AB, 0, 1); // 初始化信号量AB
	sem_init(&BA, 0, 1); // 初始化信号量BA
	sem_init(&mutex, 0, 1); // 初始化互斥信号量
	
	const int NUM_PEOPLE = 1000;
	pthread_t thA[NUM_PEOPLE], thB[NUM_PEOPLE];
	
	for(int i = 0; i < NUM_PEOPLE; ++i) {
		pthread_create(&thA[i], NULL, A_direction, NULL);
		pthread_create(&thB[i], NULL, B_direction, NULL);
	}
	
	for(int i = 0; i < NUM_PEOPLE; ++i) {
		pthread_join(thA[i], NULL);
		pthread_join(thB[i], NULL);
	}
	
	sem_destroy(&AB);
	sem_destroy(&BA);
	sem_destroy(&mutex);
	
	return 0;
}


