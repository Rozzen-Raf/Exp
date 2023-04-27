// Exp.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "ThreadPool.h"
#include "Sheduler.h"
#include "ChainableTask.h"
#include <string.h>
#include "EpollWorker.h"
#include <cstddef>
#include "Socket.h"
#include "JsonParser.h"

void signalHandler( int signum ) {
   std::cout << "Interrupt signal (" << signum << ") received.\n";

   // cleanup and close up stuff here  
   // terminate program  

   exit(signum);  
}

int main()
{
    signal(SIGINT, signalHandler);

	
	try{
		ProcessorSharedPtr processor = std::make_shared<TaskProcessorModel<ThreadPool>>(8);
		Sheduler sheduler(processor);

		WorkerBaseSharedPtr worker = std::make_shared<EpollWorker>();
		sheduler.RegisterWorker(worker);

		sheduler.Run();
	}
	catch (const std::exception& ex)
    {
        std::cerr << "Exception: " << ex.what() << '\n';
    }
    catch (...)
    {
        std::cerr << "Unknown exception.\n";
    }
	return 0;
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
