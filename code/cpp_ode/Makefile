GSLMACINC=/opt/homebrew/Cellar/gsl/2.7.1/include
GSLMACLIB=/opt/homebrew/Cellar/gsl/2.7.1/lib

BOOSTMACINC=/opt/homebrew/Cellar/boost/1.81.0_1/include
BOOSTMACLIB=/opt/homebrew/Cellar/boost/1.81.0_1/lib
BOOSTFLAGS=-Wno-deprecated-declarations

PNG=1gsl/pendulum01.png

all: 1gsl ${PNG}

1gsl: 1gsl.cpp
	g++-12 --std=c++20 -I${GSLMACINC} -L${GSLMACLIB} -lm -lgsl -lblas $< -o $@
	./$@

${PNG}: 1plot.py
	python $<

open:
	open 1gsls/pendulum*.png

2boost: 2boost.cpp Makefile
	g++-12 --std=c++20 -I${BOOSTMACINC} -L${BOOSTMACLIB} -lm -lblas ${BOOSTFLAGS} $< -o $@

clean:
	rm -rf 1gsl 1gsls 2boost
