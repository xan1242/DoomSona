#include "Example.h"
//#include "../DoomBind.hpp"
#include "../src/DoomAPI.hpp"

using namespace OF;
ID3D11Texture2D* pStagingTexture = nullptr;
bool bSetupThingies = false;

uint32_t testBuf[424 * 240];

void LoadTextureFromFile()
{
	FILE* f = fopen("test.dds", "rb");

	fseek(f, 0x80, SEEK_SET);
	fread(testBuf, sizeof(uint32_t), 424 * 240, f);

	fclose(f);
}

void Example::UpdateD3D11Buffer()
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ID3D11DeviceContext* pDeviceContext = nullptr;

	ofDevice.Get()->GetImmediateContext(&pDeviceContext);

	//HRESULT hr = pDeviceContext->Map((ID3D11Resource*)ofTextures[0].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	HRESULT hr = pDeviceContext->Map(pStagingTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	if (SUCCEEDED(hr))
	{
		uint32_t sizeX = DoomAPI::GetScreenX();
		uint32_t sizeY = DoomAPI::GetScreenY();

		uint32_t* screen = DoomAPI::GetScreenBuffer();
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
	else
	{
		printf("HR: 0x%X\n", hr);
	}
}

void Example::Setup()
{
	D3D11_VIEWPORT* vp = renderer->GetViewport();

	InitFramework(device, spriteBatch, window);
	//box = CreateBox(100, 100, 100, 100);
	//box = CreateBox(0, 0, 848, 480);
	box = CreateBox(0, 0, vp->Width, vp->Height);
	//doomtex = LoadTexture("test.dds");
	//LoadTextureFromFile();
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
	else
	{
		printf("HR2: 0x%X\n", hr);
	}
}

void Example::Render()
{
	if (bSetupThingies && DoomAPI::GetFramebufferEnabled())
	{
		CheckMouseEvents();
		UpdateD3D11Buffer();



		DrawBox(box, doomtex);

		//RECT rect;
		//rect.top = 0;
		//rect.left = 0;
		//rect.bottom = 240;
		//rect.right = 424;
		//
		//box->hasBeenRendered = true;
		//ofSpriteBatch->Draw(tex2, rect, nullptr, { 1.0f, 1.0f, 1.0f, 1.0f }, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::SpriteEffects_None);
	}
	//DrawBox(box, 255, 0, 0, 255);
}