#include <iostream>
#include <thread>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <vector>
#include <mutex>
#include <windows.h>

using namespace std;
using std::chrono::system_clock;

static const int num_threads = 10;

class myFunctor {
public:
	void operator()() {
		cout << "This is my function object" << endl;
	}
	void publicFunction() {
		cout << "public function of myFunctor class is called" << endl;
	}
};

void threadFunc() {
	cout << "Welcome to Multithreading" << endl;
}

void call_from_thread(int tid) {
	cout << "Launched by thread " << tid << endl;
}

vector<int> vec;

/*
void push() {
	for (int i = 0; i != 10; ++i) {
		cout << "Push " << i << endl;
		Sleep(500);
		vec.push_back(i);
	}
}
void pop() {
	for (int i = 0; i != 10; ++i) {
		if (vec.size() > 0) {
			int val = vec.back();
			vec.pop_back();
			cout << "Pop " << val << endl;
		}
		Sleep(500);
	}
}
*/

std::mutex m;//door handle

void push() {
	m.lock();
	for (int i = 0; i != 10; ++i) {
		cout << "Push " << i << endl;
		Sleep(500);
		vec.push_back(i);
	}
	m.unlock();
}
void pop() {
	m.lock();
	for (int i = 0; i != 10; ++i) {
		if (vec.size() > 0) {
			int val = vec.back();
			vec.pop_back();
			cout << "Pop " << val << endl;
		}
		Sleep(500);
	}
	m.unlock();
}

void makeACall() {
	m.lock();//person enters the phone booth and locks the door
			 //now he can talk to his friend without any interruption
	cout << " Hello my friend, this is " << this_thread::get_id() << endl;
	//this person finished talking to his friend
	m.unlock();//and he leaves the call box and unlocks the door
}

int main() {
	//pass a function to thread
	thread funcTest1(threadFunc);
	//detach funcTest1 from main thread
	funcTest1.detach();
	if (funcTest1.joinable())
		//join the thread with the main thread
		funcTest1.join();
	else
		cout << "functTest1 is detached" << endl;

	myFunctor myFunc;
	//initializing thread with member function of myFunctor class
	thread functorTest(&myFunctor::publicFunction, myFunc);
	if (functorTest.joinable())
		functorTest.join();

	thread t[num_threads];
	//Launch a group of threads
	for (int i = 0; i < num_threads; ++i) {
		t[i] = std::thread(call_from_thread, i);
	}
	cout << "Launched from the main\n";
	//Join the threads with the main thread
	for (int i = 0; i < num_threads; ++i) {
		t[i].join();
	}

	//create 3 different threads
	thread t1(threadFunc);
	thread t2(threadFunc);
	thread t3(threadFunc);
	//get id of all the threads
	thread::id id1 = t1.get_id();
	thread::id id2 = t2.get_id();
	thread::id id3 = t3.get_id();
	//join all the threads
	if (t1.joinable()) {
		t1.join();
		cout << "Thread with id " << id1 << " is terminated" << endl;
	}
	if (t2.joinable()) {
		t2.join();
		cout << "Thread with id " << id2 << " is terminated" << endl;
	}
	if (t3.joinable()) {
		t3.join();
		cout << "Thread with id " << id3 << " is terminated" << endl;
	}

/*
	cout << "The id of current thread is " << this_thread::get_id << endl;
	//sleep while next minute is not reached
	//get current time
	time_t timet = system_clock::to_time_t(system_clock::now());
	//convert it to tm struct
	struct tm * time = localtime(&timet);
	cout << "Current time: " << put_time(time, "%X") << '\n';
	std::cout << "Waiting for the next minute to begin...\n";
	time->tm_min++; time->tm_sec = 0;
	//sleep until next minute is not reached
	this_thread::sleep_until(system_clock::from_time_t(mktime(time)));
	cout << std::put_time(time, "%X") << " reached!\n";
	//sleep for 5 seconds
	this_thread::sleep_for(chrono::seconds(5));
	//get current time
	timet = system_clock::to_time_t(system_clock::now());
	//convert it to tm struct
	time = std::localtime(&timet);
	cout << "Current time: " << put_time(time, "%X") << '\n';
*/

	//create two threads
	thread push(push);
	thread pop(pop);
	if (push.joinable())
		push.join();
	if (pop.joinable())
		pop.join();

	//create 3 people who want to make a call from phone booth
	thread person1(makeACall);
	thread person2(makeACall);
	thread person3(makeACall);
	if (person1.joinable())
		person1.join();
	if (person2.joinable())
		person2.join();
	if (person3.joinable())
		person3.join();
}
