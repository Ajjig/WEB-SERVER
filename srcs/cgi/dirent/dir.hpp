/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dir.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roudouch <roudouch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/11 17:16:26 by roudouch          #+#    #+#             */
/*   Updated: 2022/12/11 17:18:52 by roudouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIR_HPP
# define DIR_HPP

# include <dirent.h>
# include <iostream>
# include <fstream>
# include <string>
# include <vector>

std::vector<std::string> GetDirFiles(std::string path);

#endif
