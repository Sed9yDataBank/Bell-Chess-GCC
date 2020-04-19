CC = gcc 
OBJS = board.o evaluate.o main.o bestmove.o moves.o loops.o
FLAGS = -pthread -std=c99

all: 'Bell Chess GCC'

xboard: FLAGS += -DXBOARD -Ofast
xboard: all

log-xboard: FLAGS += -g -Ofast -DXBOARD -DLOGGING
log-xboard: all

debug: FLAGS += -O0 -g
debug: all

testdebug: FLAGS+= -O0 -g -DTESTING
testdebug: OBJS+= testing.o
testdebug: testing.o all

optimised: FLAGS += -O3
optimised: 'Bell Chess GCC'

fast: FLAGS += -Ofast
fast: 'Bell Chess GCC'

profiling: CC = clang
profiling: FLAGS += -g -pg -O0
profiling: all

'Bell Chess GCC': $(OBJS)
	$(CC) -o 'Bell Chess GCC' $(FLAGS) $(OBJS)

%.o: %.c common.h 
	$(CC) $(FLAGS) -c $< -o $@
#ai.h Is Not Added, Still Need To Add More Algorithms And It Is Not Compeleted Yet To Play Against Machine
clean:
	-@rm *.o 2>/dev/null || echo "Cleaning object files"
	-@rm 'Bell Chess GCC' 2>/dev/null || echo "Cleaning Main executable"
	-@rm logfile* 2>/dev/null || echo "Cleaning logfiles"
	-@rm data.txt 2>/dev/null || echo "Cleaning data files"
	-@rm gmon.out 2>/dev/null || echo "Cleaning profiler output"
	-@rm callgrind* 2>/dev/null || echo "Cleaning call graphs"
	@echo "Cleaned all"
