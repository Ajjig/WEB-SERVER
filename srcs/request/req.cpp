/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   req.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roudouch <roudouch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 16:01:45 by roudouch          #+#    #+#             */
/*   Updated: 2022/12/10 00:46:11 by roudouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "req.hpp"

bool is_exist (const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
}

// constructors
request::request(std::string request)
{
    this->parse_request(request);
}

// setters and getters
std::map<std::string, std::string> request::get_headers(void)
{
    return this->headers;
}

void request::set_headers(std::map<std::string, std::string> headers)
{
    this->headers = headers;
}

std::string &request::get_method(void)
{
    return this->method;
}

std::string &request::get_uri(void)
{
    return this->uri;
}

std::string &request::get_http_version(void)
{
    return this->http_version;
}

int request::get_respond_status(void)
{
    return this->respond_status;
}

std::string &request::get_path(void)
{
    return this->path;
}

std::string request::get_type_file(void)
{
    return this->file_type;
}

std::string request::get_file_name(void)
{
    return this->file_name;
}

// methods
void request::req_logs(void) {
    std::string method = "\e[0;33m" + this->method + "\e[0m";
    std::string uri = "\e[0;37m" + this->uri + "\e[0m";
    std::string http_version = "\e[0;35m" + this->http_version + "\e[0m";
    std::string respond_status = (this->respond_status == 200 ? "\e[0;32m" : "\e[0;31m") + std::to_string(this->respond_status) + "\e[0m";

    std::cout << "[" <<  method << "] " << std::string(respond_status) << " " << uri << " " << http_version << std::endl;
}

void request::parse_request(std::string request)
{
    // need to replace this with the actual path and index file name later that will be passed as arguments
    std::string root_path = "./html";
    std::string index = "index.html";
    // end 

    // split the request into lines
    std::vector<std::string> lines;
    std::string line;
    std::istringstream iss(request);
    while (getline(iss, line))
        lines.push_back(line);
    
    // parse the first line to get the method, uri and http version
    std::string method = lines[0].substr(0, lines[0].find(" "));
    std::string uri = lines[0].substr(lines[0].find(" ") + 1, lines[0].find(" ", lines[0].find(" ") + 1) - lines[0].find(" ") - 1);
    std::string http_version = lines[0].substr(
        lines[0].find(" ", lines[0].find(" ") + 1) + 1,
        lines[0].size() - lines[0].find(" ", lines[0].find(" ") + 1) - 1
    );

    // parse the uri to get the path and the query
    std::string path = uri.substr(0, uri.find("?"));

    if (path[path.size() - 1] == '/')
        path = path + "/" +  index;

    // parse the path to get the file name and the extension
    std::string file_name = path.substr(path.find_last_of("/") + 1, path.size() - path.find_last_of("/") - 1);
    std::string extension = file_name.substr(file_name.find_last_of(".") + 1, file_name.size() - file_name.find_last_of(".") - 1);
    
    
    this->file_name = file_name;
    this->file_type = extension;
    this->path = path;
    this->method = method;
    this->uri = uri;
    this->http_version = http_version;
    this->respond_status = is_exist(root_path + path) ? 200 : 404;

    
    // parse the query to get the query parameters and store them in a map
    std::string query = uri.substr(uri.find("?") + 1, uri.size() - uri.find("?") - 1);
    // check if the query is empty
    if (uri.find("?") != std::string::npos) {
        std::vector<std::string> query_params;
        std::string query_param;
        std::stringstream ss(query);
        while (getline(ss, query_param, '&'))
            query_params.push_back(query_param);
        
        for (int i = 0; i < query_params.size(); i++)
        {
            std::string query_param_name = query_params[i].substr(0, query_params[i].find("="));
            std::string query_param_value = query_params[i].substr(query_params[i].find("=") + 1, query_params[i].size() - query_params[i].find("=") - 1);
            this->query[query_param_name] = query_param_value;
        }
    }
    
    // parse the headers
    for (int i = 1; i < lines.size() - 1; i++)
    {
        std::string header_name = lines[i].substr(0, lines[i].find(":"));
        std::string header_value = lines[i].substr(lines[i].find(":") + 2, lines[i].size() - lines[i].find(":") - 2);
        this->headers[header_name] = header_value;
    }
}