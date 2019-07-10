#pragma once

#include <thread>
#include <chrono>
#include <future>
#include <string>
#include <iostream>

class ThreadsCheck
{
	std::promise<bool> promise;
	std::future<bool> future{ promise.get_future() };

public:
	std::thread::id id;
	bool isCloseThread() const;

	std::promise<bool>& takePromise() { return promise; }
	std::future<bool>& takeFuture() { return future; }
};

//control Threads
class ManagerThreads
{
	std::vector<ThreadsCheck*> vecThsCheck;
	
	template<typename Return, class Class>
	using Func = Return(*)(Class, const int);

	bool isThreads(std::vector<ThreadsCheck*>&);
	void indicateTread(std::vector<ThreadsCheck*>&, std::thread::id);

public:
	void startControlThreads(); //todo add to project

	template <typename T>
	void callFuncInNewThread(Func<void, T> func, T obj, const int value = 0)
	{
		std::cerr << "start: " << std::this_thread::get_id() << " " << typeid(obj).name() << std::endl;
		bool condition(false);

		mtx.lock();
		{
			const char* name = typeid(obj).name(); //get the current type obj
			std::string strname(name);

			std::unique_ptr<ThreadsCheck> objThsCheck{ new ThreadsCheck };
			vecThsCheck.push_back(objThsCheck.release()); //add to vector
			auto lastElementThsCheck = vecThsCheck.end() - 1; //take last element of arr
			(*lastElementThsCheck)->id = std::this_thread::get_id(); //get the current ID of this thread

			condition = (strname == "class GameField *" || strname == "class Sound *");
		}
		mtx.unlock();
		
		condition ? func(obj, value) : NULL; //call func

		indicateTread(vecThsCheck, std::this_thread::get_id()); //indicate threads to delete (label - true)

		std::cerr << "end: " << std::this_thread::get_id() << std::endl;
		return;
	}

};
