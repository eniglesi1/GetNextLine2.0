/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Your Name <you@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 12:00:00 by you               #+#    #+#             */
/*   Updated: 2025/05/03 12:15:00 by you              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h" // For get_next_line_bonus prototype, BUFFER_SIZE
#include "get_next_line.h"       // For utility functions (extract_line, ft_*, etc.)
                                 // and FOPEN_MAX (as it's included via stdio.h in get_next_line.h)

char	*get_next_line_bonus(int fd)
{
	static char	gnl_static_buffers[FOPEN_MAX][BUFFER_SIZE + 1];
	char		*current_processing_buffer;
	char		*line_to_return;
	int			bytes_read;
	char		read_chunk[BUFFER_SIZE + 1];
    size_t      buffer_content_len; // Stores length of content

	// Requirement 3.a: Handle invalid fd or BUFFER_SIZE.
	if (fd < 0 || BUFFER_SIZE <= 0 || fd >= FOPEN_MAX)
		return (NULL);

	// Requirement 3.b: Initialize current_processing_buffer.
	current_processing_buffer = ft_calloc(1, sizeof(char)); // Initial empty string
	if (!current_processing_buffer) 
		return (NULL);

	// Load any existing remainder from gnl_static_buffers[fd] into it.
	if (gnl_static_buffers[fd][0] != '\0')
	{
        buffer_content_len = ft_strlen(gnl_static_buffers[fd]);
		char *temp = ft_realloc(current_processing_buffer, buffer_content_len + 1);
		if (!temp) { free(current_processing_buffer); return (NULL); }
		current_processing_buffer = temp;
		ft_strlcpy(current_processing_buffer, gnl_static_buffers[fd], buffer_content_len + 1);
		// Clear that part of the static buffer.
		gnl_static_buffers[fd][0] = '\0'; 
	}

	// Requirement 3.c: Loop.
	// Check if current_processing_buffer contains a newline using ft_strchr.
	while (ft_strchr(current_processing_buffer, '\n') == NULL)
	{
		// Read from fd into a temporary read_chunk buffer.
		bytes_read = read(fd, read_chunk, BUFFER_SIZE);
		
		// Handle read return values:
		if (bytes_read < 0) // -1 (Error)
		{
			free(current_processing_buffer);
			gnl_static_buffers[fd][0] = '\0'; // Clear gnl_static_buffers[fd].
			return (NULL);
		}
		if (bytes_read == 0) // 0 (EOF)
			break;
		
		read_chunk[bytes_read] = '\0'; // Null-terminate read_chunk.

        buffer_content_len = ft_strlen(current_processing_buffer);
		// Append read_chunk to current_processing_buffer.
		char *temp = ft_realloc(current_processing_buffer, buffer_content_len + bytes_read + 1);
		if (!temp) 
        { 
            free(current_processing_buffer); 
            gnl_static_buffers[fd][0] = '\0'; // Clear gnl_static_buffers[fd] on realloc failure.
            return (NULL); 
        }
		current_processing_buffer = temp;
		ft_strlcat(current_processing_buffer, read_chunk, buffer_content_len + bytes_read + 1);
	}

	// Requirement 3.d: After the loop, if current_processing_buffer is empty...
	if (current_processing_buffer[0] == '\0')
	{
		free(current_processing_buffer);
		return (NULL);
	}

	// Requirement 3.e: Call extract_line.
	line_to_return = extract_line(current_processing_buffer);

	// Requirement 3.f: If extract_line returns NULL (malloc failure).
	if (line_to_return == NULL) 
	{
        // extract_line did not modify current_processing_buffer if malloc failed.
        // Copy its contents back to gnl_static_buffers[fd].
		ft_strlcpy(gnl_static_buffers[fd], current_processing_buffer, BUFFER_SIZE + 1);
		free(current_processing_buffer);
		return (NULL); 
	}

	// Requirement 3.g: Copy the remainder from current_processing_buffer to gnl_static_buffers[fd].
	// (extract_line modified current_processing_buffer to hold the remainder).
	ft_strlcpy(gnl_static_buffers[fd], current_processing_buffer, BUFFER_SIZE + 1);
	
	// Requirement 3.h: Free current_processing_buffer.
	free(current_processing_buffer); 
	// Requirement 3.i: Return the line_to_return.
	return (line_to_return);
}
