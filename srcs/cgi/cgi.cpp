
#include "cgi.hpp"
#include <stdlib.h>

Cgi::Cgi(std::string file,std::map<std::string, std::string> bin_path, char **envp) : envp(envp), _bin_path(bin_path), _file(file)
{
    std::string path = file.substr(file.find_last_of(".") + 1);
    std::cout << "path: " << path << std::endl;
    for (std::map<std::string, std::string>::iterator it = bin_path.begin(); it != bin_path.end(); it++)
    {
        if (it->first == path)
        {
            std::cout << "found path: " << it->second << std::endl;
            this->_bin = it->second;
            return ;
        }
    }
    this->_bin = "none";
}

Cgi::~Cgi()
{

}

void Cgi::exec_cgi()
{
    // execute _bin with _file in a child process using a pipe

    int fd[2];
    pipe(fd);
    int pid = fork();
    char **args = new char*[3];

    args[0] = (char*)this->_bin.c_str();
    args[1] = (char *)this->_file.c_str();
    args[2] = NULL;

    int save_stdout = dup(1);
    int save_stdin = dup(0);

    if (pid == 0)
    {
        dup2(fd[1], 1);
        close(fd[0]);
        close(fd[1]);
        execve(this->_bin.c_str(), args, this->envp);
    }
    else
    {
        dup2(fd[0], 0);
        close(fd[0]);
        close(fd[1]);
        char buf[1000];
        int ret = read(0, buf, 1000);
        buf[ret] = '\0';
        dup2(save_stdout, 1);
        dup2(save_stdin, 0);
        std::cout << buf << std::endl;
    }

    delete[] args;
}