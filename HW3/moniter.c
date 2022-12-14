// CPU 사용률, 메모리 사용률, 디스크 사용률, 프로세스 상태, network 트래픽을 모니터링 해주는 프로그램

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <curses.h>
#include <sys/sysinfo.h>

#define USLEEP_INTERVAL 1000

void check_cpu_usage();
void check_mem_usage();
void check_disk_usage();
void check_process_status();
void check_network_traffic();

int cols;

int main()
{
    initscr();
    crmode();
    //noecho();
    clear();
    cols = COLS - 25;

    while (1)
    {   
        clear();
        check_cpu_usage();
        check_mem_usage();
        check_disk_usage();
        check_process_status();
        check_network_traffic();
        refresh();
        usleep(USLEEP_INTERVAL);
    }

    endwin();
    return 0;
}

void check_cpu_usage()
{
    static int prev_total = 0;
    static int prev_idle = 0;
    int usr, nice, sys, idle, iowait, irq, soft, steal, guest;
    int total;
    double cpu_usage = 0;
    char buf[100];
    FILE *fp = fopen("/proc/stat", "r");
    if (fp == NULL)
    {
        perror("fopen");
        exit(1);
    }
    fgets(buf, sizeof(buf), fp);
    fclose(fp);
    sscanf(buf, "cpu %d %d %d %d %d %d %d %d %d", &usr, &nice, &sys, &idle, &iowait, &irq, &soft, &steal, &guest);
    total = usr + nice + sys + idle + iowait + irq + soft + steal + guest;
    cpu_usage = ((double)total - idle) * 100.0 / total;
    move(0, 0);
    printw("CPU Usage: [");
    for (int i = 0; i < cpu_usage*cols/100; i++)
        printw("=");
    for (int i = 0; i < cols - cpu_usage*cols/100; i++)
        printw(" ");
    printw("] %.2f%%", cpu_usage);
}

void check_mem_usage()
{
    //read mem info from /proc/meminfo
    int mem_total = 0;
    int mem_free = 0;
    int mem_usage = 0;
    char buf[100];
    FILE *fp = fopen("/proc/meminfo", "r");
    if (fp == NULL)
    {
        perror("fopen");
        exit(1);
    }
    fgets(buf, sizeof(buf), fp);
    sscanf(buf, "MemTotal: %d", &mem_total);
    fgets(buf, sizeof(buf), fp);
    sscanf(buf, "MemFree: %d", &mem_free);
    fclose(fp);
    mem_usage = 100 - mem_free * 100 / mem_total;
    move(1, 0);
    printw("Memory Usage: [");
    for (int i = 0; i < mem_usage*cols/100; i++)
        printw("=");
    for (int i = 0; i < cols - mem_usage*cols/100; i++)
        printw(" ");
    printw("] %d%%", mem_usage);
}

void check_disk_usage()
{
    //read disk info from /proc/diskstats
    int disk_read = 0;
    int disk_write = 0;
    char buf[100];
    FILE *fp = fopen("/proc/diskstats", "r");
    if (fp == NULL)
    {
        perror("fopen");
        exit(1);
    }
    fgets(buf, sizeof(buf), fp);
    sscanf(buf, "%*d %*d %*s %d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %d", &disk_read, &disk_write);
    fclose(fp);
    move(2, 0);
    printw("Disk Usage: read %dKB/s, write %dKB/s", disk_read, disk_write);
}

void check_process_status()
{
    //read process info from /proc/stat
    int process_running = 0;
    int process_blocked = 0;
    char buf[100];
    FILE *fp = fopen("/proc/stat", "r");
    if (fp == NULL)
    {
        perror("fopen");
        exit(1);
    }
    while (fgets(buf, sizeof(buf), fp) != NULL)
    {
        if (strncmp(buf, "procs_running", 13) == 0)
        {
            sscanf(buf, "procs_running %d", &process_running);
        }
        else if (strncmp(buf, "procs_blocked", 13) == 0)
        {
            sscanf(buf, "procs_blocked %d", &process_blocked);
        }
    }
    fclose(fp);
    move(3, 0);
    printw("Process Status: running %d, blocked %d", process_running, process_blocked);
}

void check_network_traffic()
{
    //read network info from /proc/net/dev
    int network_recv = 0;
    int network_send = 0;
    char buf[100];
    FILE *fp = fopen("/proc/net/dev", "r");
    if (fp == NULL)
    {
        perror("fopen");
        exit(1);
    }
    fgets(buf, sizeof(buf), fp);
    fgets(buf, sizeof(buf), fp);
    fgets(buf, sizeof(buf), fp);
    sscanf(buf, "wlan0: %d %*d %*d %*d %*d %*d %*d %*d %*d %d", &network_recv, &network_send);
    fclose(fp);
    move(4, 0);
    printw("Network Traffic: recv %dKB/s, send %dKB/s", network_recv, network_send);
}