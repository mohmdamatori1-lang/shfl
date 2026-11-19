#include <iostream>
#include <sstream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

int main() {
    std::string line;
    while (true) {
        std::cout << "shfl> " << std::flush;
        if (!std::getline(std::cin, line)) {
            std::cout << "\n";
            break;
        }

//      input handling    
        
        std::istringstream iss(line);  
        std::vector<std::string> tokens;
        for (std::string word; iss >> word; ) {
            tokens.push_back(word);
        }


        if (tokens.empty()) continue;

        if (tokens[0] == "exit") break;

        
        std::vector<char*> argv;
        argv.reserve(tokens.size() + 1);
        for (auto& tok : tokens) {
            argv.push_back(tok.data());
        }
        argv.push_back(nullptr);

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            continue;
        }

        if (pid == 0) {
            execvp(argv[0], argv.data());
            perror("shfl");
            _exit(127);
        }

        int status = 0;
        if (waitpid(pid, &status, 0) < 0) {
            perror("waitpid");
        }
    }
    return 0;
}