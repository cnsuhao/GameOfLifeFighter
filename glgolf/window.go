package glgolf

import (
	"fmt"
	"github.com/FSX/nanovg"
	"github.com/FSX/nanovg/gl"
	"github.com/veandco/go-sdl2/sdl"
)

// initSDL initializes SDL and creates a window.
func initSDL() *sdl.Window {
	sdl.Init(sdl.INIT_EVERYTHING)

	window, err := sdl.CreateWindow("glgolf",
		sdl.WINDOWPOS_UNDEFINED,
		sdl.WINDOWPOS_UNDEFINED,
		1280, 720,
		sdl.WINDOW_OPENGL)
	if err != nil {
		panic(err)
	}

	return window
}

// initGL sets OpenGL attributes, creates a context and initializes GLEW.
func initGL(window *sdl.Window) sdl.GLContext {
	// sdl.GL_SetAttribute(sdl.GL_CONTEXT_FLAGS, sdl.GL_CONTEXT_DEBUG_FLAG)
	sdl.GL_SetAttribute(sdl.GL_CONTEXT_PROFILE_MASK, sdl.GL_CONTEXT_PROFILE_CORE)
	sdl.GL_SetAttribute(sdl.GL_CONTEXT_MAJOR_VERSION, 3)
	sdl.GL_SetAttribute(sdl.GL_CONTEXT_MINOR_VERSION, 2)
	sdl.GL_SetAttribute(sdl.GL_CONTEXT_FLAGS, sdl.GL_CONTEXT_FORWARD_COMPATIBLE_FLAG)
	sdl.GL_SetAttribute(sdl.GL_ACCELERATED_VISUAL, 1)
	sdl.GL_SetAttribute(sdl.GL_DOUBLEBUFFER, 1)
	sdl.GL_SetAttribute(sdl.GL_DEPTH_SIZE, 24)

	context, err := sdl.GL_CreateContext(window)
	if err != nil {
		panic(err)
	}

	if err := gl.InitGlew(); err != nil {
		panic(err)
	}

	return context
}

// initNanovg initializes nanovg.
func initNanovg() *nanovg.Context {
	// DEBUG: nanovg.DEBUG
	nvg, err := nanovg.CreateCtx(nanovg.ANTIALIAS | nanovg.STENCIL_STROKES | nanovg.DEBUG)
	if err != nil {
		panic(err)
	}

	return nvg
}

// StartGame initializes all dependencies and starts the game loop.
func StartGame() {
	// Initialize all dependencies.
	window := initSDL()
	context := initGL(window)
	nvg := initNanovg()

	ww, wh := window.GetSize()
	gw := ww / 5
	gh := wh / 5
	fmt.Println(gw, gh)

	life := NewLife(gw, gh)

	refgrid := make([][]bool, gh)
	for i := range refgrid {
		refgrid[i] = make([]bool, gw)
	}

	running := true
	minms := uint32(1000 / 15)
	fmt.Println(minms)

	gl.ClearColor(0.3, 0.3, 0.32, 1.0)
	gl.Clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT | gl.STENCIL_BUFFER_BIT)

	for running {
		lt := sdl.GetTicks()

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

		life.Step()

		ww, wh := window.GetSize()
		gl.Viewport(0, 0, ww, wh)

		nvg.BeginFrame(ww, wh, 1.0)

		drawGrid(life, nvg, refgrid)

		nvg.EndFrame()

		if d := sdl.GetTicks() - lt; d < minms {
			fmt.Println("Fast", d)
			sdl.Delay(minms - d)
		} else {
			fmt.Println("Slow", d)
		}

		sdl.GL_SwapWindow(window)
	}

	// Cleanup.
	window.Destroy()
	sdl.GL_DeleteContext(context)
	nvg.Close()
	sdl.Quit()
}

func drawGrid(life *Life, nvg *nanovg.Context, refgrid [][]bool) {
	w, h := float32(life.w), float32(life.h)
	counter := 0
	liveCounter := 0
	deadCounter := 0

	for y := float32(0); y < h; y++ {
		for x := float32(0); x < w; x++ {
			a, b := int(x), int(y)
			c := life.a.s[b][a]

			if refgrid[b][a] != c {
				nvg.BeginPath()
				nvg.Rect(x*5+1, y*5+1, 3, 3)

				if c {
					liveCounter++
					nvg.FillColor(nanovg.RGBA(255, 255, 255, 255))
				} else {
					nvg.FillColor(nanovg.RGBA(100, 100, 100, 255))
					deadCounter++
				}

				nvg.Fill()
				nvg.ClosePath()

				counter++
			}

			refgrid[b][a] = c
		}
	}
}
