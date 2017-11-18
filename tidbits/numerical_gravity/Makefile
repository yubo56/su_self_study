build:
	mkdir -p plots
	python pythagorean.py

video:
	ffmpeg -y -r 10 -i 'plots/%03d.png' pythagorean.mp4
	rm -rf plots

run: build video
