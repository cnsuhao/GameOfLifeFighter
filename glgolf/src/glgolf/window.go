package glgolf

import (
	"github.com/veandco/go-sdl2/sdl"
)

func InitGame() {
	sdl.Init(sdl.INIT_EVERYTHING)

	window, err := sdl.CreateWindow("glgolf",
		sdl.WINDOWPOS_UNDEFINED,
		sdl.WINDOWPOS_UNDEFINED,
		800, 600,
		sdl.WINDOW_SHOWN)
	if err != nil {
		panic(err)
	}
	defer window.Destroy()

	surface, err := window.GetSurface()
	if err != nil {
		panic(err)
	}

	running := true

	for running {
		for event := sdl.PollEvent(); event != nil; event = sdl.PollEvent() {
			switch t := event.(type) {
			case *sdl.QuitEvent:
				running = false
			case *sdl.KeyUpEvent:
				if t.Keysym.Sym == sdl.K_ESCAPE {
					running = false
				}
			}
		}

		rect := sdl.Rect{0, 0, 200, 200}
		surface.FillRect(&rect, 0xffff0000)
		window.UpdateSurface()
	}

	sdl.Quit()
}
