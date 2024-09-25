/*Напишите код для примера с рестораном, используя класс для синхронизации двух потоков.
Действия официанта
Уведомить повара о новом заказе.
Дремать в ожидании сигнала от повара.
Принести заказ.
Действия повара
Ждать, когда принесут новый заказ.
Приготовить блюда из заказа.
Нажать на кнопку вызова официанта.
Повар и официант должны иметь общий доступ к:
переменной: orderStatus;
мьютексу, защищающему заказ: std::mutex order;
condition variable: std::condition_variable orderBell;*/
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <Windows.h>
class Restaurant {
public:
    void waiter() {
        while (true) {
            std::unique_lock<std::mutex> lock(orderMutex);


            std::cout << "Официант: Жду заказ..." << std::endl;
            orderBell.wait(lock, [this] { return orderStatus == "готово"; });

            std::cout << "Официант: Принёс заказ!\n" << std::endl;
            orderStatus = "нет заказа";

            lock.unlock();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    void chef() {
        while (true) {
            std::unique_lock<std::mutex> lock(orderMutex);

            std::cout << "Повар: Жду новый заказ..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));


            std::cout << "Повар: Готовлю заказ..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(3));


            orderStatus = "готово";
            std::cout << "Повар: Заказ готов, вызываю официанта!" << std::endl;
            orderBell.notify_one();

            lock.unlock();
        }
    }

private:
    std::mutex orderMutex;
    std::condition_variable orderBell;
    std::string orderStatus = "нет заказа";
};

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    Restaurant restaurant;


    std::thread waiterThread(&Restaurant::waiter, &restaurant);
    std::thread chefThread(&Restaurant::chef, &restaurant);

    // Ожидание завершения потоков (в реальном приложении это может быть бесконечный цикл)
    waiterThread.join();
    chefThread.join();

    return 0;
}

