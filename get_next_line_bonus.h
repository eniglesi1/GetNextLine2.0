/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Your Name <you@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 11:00:00 by you               #+#    #+#             */
/*   Updated: 2025/05/03 11:05:00 by you              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

// Standard library includes needed for get_next_line_bonus
# include <unistd.h> // For read()
# include <stdlib.h> // For malloc(), free()
# include <stdio.h>  // For FOPEN_MAX (commonly used for array of static buffers)
# include <stddef.h> // For size_t

// Definition of BUFFER_SIZE, can be overridden by compiler flags.
// This is standard for 42 projects.
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

// Function prototype for the bonus version of get_next_line
char	*get_next_line_bonus(int fd);

// Note on utility functions:
// The bonus version (get_next_line_bonus.c) will likely reuse utility
// functions from the mandatory part (e.g., ft_strlen, ft_strlcpy, extract_line).
// This can be achieved by:
// 1. Including "get_next_line.h" in "get_next_line_bonus.c" (if allowed by norm).
// 2. Compiling get_next_line_utils.c and linking get_next_line_utils.o
//    with the bonus part.
// If bonus-specific utility functions are needed (e.g., in a separate
// get_next_line_utils_bonus.c), their prototypes would be declared here.
// For now, no bonus-specific utility prototypes are added.

#endif // GET_NEXT_LINE_BONUS_H
