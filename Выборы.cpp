#include <iostream>
#include <semaphore>
#include <thread>
#include "windows.h"

// Семафор для голосов первого и второго кандидата
std::binary_semaphore sem1{ 1 }, sem2{ 1 };

void first_candidate(int * votes_for_first)
{
    // Увеличение числа голосов
    while (true)
    {
        sem1.acquire();
        (*votes_for_first)++;
        sem1.release();

        Sleep(std::rand()/1000);
    }
}

void second_candidate(int* votes_for_second)
{
    // Увеличение числа голосов
    while (true)
    {
        sem2.acquire();
        (*votes_for_second)++;
        sem2.release();

        Sleep(std::rand() / 500);
    }
}

int main()
{
    setlocale(LC_ALL, "RUSSIAN");

    int total_count = 0;
    int votes_for_first = 0;
    int votes_for_second = 0;

    // Запуск подсчета подписей
    std::thread thrWorker1(first_candidate, &votes_for_first);
    std::thread thrWorker2(second_candidate, &votes_for_second);

    Sleep(1000);

    // Обновление процентажа голосов
    while (true)
    {
        sem1.acquire();
        sem2.acquire();

        total_count = votes_for_first + votes_for_second;
        std::cout << "\n\n Общее число голосов: " << total_count << std::endl;
        std::cout << "Голосов за первого кандидата: " << votes_for_first / float(total_count) * 100 << "%(" << votes_for_first << ")\n";
        std::cout << "Голосов за второго кандидата: " << votes_for_second / float(total_count) * 100 << "%(" << votes_for_second << ")\n";

        sem1.release();
        sem2.release();

        Sleep(1500);
    }

}
