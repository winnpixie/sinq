#include <stdlib.h>
#include <stdio.h>
#include "util.h"

#define LINE_MAX 128

void print_cpu_info()
{
	char link_path[PATH_MAX];
	char line_buf[LINE_MAX];

	FILE *cpu_info_fp = fopen("/proc/cpuinfo", "r");
	if (cpu_info_fp == NULL)
	{
		printf("Unable to retrieve CPU information!\n");
		return;
	}
	
	int core_count = 0;
	while (fgets(line_buf, LINE_MAX, cpu_info_fp) > 0)
	{
		if (index_of(line_buf, "processor") == 0)
		{
			core_count++;
		}
	}
	fclose(cpu_info_fp);

	printf("Core Count = %d\n", core_count);
}

void print_cpu_temp()
{
	char line_buf[8];
	FILE *temperature_fp = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
	if (temperature_fp == NULL)
	{
		printf("Unable to retrieve Temperature information!\n");
		return;
	}

	fgets(line_buf, 8, temperature_fp);
	fclose(temperature_fp);

	double core_temperature = atof(line_buf);
	printf("Temperature = %.1f*C\n", (core_temperature / 1000.0));
}

void print_mem_stat()
{
	int mem_total;
	int mem_avail;
	char line_buf[LINE_MAX];

	FILE *mem_info_fp = fopen("/proc/meminfo", "r");
	if (mem_info_fp == NULL)
	{
		printf("Unable to retrieve Memory Information!\n");
		return;
	}

	while (fgets(line_buf, LINE_MAX, mem_info_fp) > 0)
	{
		if (index_of(line_buf, "MemTotal") == 0)
		{
			mem_total = extract_int(line_buf);
		} else if (index_of(line_buf, "MemAvailable") == 0)
		{
			mem_avail = extract_int(line_buf);
		}
	}
	fclose(mem_info_fp);

	int mem_used = mem_total - mem_avail;
	printf("Total = %dMB\n", mem_total / 1024);
	printf("Available = %dMB (%.0f%%)\n", mem_avail / 1024, ((double)mem_avail / mem_total) * 100.0);
	printf("In Use = %dMB (%.0f%%)\n", mem_used / 1024, ((double)mem_used / mem_total) * 100.0);
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
}
