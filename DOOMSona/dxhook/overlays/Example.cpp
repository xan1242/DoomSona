#include "Example.h"
#include "../src/OverlayFramework.h"
//#include "../../Chocolate DOOM/re_main.h"
//#include "../../DoomWin32.h"
#include "../../DoomAPIIntegration.hpp"
#include "../../DoomD3DHook.hpp"
#include <d3d11.h>
#include "../src/IRenderCallback.h"

using namespace OF;
ID3D11Texture2D* pStagingTexture = nullptr;
bool bSetupThingies = false;

void Example::UpdateD3D11Buffer()
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ID3D11DeviceContext* pDeviceContext = nullptr;

	ofDevice.Get()->GetImmediateContext(&pDeviceContext);

	HRESULT hr = pDeviceContext->Map(pStagingTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	if (SUCCEEDED(hr))
	{
		uint32_t sizeX = DoomAPI::DoomScreenTexture::GetScreenWidth();
		uint32_t sizeY = DoomAPI::DoomScreenTexture::GetScreenHeight();

		uint32_t* screen = DoomAPI::DoomScreenTexture::GetScreenFramebuffer();
		int stride = mappedResource.RowPitch; // Use the row pitch for stride

		for (int y = 0; y < sizeY; y++) {
			for (int x = 0; x < sizeX; x++) {
				// Calculate the byte offset for the texel
				int offset = y * stride + x * sizeof(uint32_t);
				uint32_t rgba = screen[(y * sizeX) + x];
				((uint8_t*)mappedResource.pData)[offset] = ((rgba & 0xFF000000) >> 24);
				((uint8_t*)mappedResource.pData)[offset + 1] = ((rgba & 0xFF0000) >> 16);
				((uint8_t*)mappedResource.pData)[offset + 2] = ((rgba & 0xFF00) >> 8);
				((uint8_t*)mappedResource.pData)[offset + 3] = (rgba & 0xFF);
			}
		}


		pDeviceContext->Unmap(pStagingTexture, 0);

	}
#ifdef _DEBUG
	else
	{
		printf("HR: 0x%X\n", hr);
	}
#endif
}

void Example::Setup()
{
	D3D11_VIEWPORT* vp = renderer->GetViewport();

	InitFramework(device, spriteBatch, window);
	box = CreateBox(0, 0, vp->Width, vp->Height);

	doomtex = 0;
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = 424;
	textureDesc.Height = 240;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // Choose the appropriate format
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DYNAMIC; // Allows CPU writes
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // Allows CPU access
	
	HRESULT hr = ofDevice.Get()->CreateTexture2D(&textureDesc, nullptr, &pStagingTexture);
	if (SUCCEEDED(hr))
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	
		srvDesc.Format = textureDesc.Format; // Use the same format as the copy texture
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
	
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture = nullptr;
		ofDevice.Get()->CreateShaderResourceView(pStagingTexture, &srvDesc, texture.GetAddressOf());
		ofTextures.push_back(texture);
		bSetupThingies = true;
	}
#ifdef _DEBUG
	else
	{
		printf("HR2: 0x%X\n", hr);
	}
#endif
}

void Example::Render()
{
	if (bSetupThingies && DoomD3DHook::GetFramebufferEnabled() && DoomAPI::bIsDoomRunning())
	{
		if (!DoomAPI::GetHWND())
		{
			DoomAPI::SetHWND(this->renderer->GetSwapChainDesc()->OutputWindow);
		}

		UpdateD3D11Buffer();
		DrawBox(box, doomtex);
	}
}