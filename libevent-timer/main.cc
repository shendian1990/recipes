#include "invoke_timer.h"
#include "event_watcher.h"
#include "winmain-inl.h"

#include <event2/event.h>

void Print() {
	std::cout << __FUNCTION__ << " hello world." << std::endl;
}

void Print1() {
	std::cout << __FUNCTION__ << " Print1 world." << std::endl;
}

int main(int argc, const char* argv) {
	struct event_base* base = event_base_new();
	auto timer = recipes::InvokeTimer::Create(base, 1000.0, &Print);
	auto timer1 = recipes::InvokeTimer::Create(base, 2000.0, &Print1);
	timer->Start();
	timer1->Start();
	event_base_dispatch(base);
	event_base_free(base);
	
	return 0;
}