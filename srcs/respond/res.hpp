/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   res.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roudouch <roudouch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/09 19:04:01 by roudouch          #+#    #+#             */
/*   Updated: 2022/12/11 23:21:23 by roudouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RES_HPP
#define RES_HPP

#define ROOT_PATH "./html"

#include "../request/req.hpp"
#include "../../include/header.hpp"
#include "../dirent/dir.hpp"

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
            std::vector<std::string> allowed_methods;
            
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

            // the response
            std::string get_response();
        };
    

#endif