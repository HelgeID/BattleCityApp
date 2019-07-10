#include "check_threads.h"
#include "field.h"


bool ThreadsCheck::isCloseThread() const
{
	auto status = future.wait_for(std::chrono::milliseconds(0));
	return (status == std::future_status::ready);
}

bool ManagerThreads::isThreads(std::vector<ThreadsCheck*>& vecThsCheck)
{
	if (!vecThsCheck.size())
		return false;

	mtx.lock();
	for (auto it = vecThsCheck.begin(); it != vecThsCheck.end(); ++it) {
		if ((*it)->isCloseThread()) {
			vecThsCheck.erase(it);
			break;
		}
	}
	mtx.unlock();

	if (!vecThsCheck.size())
		return false;
	return true;
}

void ManagerThreads::indicateTread(std::vector<ThreadsCheck*>& vecThsCheck, std::thread::id id)
{
	if (!vecThsCheck.size())
		return;

	mtx.lock();
	for (auto it = vecThsCheck.rbegin(); it != vecThsCheck.rend(); ++it) {
		if ((*it)->id == id) {
			(*it)->takePromise().set_value(true);
			break;
		}
	}
	mtx.unlock();

	return;
}

void ManagerThreads::startControlThreads()
{
	std::packaged_task<void()> task([&] {
		bool isThreadsFlag(false);
		while (true) {
			isThreadsFlag = isThreads(vecThsCheck);
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	});
	auto future = task.get_future();

	std::unique_ptr<std::thread> thread(new std::thread(std::move(task)));
	thread->detach();

	return;
}
