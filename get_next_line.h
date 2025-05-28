/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eiglesia <eiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:00:56 by eiglesia          #+#    #+#             */
/*   Updated: 2025/05/28 17:47:28 by eiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <stddef.h>
# include <stdio.h>
//# define BUFFER_SIZE 99

char	*get_next_line(int fd);
char	*ft_realloc(char *s1, int i);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
int		ft_copynl(char *extra, int baits, char *string, int aux);
int		read_buffer_fd(int fd, char *extra);

#endif
