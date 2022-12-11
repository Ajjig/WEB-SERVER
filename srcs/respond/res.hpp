/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   res.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roudouch <roudouch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/09 19:04:01 by roudouch          #+#    #+#             */
/*   Updated: 2022/12/11 13:10:28 by roudouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RES_HPP
#define RES_HPP

#define ROOT_PATH "./html"

#include "../request/req.hpp"
#include "../../include/header.hpp"

    class respond
    {
        private:
            int status_code;
            request req;
            std::string body;
            int content_length;
            std::string header;
            
        public:
            // constructors and destructors
            respond() {};
            respond(request &req);
            ~respond() {};


            // methods
            void Get();
            void init_body();
            void init_header();
            std::string get_status_code();
            std::string get_date();
            std::map<std::string, std::string> get_type();
            std::map<std::string, std::string> status_msg();
            std::string get_header();
            std::string get_body();

            // the response
            std::string get_response();
        };
    

#endif