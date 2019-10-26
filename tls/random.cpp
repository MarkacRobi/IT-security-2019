#include "random.h"
#include <linux/random.h>

#include <unistd.h>
#include <sys/syscall.h>

bool get_random_data(uint8_t* data, std::size_t size)
{
    syscall(SYS_getrandom, data, size, firstIndex);
}
