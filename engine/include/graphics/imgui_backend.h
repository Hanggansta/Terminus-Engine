#pragma once

#ifndef IMGUIBACKEND_H
#define IMGUIBACKEND_H

#include <imgui.h>
#include <Graphics/render_device.h>
#include <SDL.h>

namespace terminus { namespace ImGuiBackend {
	/**
	* Initializes ImGui and creates Graphics Devices. Must be called after initializing Platform and RenderBackends.
	*/
	extern void initialize();
	/**
	* Shuts down ImGui and frees Graphics resources.
	*/
	extern void shutdown();
	/**
	* Prepares ImGui for a New Frame of the gameloop. Must be called at the beginning of the gameloop.
	*/
	extern void new_frame();
	/**
	* Executes ImGui Render Callback.
	*/
	extern void render();
	
    extern bool process_window_events(SDL_Event* event);
	
} }

#endif
