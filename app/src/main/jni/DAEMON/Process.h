//
// Created by PCAditiaID29 on 2/10/2021.
//

#ifndef RCHEATS_PROCESS_H
#define RCHEATS_PROCESS_H

#include <sys/types.h>
#include <sys/uio.h>
#include <sys/syscall.h>
#include <unistd.h>

pid_t target_pid = -1;


#if defined(__arm__)
int process_vm_readv_syscall = 376;
int process_vm_writev_syscall = 377;
#elif defined(__aarch64__)
int process_vm_readv_syscall = 270;
int process_vm_writev_syscall = 271;
#elif defined(__i386__)
int process_vm_readv_syscall = 347;
int process_vm_writev_syscall = 348;
#else
int process_vm_readv_syscall = 310;
int process_vm_writev_syscall = 311;
#endif


bool pvm(void* address, void* buffer, size_t size, bool iswrite) {
    struct iovec local[1];
    struct iovec remote[1];
    local[0].iov_base = buffer;
    local[0].iov_len = size;
    remote[0].iov_base = address;
    remote[0].iov_len = size;
    if (target_pid < 0) {
        return false;
    }
    ssize_t bytes = syscall((iswrite ? process_vm_writev_syscall : process_vm_readv_syscall), target_pid, local, 1, remote, 1, 0);
    return bytes == size;
}

bool vm_readv(void* address, void* buffer, size_t size) {
    return pvm(address, buffer, size, false);
}

bool vm_writev(void* address, void* buffer, size_t size) {
    return pvm(address, buffer, size, true);
}
#endif //RCHEATS_PROCESS_H
