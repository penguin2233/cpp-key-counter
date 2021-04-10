#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include <chrono>
#include <Windows.h>

int *qpointer, q = 0;
int *epointer, e = 0;
int *rpointer, r = 0;
int *fpointer, f = 0;
bool counting = TRUE;

void count(int key, std::string filename, int* pointer) {
	std::ofstream file;
	file.open(filename);
	while (counting) {
		if (GetAsyncKeyState(key) != 0) {
			++*pointer;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
	file << *pointer;
	file.close();
	return;
}

void print() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO     cursorInfo;
	COORD coord;
	coord.X = 0;
	coord.Y = 1;
	GetConsoleCursorInfo(hConsole, &cursorInfo);
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(hConsole, &cursorInfo);
	while (counting) {
		std::cout << "primary" << q << '\n';
		std::cout << "secondary" << e << '\n';
		std::cout << "melee" << r << '\n';
		std::cout << "pda" << f;
		SetConsoleCursorPosition(hConsole, coord);
	}
	return;
}

void scrolllock() {
	while (counting) {
		if (GetAsyncKeyState(VK_SCROLL) != 0) {
			counting = FALSE;
		}
	}
	return;
}

int main()
{
	std::cout << "cpp-key-counter <3 \n";
	std::ifstream file;
	file.open("primary.txt");
	file >> q;
	file.close();
	file.open("secondary.txt");
	file >> e;
	file.close();
	file.open("melee.txt");
	file >> r;
	file.close();
	file.open("pda.txt");
	file >> f;
	file.close();
	qpointer = &q;
	epointer = &e;
	fpointer = &f;
	rpointer = &r;
	std::thread countThread1(count, 0x51, "primary.txt", std::ref(qpointer));
	std::thread countThread2(count, 0x45, "secondary.txt", std::ref(epointer));
	std::thread countThread3(count, 0x52, "melee.txt", std::ref(rpointer));
	std::thread countThread4(count, 0x46, "pda.txt", std::ref(fpointer));
	std::thread printThread(print);
	std::thread scrolllockThread(scrolllock);
	countThread1.join();
	countThread2.join();
	countThread3.join();
	countThread4.join();
	printThread.join();
	scrolllockThread.join();
	return 0;
}
