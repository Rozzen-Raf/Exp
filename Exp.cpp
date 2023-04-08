﻿// Exp.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "ThreadPool.h"
#include "Sheduler.h"
#include "ChainableTask.h"
#include <string.h>
#include "EpollWorker.h"
static uint32_t start_time = 0;

CoroTaskVoid async_accept(ID_t socket_fd, Sheduler& sh)
{
	int ret;
	std::cout << "async_accept start" << std::endl;
	while((ret = accept4(socket_fd, nullptr, nullptr, SOCK_NONBLOCK)) == -1)
	{
		auto status = co_await sh.event(SELECT, socket_fd);
	}
	std::cout << ret << std::endl;
	shutdown(ret,2);
	close(ret);
}

CoroTaskVoid async_server(ID_t socket_fd, Sheduler& sh)
{
	while(true)
	{
		co_await async_accept(socket_fd, sh);
	}
	co_return;
}

int main()
{
	ProcessorSharedPtr processor = std::make_shared<TaskProcessorModel<ThreadPool>>(8);

	Sheduler sheduler(processor);

	WorkerBaseSharedPtr worker = std::make_shared<EpollWorker>();
	sheduler.RegisterWorker(worker);

	int fd_ = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);

	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_addr.s_addr = inet_addr("192.168.0.104");
	addr.sin_port = htons(11111);
	addr.sin_family = AF_INET;

	if(bind(fd_, (sockaddr*)&addr, sizeof(addr)) == -1)
	{
		shutdown(fd_, 2);
		close(fd_);
		std::cout << "bind error" << std::endl;
		return 0;
	}

	if(listen(fd_, SOMAXCONN) != 0)
	{
		shutdown(fd_, 2);
		close(fd_);
		std::cout << "listen error" << std::endl;
		return 0;
	}
	EpollRegister(fd_, worker->GetID());
	sheduler.CoroStart(async_server(fd_, sheduler));
	sheduler.Run();
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10000));
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
