#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <string>
#include <chrono>
#include <ctime>

#include "CodeFiles/client.h"
#include "CodeFiles/server.h"

typedef std::string string;
typedef std::chrono::system_clock SystemClock;

int main() {
    pid_t pid = fork();  // Create a new process

    if (pid < 0) {
        // If fork() returns a negative value, the creation of the child process was unsuccessful
        std::cerr << "Fork failed" << std::endl;
        return 1;
    } else if (pid == 0) {
        // Child process: Start the server
        StartServer();
        exit(0);  // Ensure the child process exits after running the server
    } else {
        // Parent process: Start the client
        sleep(1); // Give server time to start
        StartClient();

        // Wait for the child process to complete
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            std::cout << "Child process exited with status " << WEXITSTATUS(status) << std::endl;
        } else if (WIFSIGNALED(status)) {
            std::cerr << "Child process was terminated by signal " << WTERMSIG(status) << std::endl;
        } else {
            std::cerr << "Child process did not terminate correctly" << std::endl;
        }
    }
    return 0;
}

typedef struct timestamp
{
    int year;    // 0-based
    int month;   // [1-12]
    int day;     // [1-31]
    int hour;    // [0-23]
    int minute;  // [0-59]
    int sec;     // [0-59]
    int ms;      // [0-999]
}timestamp;

string CurTime(){
    timestamp t;

    auto now = std::chrono::system_clock::now();
    auto seconds = std::chrono::time_point_cast<std::chrono::seconds>(now);
    auto fraction = now - seconds;
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(fraction);

    time_t t_now = std::chrono::system_clock::to_time_t(now);
    tm* ptm = localtime(&t_now);

    t.year = ptm->tm_year + 1900;
    t.month = ptm->tm_mon + 1;
    t.day = ptm->tm_mday;
    t.hour = ptm->tm_hour;
    t.minute = ptm->tm_min;
    t.sec = ptm->tm_sec;
    t.ms = static_cast<int>(milliseconds.count());

    char buffer[100];
    sprintf(buffer, "%d/%d/%d-%d:%d:%d:%d", t.day,t.month,t.year,t.hour,t.minute,t.sec,t.ms);

    return buffer;
}

void Print(bool isServer, string txt){
    string header = (isServer)? "[SERVER]:":"[CLIENT]:";
    header+="[" + (string)CurTime() + "]-> ";
    std::cout << header << txt << std::endl;
}
