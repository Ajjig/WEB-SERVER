/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   req.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roudouch <roudouch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 16:02:05 by roudouch          #+#    #+#             */
/*   Updated: 2022/12/18 21:25:50 by roudouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../../include/header.hpp"

class Request {
    private:
        std::string                         method;
        std::string                         uri;
        std::string                         path;
        std::string                         http_version;
        //int                                 respond_status;
        //std::map<std::string, std::string>  query;
        std::map<std::string, std::string>  headers;
        //std::string                         file_name;
        //std::string                         file_type;
        Server                              server;

    public:
        // constructors and destructors
        Request(){};
        //Request(std::string request);
        Request(std::string request, Server server);
        ~Request(){}
        
        // setters and getters
        Server                              &get_server(void);
        std::map<std::string, std::string>  get_headers(void);
        std::string                         &get_method(void);
        std::string                         &get_uri(void);
        std::string                         &get_http_version(void);
        //int                                 get_respond_status(void);
        void                                set_headers(std::map<std::string, std::string> headers);
        std::string                         &get_path(void);
        // methods
        void                                parse_request(std::string request);
        void                                set_path(std::string path);
        //std::string                         get_type_file(void);
        //std::string                         get_file_name(void);
        //void                                set_content_type(std::string file_type);
};