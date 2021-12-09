#pragma once

#include <chrono>
#include <ctime>


class Timer {
public:
	void start() {
		m_Starttime = std::chrono::steady_clock::now();
		m_Running = true;
	}


	void end() {
		m_Endtime = std::chrono::steady_clock::now();
		m_Running = false;
	}

	double elapsed() {
		if (m_Running) {
			decltype(m_Endtime) end_time = std::chrono::steady_clock::now();
			return std::chrono::duration_cast<std::chrono::milliseconds>(end_time - m_Starttime).count();
		} else {
			return std::chrono::duration_cast<std::chrono::milliseconds>(m_Endtime - m_Starttime).count();
		}
	}


private:
	std::chrono::time_point<std::chrono::steady_clock> m_Starttime;
	std::chrono::time_point<std::chrono::steady_clock> m_Endtime;
	bool m_Running = false;
};