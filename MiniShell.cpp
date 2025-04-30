#include <sys/wait.h>
#include <unistd.h>

#include <algorithm>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "readline/history.h"
#include "readline/readline.h"

class Shell {
   private:
    bool running;
    std::string prompt;

    std::vector<std::string> tokenize(const std::string& cmdLine) {
        std::vector<std::string> tokens;
        std::istringstream iss(cmdLine);
        std::string token;
        while (iss >> token) tokens.push_back(token);
        return tokens;
    }

    bool executeBuiltIn(const std::vector<std::string>& tokens) {
        if (tokens.empty()) return true;

        const std::string& command = tokens[0];

        if (command == "exit" || command == "quit") {
            running = false;
            return true;
        } else if (command == "cd") {
            const char* home = getenv("HOME");
            if (tokens.size() == 1) {
                if (home && chdir(home) != 0) {
                    std::cerr << "cd: " << home << ": " << strerror(errno)
                              << std::endl;
                }
            } else {
                if (chdir(tokens[1] == "~" ? home : tokens[1].c_str()) != 0) {
                    std::cerr << "cd: " << tokens[1] << ": " << strerror(errno)
                              << std::endl;
                }
            }
            return true;
        } else if (command == "history") {
            HIST_ENTRY** hist_list = history_list();
            if (hist_list) {
                for (int i = 0; hist_list[i]; i++) {
                    std::cout << i + 1 << ": " << hist_list[i]->line
                              << std::endl;
                }
            }
            return true;
        }

        return false;
    }

    void executeExternal(const std::vector<std::string>& tokens,
                         const std::string& outputFile = "",
                         const std::string& inputFile = "") {
        pid_t pid = fork();

        if (pid == -1) {
            // Fork failed
            std::cerr << "Fork failed: " << strerror(errno) << std::endl;
            return;
        } else if (pid == 0) {
            // Child process
            std::vector<char*> args;
            for (const auto& token : tokens) {
                args.push_back(const_cast<char*>(token.c_str()));
            }
            args.push_back(nullptr);

            FILE* ip_file = fopen(inputFile.c_str(), "r");

            // op_file create & write mode
            FILE* op_file = fopen(outputFile.c_str(), "w");

            if (!inputFile.empty()) {
                if (ip_file) {
                    dup2(fileno(ip_file), STDIN_FILENO);
                    fclose(ip_file);
                } else {
                    std::cerr << "Error opening file: " << strerror(errno)
                              << std::endl;
                }
            }

            if (!outputFile.empty()) {
                if (op_file) {
                    dup2(fileno(op_file), STDOUT_FILENO);
                    fclose(op_file);
                } else {
                    std::cerr << "Error opening file: " << strerror(errno)
                              << std::endl;
                }
            }

            execvp(args[0], args.data());

            // If we get here, execvp failed
            std::cerr << "Command not found: " << tokens[0] << std::endl;
            exit(EXIT_FAILURE);
        } else {
            // Parent process
            int status;
            waitpid(pid, &status, 0);
        }
    }

   public:
    Shell() : running(true), prompt("shell$ ") {
        using_history();
        stifle_history(500);
        read_history(".shell_history");
    }

    ~Shell() { write_history(".shell_history"); }

    void run() {
        std::cout << "Type 'exit' to quit, 'history' to see command history\n";

        char* line;
        while (running && (line = readline(prompt.c_str()))) {
            std::string cmdLine(line);

            // Add to history if not empty
            if (!cmdLine.empty()) {
                add_history(line);

                std::vector<std::string> tokens = tokenize(cmdLine);

                auto outputCh = std::find(tokens.begin(), tokens.end(), ">");
                auto inputCh = std::find(tokens.begin(), tokens.end(), "<");
                // auto pipeCh = std::find(tokens.begin(), tokens.end(), "|");

                std::string inputFile, outputFile;

                if (outputCh != tokens.end() && outputCh + 1 != tokens.end()) {
                    std::cout << "output redirection to: " << *(outputCh + 1)
                              << std::endl;
                    outputFile = *(outputCh + 1);
                    tokens.erase(outputCh, outputCh + 2);
                }

                if (inputCh != tokens.end() && inputCh + 1 != tokens.end()) {
                    std::cout << "input redirection from: " << *(inputCh + 1)
                              << std::endl;
                    inputFile = *(inputCh + 1);
                    tokens.erase(inputCh, inputCh + 2);
                }

                if (!executeBuiltIn(tokens)) {
                    executeExternal(tokens, outputFile, inputFile);
                }
            }

            free(line);
        }

        std::cout << "\nGoodbye!\n";
    }
};

int main() {
    Shell shell;
    shell.run();
    return 0;
}

