
#include "cgi.hpp"


Cgi::Cgi(std::string file,std::map<std::string, std::string> bin_path, char **envp) : envp(envp), _bin_path(bin_path), _file(file)
{
    this->find_bin();
    this->set_body(this->exec_cgi());
}

Cgi::Cgi(Server server, std::string file) : _file(file) , _server(server)
{
    for (size_t i = 0; i < server.getLocations().size(); i++) 
     {
        if ("/cgi-bin" == server.getLocationPaths()[i])
        {
            // remove the /cgi-bin from the file path using find   
            this->_file = this->_file.substr(this->_file.find(server.getLocationPaths()[i]) + server.getLocationPaths()[i].length());
            this->_file =  server.getLocations()[i].getRoot() + this->_file;
        }
    }
    this->envp = server.getEnv();
    this->set_env();
    this->find_bin();
    this->set_body(this->exec_cgi());
}

Cgi::~Cgi()
{

}


void Cgi::find_bin()
{
     std::string ext = _file.substr(_file.find_last_of("."));
     std::string bin =  _server.getCGI(ext);
     if (bin == "None")
     {
         this->_bin = "none";
         return ;
     }
     else 
     {
         if (ext == "cgi")
         {
             this->_bin = std::string(getenv("PWD")) + std::string("/") + bin;
             return ;
         }
         this->_bin = bin;
         return ;
     }

}

int Cgi::check_file(std::string file)
{
    // check for existence
    if (!access(file.c_str(), F_OK))
    {
        if (access(file.c_str(), X_OK) == -1)
            return 403;
 
        if (access(file.c_str(), R_OK) == -1)
            return 403;
        return 200;
    }
    return 404;
}

int Cgi::check_bin(std::string bin)
{
    if (bin == "none")
        return 404;
    if (access(bin.c_str(), F_OK) == -1)
        return 404;
    if (access(bin.c_str(), X_OK) == -1)
        return 403;
    return 200;
}

std::string Cgi::exec_cgi()
{
    // check for existence and permissions
    int status_bin = this->check_bin(this->_bin);
    if (status_bin != 200)
        return std::to_string(status_bin);
    
    int status_file = this->check_file(this->_file);
    if (status_file != 200)
        return std::to_string(status_file);
    // check for existence and permissions
    
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

void Cgi::set_body(std::string buf)
{
    this->_buffer = buf;
}

std::string Cgi::get_body()
{
    return this->_buffer;
}


void Cgi::set_env()
{
    // set env variables
    for (size_t i = 0; this->envp[i]; i++)
    {
        std::string env = std::string(this->envp[i]);
        std::string key = env.substr(0, env.find("="));
        std::string value = env.substr(env.find("=") + 1);
        this->_env[key] = value;
    }
    // set Server specific variables
    this->_env["SERVER_SOFTWARE"] = "Webserv/1.0";
    this->_env["SERVER_NAME"] = this->_server.getName();
    this->_env["GATEWAY_INTERFACE"] = "CGI/1.1";
    this->_env["SERVER_PROTOCOL"] = "HTTP/1.1";
    this->_env["SERVER_PORT"] = std::to_string(this->_server.getPort());
    this->_env["REQUEST_METHOD"] = "GET";
    this->_env["PATH_INFO"] = this->_file;
    this->_env["PATH_TRANSLATED"] = this->_file;
    this->_env["SCRIPT_NAME"] = this->_file;
    this->_env["QUERY_STRING"] = this->_file;
    this->_env["REMOTE_HOST"] = this->_server.getHost();

    // add env to envp

    this->envp = new char*[this->_env.size() + 1];
    int i = 0;
    for (std::map<std::string, std::string>::iterator it = this->_env.begin(); it != this->_env.end(); it++)
    {
        std::string env = it->first + "=" + it->second;
        this->envp[i] = new char[env.length() + 1];
        strcpy(this->envp[i], env.c_str());
        i++;
    }
}