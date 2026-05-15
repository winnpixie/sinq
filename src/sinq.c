#include <stdlib.h>
#include <stdio.h>
#include "util.h"

#define LINE_MAX 128

void print_cpu_info()
{
	char line_buf[LINE_MAX];

	FILE *cpuinfo_fp = fopen("/proc/cpuinfo", "r");
	if (cpuinfo_fp == NULL)
	{
		printf("Unable to retrieve CPU information!\n");
		return;
	}
	
	int core_count = 0;
	while (fgets(line_buf, LINE_MAX, cpuinfo_fp) > 0)
	{
		if (index_of(line_buf, "processor") == 0)
		{
			core_count++;
		}
	}
	fclose(cpuinfo_fp);

	printf("Core Count = %d\n", core_count);
}

void print_cpu_temp()
{
	char line_buf[8];
	FILE *thermal_temp_fp = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
	if (thermal_temp_fp == NULL)
	{
		printf("Unable to retrieve Temperature information!\n");
		return;
	}

	fgets(line_buf, 8, thermal_temp_fp);
	fclose(thermal_temp_fp);

	double core_temperature = atof(line_buf);
	printf("Temperature = %.1f*C\n", (core_temperature / 1000.0));
}

void print_mem_stat()
{
	int mem_total;
	int mem_avail;
	char line_buf[LINE_MAX];

	FILE *meminfo_fp = fopen("/proc/meminfo", "r");
	if (meminfo_fp == NULL)
	{
		printf("Unable to retrieve Memory Information!\n");
		return;
	}

	while (fgets(line_buf, LINE_MAX, meminfo_fp) > 0)
	{
		if (index_of(line_buf, "MemTotal") == 0)
		{
			mem_total = extract_int(line_buf);
		} else if (index_of(line_buf, "MemAvailable") == 0)
		{
			mem_avail = extract_int(line_buf);
		}
	}
	fclose(meminfo_fp);

	int mem_used = mem_total - mem_avail;
	printf("Total = %dMB\n", mem_total / 1024);
	printf("Available = %dMB (%.0f%%)\n", mem_avail / 1024, ((double)mem_avail / mem_total) * 100.0);
	printf("In Use = %dMB (%.0f%%)\n", mem_used / 1024, ((double)mem_used / mem_total) * 100.0);
}

void print_public_addr()
{
	FILE *curl_proc = popen("curl -GLsS \"https://checkip.amazonaws.com/\"", "r");
	if (curl_proc == NULL)
	{
		printf("Unable to launch cURL!\n");
		return;
	}

	char ip_addr[40];
	readline_s(curl_proc, ip_addr, 39);
	pclose(curl_proc);

	printf("Public IP Address = %s\n", ip_addr);
}

int main(int argc, char **argv)
{
	printf("=== sinq ===\n");
	// CPU
	printf("--- CPU ---\n");
	print_cpu_info();
	print_cpu_temp();
	
	// Memory
	printf("\n--- Memory ---\n");
	print_mem_stat();

	// Network
	printf("\n--- Network ---\n");
	print_public_addr();
}
