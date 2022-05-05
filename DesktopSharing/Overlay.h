#ifndef OVERLAY_H
#define OVERLAY_H

#include "SDL.h"
#include "imgui_latest/imgui.h"
#include "imgui_latest/imgui_impl_dx9.h" 
#include "imgui_latest/imgui_impl_sdl.h"
#include "imgui_latest/imgui_impl_glfw.h"
#include "imgui_latest/imgui_impl_opengl3.h"
#include <d3d9.h>
//#include <SDL_opengl.h>
#include <string>
#include <vector>

#include <GL/gl3w.h>            // Initialize with gl3wInit()
#include <GLFW/glfw3.h>

enum OverlayEventType
{
	EVENT_TYPE_RTSP_SERVER = 0x001,
	EVENT_TYPE_RTSP_PUSHER = 0x002,
	EVENT_TYPE_RTMP_PUSHER = 0x003,
};

class OverlayCallack
{
public:
	virtual bool StartLive(int& event_type, 
		std::vector<std::string>& encoder_settings,
		std::vector<std::string>& live_settings) = 0;

	virtual void StopLive(int event_type) = 0;

//protected:
	virtual ~OverlayCallack() {};
};

class Overlay
{
public:
	Overlay();
	virtual ~Overlay();

	void RegisterObserver(OverlayCallack* callback);

	bool Init(SDL_Window* window, IDirect3DDevice9* device);
	bool Init(SDL_Window* window, SDL_GLContext gl_context);

	void SetRect(int x, int y, int w, int h);
	void Destroy();
	bool Render();

	static void Process(SDL_Event* event);

	void SetLiveState(int event_type, bool state);
	void SetDebugInfo(std::string text);

private:
	void Init();
	bool Copy();
	bool Begin();
	bool End();
	void NotifyEvent(int event_type);

	SDL_Window* window_ = nullptr;
	IDirect3DDevice9* device_ = nullptr;
	SDL_GLContext gl_context_ = nullptr;

	SDL_Rect rect_;

	OverlayCallack* callback_ = nullptr;

	/* debug info */
	std::string debug_info_text_;

	/* live config */
	int  encoder_index_ = 1;
	char encoder_bitrate_kbps_[8];
	char encoder_framerate_[3];

	struct LiveInfo {
		char server_ip[16];
		char server_port[6];
		char server_stream[16];
		char pusher_url[60];

		bool state = false;
		char state_info[16];
	} live_info_[10] ;
};

#endif