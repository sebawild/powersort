#! /bin/sh
BUILDDIR=cmake-build-release
mkdir -p $BUILDDIR
cd $BUILDDIR
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
cd ..

PREFIX="taskset -c 0 $BUILDDIR/src"
SEED=439569436534

## Usage: mergesorts [reps] [n1,n2,n3] [inputs] [contestants] [seed] [outfile]

echo "Experiment 2a: 10^7 ints distribution, random runs"

for runs in 3 30 300 3000 30000 300000 3000000
do
  ${PREFIX}/mergesorts 3  10000000 runs$runs '*' ${SEED} times-runs$runs-10m-int-dist    >> times-runs$runs-int.out
done



echo "Experiment 1: int, random runs, various n"

${PREFIX}/mergesorts 1001     10000 runs-sqrtn '*' ${SEED} times-runs-10k-int    >> times-runs-int.out
${PREFIX}/mergesorts 1001    100000 runs-sqrtn '*' ${SEED} times-runs-100k-int   >> times-runs-int.out
${PREFIX}/mergesorts 1001   1000000 runs-sqrtn '*' ${SEED} times-runs-1m-int     >> times-runs-int.out
${PREFIX}/mergesorts  101  10000000 runs-sqrtn '*' ${SEED} times-runs-10m-int    >> times-runs-int.out
${PREFIX}/mergesorts  101 100000000 runs-sqrtn '*' ${SEED} times-runs-100m-int   >> times-runs-int.out


echo "Experiment 2: 10^7 ints distribution, random runs"

${PREFIX}/mergesorts 1001  10000000 runs-sqrtn '*' ${SEED} times-runs3k-10m-int-dist    >> times-runs3k-int.out


echo "Experiment 3: long+pointer, random runs, various n"

${PREFIX}/mergesorts-long+pointer 1001     10000 runs-sqrtn '*' ${SEED} times-runs-10k-l+p    >> times-runs-l+p.out
${PREFIX}/mergesorts-long+pointer 1001    100000 runs-sqrtn '*' ${SEED} times-runs-100k-l+p   >> times-runs-l+p.out
${PREFIX}/mergesorts-long+pointer 1001   1000000 runs-sqrtn '*' ${SEED} times-runs-1m-l+p     >> times-runs-l+p.out
${PREFIX}/mergesorts-long+pointer  101  10000000 runs-sqrtn '*' ${SEED} times-runs-10m-l+p    >> times-runs-l+p.out
${PREFIX}/mergesorts-long+pointer  101 100000000 runs-sqrtn '*' ${SEED} times-runs-100m-l+p   >> times-runs-l+p.out


echo "Experiment 4: int, random permutations, various n"

${PREFIX}/mergesorts 1001     10000 random-permutations '*' ${SEED} times-rp-10k-int    >> times-rp-int.out
${PREFIX}/mergesorts 1001    100000 random-permutations '*' ${SEED} times-rp-100k-int   >> times-rp-int.out
${PREFIX}/mergesorts 1001   1000000 random-permutations '*' ${SEED} times-rp-1m-int     >> times-rp-int.out
${PREFIX}/mergesorts  101  10000000 random-permutations '*' ${SEED} times-rp-10m-int    >> times-rp-int.out
${PREFIX}/mergesorts  101 100000000 random-permutations '*' ${SEED} times-rp-100m-int   >> times-rp-int.out


echo "Experiment 5: count comparisons and merge cost, random runs, various n"

${PREFIX}/mergesorts-count-cmps 1001     10000 runs-sqrtn '*' ${SEED} times-runs-10k-cmps    >> times-runs-cmps.out
${PREFIX}/mergesorts-count-cmps 1001    100000 runs-sqrtn '*' ${SEED} times-runs-100k-cmps   >> times-runs-cmps.out
${PREFIX}/mergesorts-count-cmps 1001   1000000 runs-sqrtn '*' ${SEED} times-runs-1m-cmps     >> times-runs-cmps.out
${PREFIX}/mergesorts-count-cmps  101  10000000 runs-sqrtn '*' ${SEED} times-runs-10m-cmps    >> times-runs-cmps.out
${PREFIX}/mergesorts-count-cmps  101 100000000 runs-sqrtn '*' ${SEED} times-runs-100m-cmps   >> times-runs-cmps.out



echo "Experiment 6: Cachegrind"

BUILDDIR=cmake-build-relwithdebuginfo
mkdir -p $BUILDDIR
cd $BUILDDIR
cmake -D CMAKE_BUILD_TYPE=RelWithDebInfo ..
cmake --build .
cd ..
for algo in 0 1 2 3 5
do 
	valgrind --tool=callgrind --simulate-cache=yes  ${BUILDDIR}/src/mergesorts 1 100000000 runs-sqrtn $algo | tee -a cachegrind-100m-ints
done
