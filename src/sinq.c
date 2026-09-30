#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "util.h"

#define LINE_MAX 256
#define MAX(a, b) (a > b ? a : b)

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
	while (io_readline_s(fp_cpu_info, line, LINE_MAX) > -1)
	{
		if (str_indexof(line, "processor") == 0)
		{
			core_count++;
		}
	}
	fclose(fp_cpu_info);

	printf("Core Count = %d\n", core_count);
}

void print_cpu_freq()
{
	char line[LINE_MAX];

	FILE *fp_cpu_freq = fopen("/sys/bus/cpu/devices/cpu0/cpufreq/cpuinfo_max_freq", "r");
	if (fp_cpu_freq != NULL)
	{
		io_readline_s(fp_cpu_freq, line, LINE_MAX);
		fclose(fp_cpu_freq);

		int frequency = extract_int(line);
		printf("Frequency[cpu0] = ");
		if (frequency > 999999)
		{
			printf("%.2f GHz\n", frequency / 1000000.0);
		} else if (frequency > 999)
		{
			printf("%.2f MHz\n", frequency / 1000.0);
		} else
		{
			printf("%d Hz\n", frequency);
		}

		return;
	}

	// try executing lscpu
	FILE *p_lscpu = popen("lscpu", "r");
	if (p_lscpu != NULL)
	{	
		double freq = 0.0;
		while (io_readline_s(p_lscpu, line, LINE_MAX) > -1)
		{
			if (strstr(line, "max MHz:") != NULL)
			{
				double tmp = extract_fp(line);
				freq = MAX(freq, tmp);
			}
		}
		pclose(p_lscpu);

		if (freq > 0.0)
		{
			printf("Frequency[lscpu] = ");
			if (freq > 999.0)
			{
				printf("%.2f GHz\n", freq / 1000.0);
			} else if (freq >= 1.0)
			{
				printf("%.2f MHz\n", freq);
			} else
			{
				printf("%.0f Hz\n", freq * 1000.0);
			}

			return;
		}
	}

	// try /proc/cpuinfo
	FILE *fp_cpu_info = fopen("/proc/cpuinfo", "r");
	if (fp_cpu_info != NULL)
	{
		double freq = 0.0;
		while (io_readline_s(fp_cpu_info, line, LINE_MAX) > -1)
		{
			if (str_indexof(line, "cpu MHz") == 0)
			{
				double tmp = extract_fp(line);
				freq = MAX(freq, tmp);
			}
		}
		fclose(fp_cpu_info);

		if (freq > 0.0)
		{
			printf("Frequency[cpuinfo] = ");

			if (freq > 999.0)
			{
				printf("%.2f GHz\n", freq / 1000.0);
			} else if (freq >= 1.0)
			{
				printf("%.2f MHz\n", freq);
			} else
			{
				printf("%.0f Hz\n", freq * 1000.0);
			}
		}
	}

	printf("Unable to retrieve CPU frequency information!\n");
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

	io_readline_s(fp_thermals, line, LINE_MAX);
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

	while (io_readline_s(fp_mem_info, line, LINE_MAX) > -1)
	{
		if (str_indexof(line, "MemTotal") == 0)
		{
			mem_total = extract_int(line);
		} else if (str_indexof(line, "MemAvailable") == 0)
		{
			mem_avail = extract_int(line);
		}
	}
	fclose(fp_mem_info);

	int mem_used = mem_total - mem_avail;
	printf("Total = %dMB\n", mem_total / 1024);
	printf("In Use = %dMB (%.1f%%)\n", mem_used / 1024, ((double)mem_used / mem_total) * 100.0);
	printf("Available = %dMB (%.1f%%)\n", mem_avail / 1024, ((double)mem_avail / mem_total) * 100.0);
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
	io_readline_s(p_curl, ip_addr, 39);
	pclose(p_curl);

	printf("Public IP Address = %s\n", ip_addr);
}

void print_uptime_info()
{
	char line[LINE_MAX];

	FILE *fp_uptime = fopen("/proc/uptime", "r");
	if (fp_uptime == NULL)
	{
		printf("Unable to read Uptime status!\n");
		return;
	}

	fgets(line, LINE_MAX, fp_uptime);
	double seconds = extract_fp(line);
	printf("Uptime[raw] = %.2fs\n", seconds);

	printf("Uptime[pretty] ");
	if (seconds >= 86400.0)
	{
		// 60s * 60m * 24h
		printf("%.1f day(s)\n", seconds / 86400.0);
	} else if (seconds >= 3600.0)
	{
		// 60s * 60m
		printf("%.1f hour(s)\n", seconds / 3600.0);
	} else if (seconds >= 60.0)
	{
		// 60s
		printf("%.1f minute(s)\n", seconds / 60.0);
	} else {
		// seconds
		printf("%.2f second(s)\n", seconds);
	}

	fclose(fp_uptime);
}

int main(int argc, char *argv[])
{
	// Header
	printf("=== sinq ===\n");
	printf("\"system information querier\"\n\n");

	// CPU
	printf("--- CPU ---\n");
	print_cpu_info();
	print_cpu_freq();
	print_cpu_temp();
	
	// Memory
	printf("\n--- Memory ---\n");
	print_mem_stat();

	// Network
	if (argc > 1 && strcmp(argv[1], "-nn") == 0)
	{
		printf("\n--- Network ---\n");
		print_public_addr();
	}

	printf("\n--- Misc ---\n");
	print_uptime_info();

	printf("=== END ===\n");
}
