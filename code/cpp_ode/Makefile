MACINC=/opt/homebrew/Cellar/gsl/2.7.1/include
MACLIB=/opt/homebrew/Cellar/gsl/2.7.1/lib

PNG=1tests/pendulum01.png

all: 1test ${PNG}

1test: 1test.cpp
	g++-12 --std=c++20 -I${MACINC} -L${MACLIB} -lm -lgsl -lblas $< -o $@
	./$@

${PNG}: 1plot.py
	python $<

open:
	open 1tests/pendulum*.png

clean:
	rm -rf 1test 1tests/
