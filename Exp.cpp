// Exp.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "ThreadPool.h"
#include "Sheduler.h"
#include "ChainableTask.h"
static uint32_t start_time = 0;
CoroTaskVoid getprintInt1()
{
	static int x = 0;
	if (!start_time)
		start_time = clock();

	uint32_t end_time = clock();
	uint32_t dif = end_time - start_time;
	std::cout << x++ << " Co "<< dif << std::endl;
	co_return;
}

CoroTaskVoid getprintInt2()
{
	co_await getprintInt1();
	static int x = 0;
	if (!start_time)
		start_time = clock();

	uint32_t end_time = clock();
	uint32_t dif = end_time - start_time;
	std::cout << x++ << "  !" << dif << std::endl;
}

int main()
{
	ProcessorSharedPtr processor = std::make_shared<TaskProcessorModel<ThreadPool>>(std::make_unique<ThreadPool>(8));
	Sheduler sheduler;

	while (true)
	{
		Task t(std::move(getprintInt2()), processor);
		sheduler.CoroStart(std::move(t));

		std::this_thread::sleep_for(std::chrono::microseconds(1));
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
