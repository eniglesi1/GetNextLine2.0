/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Your Name <you@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 13:00:00 by you               #+#    #+#             */
/*   Updated: 2025/05/03 13:00:00 by you              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "get_next_line_bonus.h"
#include <stdio.h>  // For printf, fopen, fclose (though using open for fd)
#include <fcntl.h>  // For open, close, O_RDONLY
#include <stdlib.h> // For free
#include <string.h> // For strcmp (if comparing line content, not used here)

void test_mandatory_file(const char *filename)
{
    int fd;
    char *line;
    int line_num = 1;

    printf("\n--- Testing Mandatory: %s ---\n", filename);
    fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        perror("Error opening file");
        return;
    }

    while ((line = get_next_line(fd)) != NULL)
    {
        printf("Line %d: [%s]\n", line_num++, line);
        free(line);
    }
    // After loop, one last call to ensure it returns NULL if file ends without \n
    // or if it was empty. The loop condition already handles this.
    // If get_next_line returned NULL, line is NULL, loop terminates.

    close(fd);
    printf("--- Finished Mandatory: %s ---\n", filename);
}

int main(void)
{
    // --- Mandatory Part Testing ---
    const char *mandatory_files[] = {
        "test1.txt", 
        "empty.txt", 
        "no_newline.txt", 
        "oneline.txt", 
        "multiple_nl.txt",
        "non_existent_file.txt" // Test opening a non-existent file
    };
    int num_mandatory_files = sizeof(mandatory_files) / sizeof(mandatory_files[0]);

    for (int i = 0; i < num_mandatory_files; i++)
    {
        test_mandatory_file(mandatory_files[i]);
    }

    // --- Bonus Part Testing ---
    printf("\n\n--- Testing Bonus Part (Interleaved Reads) ---\n");
    int fd_bonus1, fd_bonus2, fd_bonus3;
    char *line_b1, *line_b2, *line_b3;
    int active_fds = 0;

    fd_bonus1 = open("test1.txt", O_RDONLY);
    if (fd_bonus1 == -1) {
        perror("Error opening test1.txt for bonus");
    } else {
        active_fds++;
        printf("Opened test1.txt for bonus as fd_bonus1 (%d)\n", fd_bonus1);
    }

    fd_bonus2 = open("test2.txt", O_RDONLY);
    if (fd_bonus2 == -1) {
        perror("Error opening test2.txt for bonus");
    } else {
        active_fds++;
        printf("Opened test2.txt for bonus as fd_bonus2 (%d)\n", fd_bonus2);
    }
    
    fd_bonus3 = open("oneline.txt", O_RDONLY);
    if (fd_bonus3 == -1) {
        perror("Error opening oneline.txt for bonus");
    } else {
        active_fds++;
        printf("Opened oneline.txt for bonus as fd_bonus3 (%d)\n", fd_bonus3);
    }

    if (active_fds == 0) {
        printf("No files could be opened for bonus testing. Skipping.\n");
    } else {
        printf("\nStarting interleaved reads (BUFFER_SIZE=%d):\n", BUFFER_SIZE);
        int line_count_b1 = 1, line_count_b2 = 1, line_count_b3 = 1;
        int fd1_done = (fd_bonus1 == -1), fd2_done = (fd_bonus2 == -1), fd3_done = (fd_bonus3 == -1);

        while (active_fds > 0)
        {
            if (!fd1_done)
            {
                line_b1 = get_next_line_bonus(fd_bonus1);
                if (line_b1 != NULL)
                {
                    printf("FD1 (test1.txt) Line %d: [%s]\n", line_count_b1++, line_b1);
                    free(line_b1);
                }
                else
                {
                    printf("FD1 (test1.txt) finished.\n");
                    fd1_done = 1;
                    active_fds--;
                    if(fd_bonus1 != -1) close(fd_bonus1); // Close as soon as it's done
                    fd_bonus1 = -1; // Mark as closed
                }
            }

            if (!fd2_done)
            {
                line_b2 = get_next_line_bonus(fd_bonus2);
                if (line_b2 != NULL)
                {
                    printf("FD2 (test2.txt) Line %d: [%s]\n", line_count_b2++, line_b2);
                    free(line_b2);
                }
                else
                {
                    printf("FD2 (test2.txt) finished.\n");
                    fd2_done = 1;
                    active_fds--;
                    if(fd_bonus2 != -1) close(fd_bonus2);
                    fd_bonus2 = -1;
                }
            }
            
            if (!fd3_done)
            {
                line_b3 = get_next_line_bonus(fd_bonus3);
                if (line_b3 != NULL)
                {
                    printf("FD3 (oneline.txt) Line %d: [%s]\n", line_count_b3++, line_b3);
                    free(line_b3);
                }
                else
                {
                    printf("FD3 (oneline.txt) finished.\n");
                    fd3_done = 1;
                    active_fds--;
                    if(fd_bonus3 != -1) close(fd_bonus3);
                    fd_bonus3 = -1;
                }
            }
             if (fd1_done && fd2_done && fd3_done) break; // All explicitly done
        }
    }
    
    // Ensure any fds that might not have been closed in the loop (e.g. if loop exited early) are closed.
    if(fd_bonus1 != -1) close(fd_bonus1);
    if(fd_bonus2 != -1) close(fd_bonus2);
    if(fd_bonus3 != -1) close(fd_bonus3);

    printf("\n--- Finished Bonus Part ---\n");
    printf("\nNote: BUFFER_SIZE for this test run is %d as defined in header or by compilation flags.\n", BUFFER_SIZE);
    printf("If testing with a specific BUFFER_SIZE (e.g., -D BUFFER_SIZE=X), that value will be used by GNL functions.\n");

    return (0);
}
