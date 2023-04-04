// Exp.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "ThreadPool.h"
#include "Sheduler.h"
#include "ChainableTask.h"
CoroTaskVoid getprintInt(int x)
{
	std::cout << x << std::endl;
	co_return;
}

int main()
{
	ProcessorSharedPtr processor = std::make_shared<TaskProcessorModel<ThreadPool>>(std::make_unique<ThreadPool>(8));
	Sheduler sheduler;

	Task* t = new Task(std::move(getprintInt(5)), processor);
	sheduler.CoroStart(std::move(*t));

	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
