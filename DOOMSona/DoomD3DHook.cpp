#include "framework.h"
#include "DoomD3DHook.hpp"
#include "dxhook/src/Renderer.h"
#include "dxhook/src/DirectXHook.h"
#include "dxhook/overlays/Example.h"

namespace DoomD3DHook
{
	static bool bFramebufferEnabled;

	bool GetFramebufferEnabled()
	{
		return bFramebufferEnabled;
	}

	void SetFramebufferEnabled(bool val)
	{
		bFramebufferEnabled = val;
	}

	void Init()
	{
		static Renderer renderer;
		static DirectXHook dxHook(&renderer);
		static Example example;

		dxHook.AddRenderCallback(&example);
		dxHook.Hook();

		example.renderer = &renderer;
	}
}
