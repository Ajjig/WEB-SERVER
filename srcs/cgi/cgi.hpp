/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roudouch <roudouch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/09 12:08:04 by roudouch          #+#    #+#             */
/*   Updated: 2022/12/12 00:17:02 by roudouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "../../include/header.hpp"

    class cgi {
        private:
            Request *req;
        public:
            cgi(Request *req);
    };
