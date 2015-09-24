glgolf
======

Game of Life in Go, rendered with nanovg.

First [install Go](https://golang.org/doc/install),
and [SDL2](https://www.libsdl.org/download-2.0.php),
make sure `$GOPATH` is set, then do this:

	# Install dependencies:
	go get github.com/veandco/go-sdl2/sdl
	go get github.com/FSX/nanovg

	# Install glgolf:
	mkdir "$GOPATH/src/github.com/LucKeyProductions"
	cd "$GOPATH/src/github.com/LucKeyProductions"
	git clone https://github.com/LucKeyProductions/GameOfLifeFighter.git
	cd GameOfLifeFighter
	go install ./...

	# Run glgolf:
	$GOPATH/bin/glgolf
