
#include "cgi.hpp"


Cgi::Cgi(std::string file,std::map<std::string, std::string> bin_path, char **envp) : envp(envp), _bin_path(bin_path), _file(file)
{
    this->find_bin();
    this->start(this->exec_cgi());
}

Cgi::~Cgi()
{

}


void Cgi::find_bin()
{
    std::string path = _file.substr(_file.find_last_of(".") + 1);
    for (std::map<std::string, std::string>::iterator it = _bin_path.begin(); it != _bin_path.end(); it++)
    {
        if (it->first == path)
        {
            if (it->first == "cgi")
            {
                this->_bin = it->second + std::string("/") + _file;
                return ;
            }
            this->_bin = it->second;
            return ;
        }
    }
    this->_bin = "none";

}

std::string Cgi::exec_cgi()
{
    int fd[2];
    pipe(fd);
    int pid = fork();
    char **args = new char*[3];

    args[0] = (char*)this->_bin.c_str();
    args[1] = (char *)this->_file.c_str();
    args[2] = NULL;

    int save_stdout = dup(1);
    int save_stdin = dup(0);


    if (this->_bin == "none")
        return "none";

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
        
        char buf[1024];
        int ret =0; 
        while ((ret = read(0, buf, 1020)) > 0)
        {
            buf[ret] = '\0';
            this->_buffer += std::string(buf);
        }

        dup2(save_stdout, 1);
        dup2(save_stdin, 0);
    }

    delete[] args;
    
    return this->_buffer;
}

std::string Cgi::start(std::string buf)
{
    if (buf == "none")
    {
        this->_buffer = "none";
        return NULL;
    }
    else 
    {
        return this->_buffer;
    }
}

std::string Cgi::get_response()
{
    return this->_buffer;
}