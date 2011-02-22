#include <stdio.h>
#include <errno.h>
#include <sys/time.h>
#include <fcntl.h>

ssize_t do_recvfile(int out_fd, int in_fd, off_t offset, size_t count)
{
    ssize_t bytes, bytes_sent, bytes_in_pipe;
    size_t total_bytes_sent = 0;

    /* Splice the data from in_fd into the pipe */
    while (total_bytes_sent < count) {
        if ((bytes_sent = splice(in_fd, NULL, pipefd[1], NULL,
                count - total_bytes_sent,
                SPLICE_F_MORE | SPLICE_F_MOVE)) <= 0) {
            if (errno != EINTR && errno != EAGAIN) {
                perror("splice");
                return -1;
            }
            /* Interrupted system call/try again
               Just skip to the top of the loop and try again */
        }

        /* Splice the data from the pipe into out_fd */
        bytes_in_pipe = bytes_sent;
        while (bytes_in_pipe > 0) {
            if ((bytes = splice(pipefd[0], NULL, out_fd, &offset, bytes_in_pipe,
                    SPLICE_F_MORE | SPLICE_F_MOVE)) <= 0) {
                if (errno != EINTR && errno != EAGAIN) {
                    perror("splice");
                    return -1;
                }
            }
            bytes_in_pipe -= bytes;
        }
        total_bytes_sent += bytes_sent;
    }
    return total_bytes_sent;
}

int main()
{
    int pipe[2], i = 0;
    size_t b_sent = 0, total_b_sent = 0, b_in_pipe = 0, count = 0;
    struct timespec start, end, diff;

    if(pipe(&pipe) < 0) {
        perror("Boum ! Headshot !\n");
        exit(errno);
    }

    nanotime(&start);

    for(i = 0; i < 1000; ++i)
        do_recvfile(); /* TODO */

    nanotime(&end);

    if((end.tv_nsec > start.tv_nsec)) {
        diff.tv_sec = end.tv_sec - start.tv_sec;
        diff.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
    else {
        diff.tv_sec = end.tv_sec - start.tv_sec - 1;
        diff.tv_nsec = 1000000000 - t2.tv_nsec + t1.tv_nsec;
    }

    printf("bench : %d sec %d ns\n", diff.tv_sec, diff.tv_nsec);
    
    exit(0);
}
