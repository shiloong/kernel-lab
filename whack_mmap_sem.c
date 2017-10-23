#define _GNU_SOURCE
#include <stdio.h>
#include <errno.h>
#include <sched.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <getopt.h>
#include <pthread.h>

#define nano2sec(nan) (nan / 1000000000ULL)
#define nano2ms(nan) (nan / 1000000ULL)
#define nano2usec(nan) (nan / 1000ULL)
#define usec2nano(sec) (sec * 1000ULL)
#define ms2nano(ms) (ms * 1000000ULL)
#define sec2nano(sec) (sec * 1000000000ULL)
#define sec2usec(sec) (sec * 1000000ULL)

#define DEF_BUFFER_SIZE (4096ULL * 1024 * 1024)
#define DEF_NUMBER_WRITES 10000

unsigned long long buffersize = DEF_BUFFER_SIZE;
int nthreads = 0;
int nwrites = DEF_NUMBER_WRITES;
int ncpus;
char *buffer;
int verbose = 0;
int debugging = 0;

pthread_barrier_t start_barrier;
pthread_barrier_t stop_barrier;

void usage(void)
{
	printf("usage: whack_mmap_sem [options]\n\n");
 	printf("  starts some number of threads that randomly write in a large memory region\n\n");
	printf("    options:\n");
	printf("        -t|--threads <n> : number of threads to start\n");
	printf("        -m|--memory <n>  : size of memory buffer\n");
	printf("        -w|--writes <n>  : number of writes each thread should do\n");
	printf("        -v|--verbose     : extra info\n");
	printf("        -d|--debug       : mostly useless debug prints\n");
	printf("        -h|--help        : print this message\n");
}

void process_cmdline(int argc, char **argv)
{
	static struct option long_options[] = {
		{"threads",  required_argument,    NULL,    't'},
		{"memory",   required_argument,    NULL,    'm'},
		{"writes",   required_argument,    NULL,    'w'},
		{"verbose",  no_argument,          NULL,    'v'},
		{"debug",    no_argument,          NULL,    'd'},
		{"help",     no_argument,          NULL,    'h'},
		{NULL,       0,                    NULL,     0},
	};
	int c;
	int option_index = 0;

	for (;;) {
		c = getopt_long(argc, argv, "t:m:w:vdh", long_options, &option_index);

		if (c == -1)
			break;

		switch(c) {
		case 't':
			nthreads = strtol(optarg, NULL, 10);
			break;
		case 'm':
			buffersize = (unsigned long long) strtoll(optarg, NULL, 10);
			break;
		case 'w':
			nwrites = strtol(optarg, NULL, 10);
			break;
		case 'v':
			verbose = 1;
			break;
		case 'd':
			debugging = 1;
			break;
		case 'h':
		default:
			usage();
			exit(-1);
		}
	}
	/* if --threads not specified, default to ncpus * 2 */
	if (!nthreads)
		nthreads = ncpus * 2;
}


int getncpus(void)
{
	return sysconf(_SC_NPROCESSORS_ONLN);
}

void initialize_barriers(void)
{
	if (pthread_barrier_init(&start_barrier, NULL, nthreads+1) < 0) {
		perror("start barrier");
		exit(errno);
	}

	if (pthread_barrier_init(&stop_barrier, NULL, nthreads+1) < 0) {
		perror("stop barrier");
		exit(errno);
	}
}


void debug(char *fmt, ...) {
	va_list ap;

	if (debugging) {
		va_start (ap, fmt);
		vprintf(fmt, ap);
		va_end(ap);
	}
}

void info(char *fmt, ...) {
	va_list ap;

	if (verbose) {
		va_start (ap, fmt);
		vprintf(fmt, ap);
		va_end(ap);
	}
}


void *thread_routine(void *data)
{
	int i;
	unsigned long long int idx;
	cpu_set_t cpuset;
	long cpu = (long) data;

	CPU_ZERO_S(ncpus, &cpuset);
	CPU_SET_S(cpu, ncpus, &cpuset);

	pthread_setaffinity_np (pthread_self(), ncpus, &cpuset);

	pthread_barrier_wait(&start_barrier);
	for (i=0; i < nwrites; i++) {
		idx = random() % buffersize;
		debug ("writing to index %llu\n", idx);
		buffer[idx]++;
	}
	pthread_barrier_wait(&stop_barrier);

	return NULL;
}

static unsigned long long get_time(void)
{
	struct timeval tv;
	unsigned long long time;

	gettimeofday(&tv, NULL);

	time = sec2usec(tv.tv_sec);
	time += tv.tv_usec;

	return time;
}


void run_test(void)
{
	pthread_t t[nthreads];
	int i;
	long cpu;
	unsigned long long start, end;

	info("starting %d threads on %d cpus for %d writes each\n", nthreads, ncpus, nwrites);
	info("buffer size is %llu bytes (%llu pages)\n", buffersize, (buffersize / 4096));

	/* start the threads */
	for (i = cpu = 0; i < nthreads; i++) {
		info("starting thread on cpu %d\n", cpu);
		pthread_create(&t[i], NULL, thread_routine, (void *) cpu);
		++cpu;
		cpu %= ncpus;
	}

	/* get the start time */
	start = get_time();

	/* now start the threads */
	info("starting threads\n");
	pthread_barrier_wait(&start_barrier);

	/* wait for everyone to finish */
	pthread_barrier_wait(&stop_barrier);
	info("threads stopped\n");

	/* get the end time */
	end = get_time();

	/* report */
	printf("%lld us\n", end - start);

	/* clean up the threads */
	for (i = 0; i < nthreads; i++)
		pthread_join(t[i], NULL);

}

int main(int argc, char **argv)
{

	/* figure out how many cpus online */
	ncpus = getncpus();

	/* process command line arguments */
	process_cmdline(argc, argv);

	srandom(buffersize);

	/* initialize our barriers */
	initialize_barriers();

	/* allocate the buffer */
	if ((buffer = malloc(buffersize)) == NULL) {
		perror("malloc");
		exit(ENOMEM);
	}

	run_test();

	/* clean up */
	if (buffer)
		free(buffer);
	exit(0);
}
