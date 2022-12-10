/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   res.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roudouch <roudouch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/09 19:04:01 by roudouch          #+#    #+#             */
/*   Updated: 2022/12/10 01:06:24 by roudouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RES_HPP
#define RES_HPP

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
            std::string get_status_code();
            std::string get_date();
            std::map<std::string, std::string> get_type();
            std::map<std::string, std::string> status_msg();

            // the response
            std::string get_response();
        };
    

#endif