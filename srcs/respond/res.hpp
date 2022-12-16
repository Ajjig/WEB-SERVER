/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   res.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roudouch <roudouch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/09 19:04:01 by roudouch          #+#    #+#             */
/*   Updated: 2022/12/16 21:16:19 by roudouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../../include/header.hpp"

typedef struct file {
    std::string str;
    int size;
    bool is_good;
} s_file;

class Respond
{
    private:
        Request req;
        int status_code;
        bool list_is_allowed;
        std::string body;
        int content_length;
        std::map<std::string, std::string> header;
        Location location;
        
    public:
        // constructors and destructors
        Respond() {};
        Respond(Request &req);
        ~Respond() {};


        // methods
        void list_dir(std::string path);
        void Get();
        void init_body();
        void init_header();
        std::string get_status_code();
        std::string get_date();
        std::map<std::string, std::string> get_type();
        std::map<std::string, std::string> status_msg();
        std::string get_header();
        std::string get_body();
        s_file read_file(std::string filename);
        bool is_allowed_method(std::string method);
        void init_403();
        void init_405();
        void init_404();
        void logs();

        // the response
        std::string get_response();
};