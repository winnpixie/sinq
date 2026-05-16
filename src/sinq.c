#include <stdlib.h>
#include <stdio.h>
#include "util.h"

#define LINE_MAX 128

void print_cpu_info()
{
	char line[LINE_MAX];

	FILE *fp_cpu_info = fopen("/proc/cpuinfo", "r");
	if (fp_cpu_info == NULL)
	{
		printf("Unable to retrieve CPU information!\n");
		return;
	}
	
	int core_count = 0;
	while (read_line_s(fp_cpu_info, line, LINE_MAX) > -1)
	{
		if (index_of(line, "processor") == 0)
		{
			core_count++;
		}
	}
	fclose(fp_cpu_info);

	printf("Core Count = %d\n", core_count);
}

void print_cpu_temp()
{
	char line[LINE_MAX];
	FILE *fp_thermals = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
	if (fp_thermals == NULL)
	{
		printf("Unable to retrieve Temperature information!\n");
		return;
	}

	read_line_s(fp_thermals, line, LINE_MAX);
	fclose(fp_thermals);

	double core_temperature = atof(line);
	printf("Temperature = %.1f*C\n", (core_temperature / 1000.0));
}

void print_mem_stat()
{
	int mem_total;
	int mem_avail;
	char line[LINE_MAX];

	FILE *fp_mem_info = fopen("/proc/meminfo", "r");
	if (fp_mem_info == NULL)
	{
		printf("Unable to retrieve Memory Information!\n");
		return;
	}

	while (read_line_s(fp_mem_info, line, LINE_MAX) > -1)
	{
		if (index_of(line, "MemTotal") == 0)
		{
			mem_total = extract_int(line);
		} else if (index_of(line, "MemAvailable") == 0)
		{
			mem_avail = extract_int(line);
		}
	}
	fclose(fp_mem_info);

	int mem_used = mem_total - mem_avail;
	printf("Total = %dMB\n", mem_total / 1024);
	printf("Available = %dMB (%.0f%%)\n", mem_avail / 1024, ((double)mem_avail / mem_total) * 100.0);
	printf("In Use = %dMB (%.0f%%)\n", mem_used / 1024, ((double)mem_used / mem_total) * 100.0);
}

void print_public_addr()
{
	FILE *p_curl = popen("curl -GLsS \"https://checkip.amazonaws.com/\"", "r");
	if (p_curl == NULL)
	{
		printf("Unable to launch cURL!\n");
		return;
	}

	char ip_addr[40];
	read_line_s(p_curl, ip_addr, 39);
	pclose(p_curl);

	printf("Public IP Address = %s\n", ip_addr);
}

int main(int argc, char *argv[])
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
