#include <chrono>
#include <iostream>
#include <thread>


int main() {
  std::cout << "SERVER STARTED" << std::endl;

  // Force delay so we KNOW it runs
  std::this_thread::sleep_for(std::chrono::seconds(2));

  std::cout << "ENTERING LOOP..." << std::endl;

  while (true) {
    std::cout << "[SERVER] Running..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
  }

  return 0;
}