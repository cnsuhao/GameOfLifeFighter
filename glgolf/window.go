package glgolf

import (
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

		ww, wh := window.GetSize()

		gl.Viewport(0, 0, ww, wh)
		gl.ClearColor(0.3, 0.3, 0.32, 1.0)
		gl.Clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT | gl.STENCIL_BUFFER_BIT)

		nvg.BeginFrame(ww, wh, 1.0)

		nvg.BeginPath()

		for i := float32(10.0); i <= 600.0; i += 10.0 {
			nvg.MoveTo(i, 10.0)
			nvg.LineTo(i, 600.0)
			nvg.MoveTo(10.0, i)
			nvg.LineTo(600.0, i)
		}

		nvg.StrokeColor(nanovg.RGBA(255, 255, 255, 255))
		nvg.StrokeWidth(1.0)
		nvg.Stroke()
		nvg.ClosePath()

		nvg.EndFrame()

		sdl.GL_SwapWindow(window)
	}

	// Cleanup.
	window.Destroy()
	sdl.GL_DeleteContext(context)
	nvg.Close()
	sdl.Quit()
}
