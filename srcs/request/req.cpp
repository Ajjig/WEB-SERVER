/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   req.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roudouch <roudouch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 16:01:45 by roudouch          #+#    #+#             */
/*   Updated: 2022/12/20 14:38:22 by roudouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/header.hpp"

//bool is_exist (const std::string& name) {
//    std::ifstream f(name.c_str());
//    return f.good();
//}

// constructors
//Request::Request(std::string request)
//{
//    this->parse_request(request);
//}

Request::Request(std::string request, Server server) {
    this->parse_request(request);
    this->server = server;
};

// setters and getters
std::map<std::string, std::string> Request::get_headers(void)
{
    return this->headers;
}

void Request::set_headers(std::map<std::string, std::string> headers)
{
    this->headers = headers;
}

std::string &Request::get_method(void)
{
    return this->method;
}

std::string &Request::get_uri(void)
{
    return this->uri;
}

std::string &Request::get_http_version(void)
{
    return this->http_version;
}

//int Request::get_respond_status(void)
//{
//    return this->respond_status;
//}

std::string &Request::get_path(void)
{
    return this->path;
}

//std::string Request::get_type_file(void)
//{
//    return this->file_type;
//}

//std::string Request::get_file_name(void)
//{
//    return this->file_name;
//}

void Request::set_path(std::string path)
{
    this->path = path;
}

Server &Request::get_server() {
    return this->server;
}

//void Request::set_content_type(std::string file_type)
//{
//    this->file_type = file_type;
//}

bool create_file_using_body(std::string &body) {
    std::ofstream file;
    file.open("./html/upload/tmp.png");
    if (file.is_open()) {
        file << body;
        file.close();
        return true;
    }
    return false;
}

void Request::parse_request(std::string request)
{

    // cut header from body
    std::string header = request.substr(0, request.find("\r\n\r\n"));
    std::string body = request.substr(request.find("\r\n\r\n") + 4);
    
    request = header;

    //std::cout << header << std::endl;
    if (create_file_using_body(body))
       std::cout << "file created" << std::endl;
    //else
    //    std::cout << "file not created" << std::endl;

    ////return;


    // split the request into lines
    std::vector<std::string> lines;
    std::string line;
    std::istringstream iss(request);
    while (getline(iss, line))
        lines.push_back(line);
    
    // remove the \r from the end of the line
    lines[0].erase(std::remove(lines[0].begin(), lines[0].end(), '\r'), lines[0].end());
    
    // parse the first line to get the method, uri and http version
    std::string method = lines[0].substr(0, lines[0].find(" "));
    std::string uri = lines[0].substr(lines[0].find(" ") + 1, lines[0].find(" ", lines[0].find(" ") + 1) - lines[0].find(" ") - 1);
    std::string http_version = lines[0].substr(
        lines[0].find(" ", lines[0].find(" ") + 1) + 1,
        lines[0].size() - lines[0].find(" ", lines[0].find(" ") + 1) - 1
    );

    this->method = method;
    this->uri = uri;
    this->http_version = http_version;
    this->path = uri;
    
    // parse the headers and store them in a map
    for (int i = 1; i < (int )lines.size(); i++) {
        try {
            // remove the \r
            lines[i].erase(std::remove(lines[i].begin(), lines[i].end(), '\r'), lines[i].end());

            std::string header_name = lines[i].substr(0, lines[i].find(":"));
            std::string header_value = lines[i].substr(lines[i].find(":") + 2, lines[i].size() - lines[i].find(":") - 2);
            this->headers[std::string(header_name)] = std::string(header_value);

        } catch (const std::exception& e) {
            // skip the line
        }
    }

    

    //// print all header values
    // for (std::map<std::string, std::string>::iterator it = this->headers.begin(); it != this->headers.end(); it++)
    //     std::cout << it->first << " : " << it->second << std::endl;
}