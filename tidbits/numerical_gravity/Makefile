build:
	python pythagorean.py

video:
	ffmpeg -y -r 10 -i 'plots/%03d.png' pythagorean.mp4

run: build video
