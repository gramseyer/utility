#pragma once

#include "utils/async_worker.h"
#include "utils/threadlocal_cache.h"

#include <array>
#include <string>
#include <vector>

namespace utils
{

namespace detail
{

using log_cache_t = ThreadlocalCache<std::vector<std::string>>;

class AsyncLogWriter : public AsyncWorker
{
	bool has_obj_to_write = false;
	log_cache_t::cache_array_t objects;
	std::string filename;

	bool exists_work_to_do() override final {
		return has_obj_to_write;
	}

	void run()
	{
		while(true) {
			std::unique_lock lock(mtx);

			if ((!done_flag) && (!exists_work_to_do())) {
				cv.wait(
					lock, 
					[this] () {
						return done_flag || exists_work_to_do();
					});
			}

			if (done_flag) break;
			
			write_logs();

			clear();

			cv.notify_all();
		}
	}

	void clear()
	{
		objects = log_cache_t::cache_array_t();
		filename = "";
		has_obj_to_write = false;
	}

	std::vector<std::string> get_unified_logs()
	{
		std::vector<std::string> out;
		for (auto& obj : objects)
		{
			if (obj)
			{
				out.insert(out.end(), obj -> begin(), obj -> end());
			}
		}

		std::sort(out.begin(), out.end());
		return out;
	}

	void write_logs()
	{
		FILE* f = std::fopen(filename.c_str(), "w");

		if (f == nullptr) {
			throw std::runtime_error(std::string("failed to open log file: ") + filename);
		}

		auto logs = get_unified_logs();

		for (auto const& log : logs)
		{
			std::fwrite(log.c_str(), sizeof(unsigned char), log.size(), f);
		}

		std::fflush(f);
		std::fclose(f);
	}

public:

	AsyncLogWriter()
		: AsyncWorker() {
			start_async_thread([this] {run();});
		}

	~AsyncLogWriter() {
		terminate_worker();
	}

	void submit_logs_to_write(log_cache_t::cache_array_t&& _objects, std::string _filename)
	{
		wait_for_async_task();
		std::lock_guard lock(mtx);

		objects = std::move(_objects);
		filename = _filename;
		has_obj_to_write = true;
		cv.notify_all();
	}
};

} // namespace detail

class LogCollector
{
	ThreadlocalCache<std::vector<std::string>> logs;

	detail::AsyncLogWriter writer;

public:

	void log(std::string str)
	{
		logs.get().emplace_back(std::move(str));
	}

	void write_logs(std::string filename)
	{
		writer.submit_logs_to_write(std::move(logs.get_objects()), filename);
		logs.clear();
	}

	void clear()
	{
		logs.clear();
	}
};

}