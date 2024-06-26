#pragma once

#include <d3d11.h>
#include <vector>
#include <fstream>
//#include <WICTextureLoader.h>
#include "../DirectXTK/Inc/WICTextureLoader.h"
#include "../DirectXTK/Inc/DDSTextureLoader.h"
//#include <DDSTextureLoader.h>
#include <comdef.h>
#include "../DirectXTK/Inc/SpriteBatch.h"
//#include <SpriteFont.h>
#include <wrl/client.h>
#include <chrono>
#include <string>
#include <filesystem>

#include "Logger.h"

#undef DrawText

namespace OF
{
	static struct Box
	{
		int x = 0;
		int y = 0;
		float z = 0.0f;
		int width = 0;
		int height = 0;
		bool pressed = false;
		bool clicked = false;
		bool hover = false;
		bool draggable = true;
		bool hasBeenRendered = false;
		Box* parentBox = nullptr;
	};

#ifdef _DEBUG
	static Logger logger{ "OverlayFramework" };
#endif
	static HWND ofWindow = 0;
	static int ofWindowWidth = 0;
	static int ofWindowHeight = 0;
	static std::vector<Box*> ofBoxes = std::vector<Box*>();
	static constexpr unsigned char UNBOUND = 0x07;
	
	static Microsoft::WRL::ComPtr<ID3D11Device> ofDevice = nullptr;
	static std::shared_ptr<DirectX::SpriteBatch> ofSpriteBatch = nullptr;
	static std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> ofTextures = std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>();
	static std::vector<ID3D11Resource*> ofTextures2 = std::vector<ID3D11Resource*>();
	//static std::vector<std::shared_ptr<DirectX::SpriteFont>> ofFonts = std::vector<std::shared_ptr<DirectX::SpriteFont>>();
	//static std::shared_ptr<DirectX::SpriteFont> ofActiveFont = nullptr;

	// Gives the framework the required DirectX objects to draw
	static void InitFramework(
		Microsoft::WRL::ComPtr<ID3D11Device> device,
		std::shared_ptr<DirectX::SpriteBatch> spriteBatch,
		HWND window)
	{
#ifdef _DEBUG
		logger.Log("Initialized");
#endif
		ofDevice = device;
		ofSpriteBatch = spriteBatch;
		ofWindow = window;

		RECT hwndRect;
		GetClientRect(ofWindow, &hwndRect);
		ofWindowWidth = hwndRect.right - hwndRect.left;
		ofWindowHeight = hwndRect.bottom - hwndRect.top;
	}

	static int MapIntToRange(
		int number,
		int inputStart,
		int inputEnd,
		int outputStart,
		int outputEnd)
	{
		return outputStart + (outputEnd - outputStart) * (number - inputStart) / (inputEnd - inputStart);
	}

	static float MapFloatToRange(
		float number,
		float inputStart,
		float inputEnd,
		float outputStart,
		float outputEnd)
	{
		return outputStart + (outputEnd - outputStart) * (number - inputStart) / (inputEnd - inputStart);
	}

	//static int LoadTexture(std::string filepath)
	static int LoadTexture(std::filesystem::path filepath)
	{
		if (ofDevice.Get() == nullptr)
		{
#ifdef _DEBUG
			logger.Log("Could not load texture, ofDevice is nullptr! Run InitFramework before attempting to load textures!");
#endif
			return -1;
		}

		//if (ofTextures.size() == 0 && filepath != "blank") {
		//	if (LoadTexture("blank") != 0)
		//	{
		//		return -1;
		//	}
		//}
		//else if (filepath == "blank")
		//{
		//	filepath = "hook_textures\\blank.jpg";
		//}
#ifdef _DEBUG
		logger.Log("Loading texture: %s", filepath.string().c_str());
#endif
		//std::wstring wideString(filepath.length(), ' ');
		//std::copy(filepath.begin(), filepath.end(), wideString.begin());
		std::fstream file = std::fstream(filepath);
		if (file.fail())
		{
#ifdef _DEBUG
			logger.Log("Texture loading failed, file not found: %s", filepath.string().c_str());
#endif
			file.close();
			return -1;
		}
		file.close();

		HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
#ifdef _DEBUG
		if (FAILED(hr))
		{
			logger.Log("Error %#010x when initializing the COM library", hr);
		}
		else
		{
			logger.Log("Successfully initialized the COM library");
		}
#endif
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture = nullptr;
		ID3D11Resource* res = nullptr;
		HRESULT texResult = 0;

		if (filepath.extension() == ".dds")
			texResult = DirectX::CreateDDSTextureFromFile(ofDevice.Get(), (wchar_t*)filepath.u16string().c_str(), &res, texture.GetAddressOf());
		else
			texResult = DirectX::CreateWICTextureFromFile(ofDevice.Get(), (wchar_t*)filepath.u16string().c_str(), nullptr, texture.GetAddressOf());


		//_com_error texErr(texResult);
		//logger.Log("Texture HRESULT: %s", texErr.ErrorMessage());
		//if (FAILED(texResult))
		//{
		//	logger.Log("Texture loading failed: %s", filepath.c_str());
		//	return -1;
		//}

		ofTextures.push_back(texture);
		ofTextures2.push_back(res);
		//printf("Texture: 0x%X\n", );

		//Sleep(10);

		//const char* p = reinterpret_cast<const char*>(texture.GetAddressOf() + 0x100);
		//std::stringstream dumpstream("");
		//for (unsigned int i = 0; i < 0x100; i++) {
		//	if ((int(p[i]) & 0xFF) < 0xF)
		//		dumpstream << "0";
		//	dumpstream << std::uppercase << std::hex << (int(p[i]) & 0xFF) << " ";
		//}
		//printf("%s", dumpstream.str().c_str());

		return ofTextures.size() - 1;
	}


	static void PlaceOnTop(Box* box)
	{
		static std::vector<int> ofBoxOrder = std::vector<int>();
		size_t boxIndex = 0;
		for (size_t i = 0; i < ofBoxes.size(); i++)
		{
			if (ofBoxes[i] == box)
			{
				boxIndex = i;
				break;
			}
		}

		ofBoxOrder.push_back(boxIndex);
		for (size_t i = 0; i < ofBoxOrder.size() - 1; i++)
		{
			if (ofBoxes[ofBoxOrder[i]] == ofBoxes[ofBoxOrder.back()])
			{
				ofBoxOrder.erase(ofBoxOrder.begin() + i);
			}
		}

		for (float i = 0; i < ofBoxOrder.size(); i++)
		{
			ofBoxes[ofBoxOrder[i]]->z = 1.0f / (1 + (i / 1000));
		}
	}

	static POINT GetAbsolutePosition(Box* box)
	{
		if (box == nullptr)
		{
			return { 0, 0 };
		}

		POINT absolutePosition = { box->x, box->y };
		Box* parentBox = box->parentBox;
		while (parentBox != nullptr)
		{
			if (parentBox->parentBox == box)
			{
				break;
			}

			absolutePosition.x += parentBox->x;
			absolutePosition.y += parentBox->y;

			parentBox = parentBox->parentBox;
		}

		return absolutePosition;
	}

	static Box* CreateBox(Box* parentBox, int x, int y, int width, int height)
	{
		Box* box = new Box;
		box->x = x;
		box->y = y;
		box->width = width;
		box->height = height;
		box->parentBox = parentBox;

		if (parentBox != nullptr)
		{
			box->draggable = false;
		}

		ofBoxes.push_back(box);
		PlaceOnTop(box);
		return ofBoxes.back();
	}

	static Box* CreateBox(int x, int y, int width, int height)
	{
		return CreateBox(nullptr, x, y, width, height);
	}
	
	static void _DrawBox(Box* box, DirectX::XMVECTOR color, int textureID)
	{
		static bool ofFailedToLoadBlank = false;

		if (box == nullptr) 
		{
#ifdef _DEBUG
			logger.Log("Attempted to render a nullptr Box!");
#endif
			return;
		}

		if (ofSpriteBatch == nullptr)
		{
#ifdef _DEBUG
			logger.Log("Attempted to render with ofSpriteBatch as nullptr! Run InitFramework before attempting to draw!");
#endif
			return;
		}

		if (ofTextures.size() < 1) 
		{
			if (ofFailedToLoadBlank == false) 
			{
				if (LoadTexture("blank") != 0)
				{
					ofFailedToLoadBlank = true;
					return;
				}
			}
			else
			{
				return;
			}
		}

		if (textureID < 0 || textureID > ofTextures.size() - 1) 
		{
#ifdef _DEBUG
			logger.Log("'%i' is an invalid texture ID!", textureID);
#endif
			return;
		}
	
		POINT position = GetAbsolutePosition(box);

		RECT rect;
		rect.top = position.y;
		rect.left = position.x;
		rect.bottom = position.y + box->height;
		rect.right = position.x + box->width;

		box->hasBeenRendered = true;
		ofSpriteBatch->Draw(ofTextures[textureID].Get(), rect, nullptr, color, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::SpriteEffects_None, box->z);
	}

	static void DrawBox(Box* box, int textureID)
	{
		_DrawBox(box, { 1.0f, 1.0f, 1.0f, 1.0f }, textureID);
	}

	static void DrawBox(Box* box, int r, int g, int b, int a = 255)
	{
		float _r = MapFloatToRange((float)r, 0.0f, 255.0f, 0.0f, 1.0f);
		float _g = MapFloatToRange((float)g, 0.0f, 255.0f, 0.0f, 1.0f);
		float _b = MapFloatToRange((float)b, 0.0f, 255.0f, 0.0f, 1.0f);
		float _a = MapFloatToRange((float)a, 0.0f, 255.0f, 0.0f, 1.0f);
		_DrawBox(box, { _r, _g, _b, _a }, 0);
	}

	static bool IsCursorInsideBox(POINT cursorPos, Box* box)
	{
		POINT position = GetAbsolutePosition(box);
		POINT boxSize = { box->width, box->height };

		if (cursorPos.x < (position.x + boxSize.x) && cursorPos.x > position.x)
		{
			if (cursorPos.y < (position.y + boxSize.y) && cursorPos.y > position.y)
			{
				return true;
			}
		}

		return false;
	}

	static bool CheckHotkey(unsigned char key, unsigned char modifier = UNBOUND)
	{
		static std::vector<unsigned char> notReleasedKeys;

		if (ofWindow != GetForegroundWindow())
		{
			return false;
		}

		bool keyPressed = GetAsyncKeyState(key) & 0x8000;
		bool modifierPressed = GetAsyncKeyState(modifier) & 0x8000;

		if (key == UNBOUND)
		{
			return modifierPressed;
		}

		auto iterator = std::find(notReleasedKeys.begin(), notReleasedKeys.end(), key);
		bool keyNotReleased = iterator != notReleasedKeys.end();

		if (keyPressed && keyNotReleased)
		{
			return false;
		} 
		
		if(!keyPressed)
		{
			if (keyNotReleased)
			{
				notReleasedKeys.erase(iterator);
			}
			return false;
		}

		if (modifier != UNBOUND && !modifierPressed)
		{
			return false;
		}

		notReleasedKeys.push_back(key);
		return true;
	}

	static void CheckMouseEvents()
	{
		static int ofMouseX = 0, ofMouseY = 0;
		static int ofDeltaMouseX = 0, ofDeltaMouseY = 0;
		static bool ofMousePressed = false;
		static Box* ofClickedBox = nullptr;

		if (ofWindow == GetForegroundWindow())
		{
			POINT cursorPos;
			GetCursorPos(&cursorPos);
			ScreenToClient(ofWindow, &cursorPos);

			ofDeltaMouseX = ofMouseX;
			ofDeltaMouseY = ofMouseY;
			ofMouseX = cursorPos.x;
			ofMouseY = cursorPos.y;
			ofDeltaMouseX = ofDeltaMouseX - ofMouseX;
			ofDeltaMouseY = ofDeltaMouseY - ofMouseY;

			if (ofClickedBox != nullptr)
			{
				if (ofClickedBox->clicked)
				{
					ofClickedBox->clicked = false;
					ofClickedBox = nullptr;
				}
			}

			Box* topMostBox = nullptr;
			for (size_t i = 0; i < ofBoxes.size(); i++)
			{
				Box* box = ofBoxes[i];
				box->hover = false;

				if (!box->hasBeenRendered)
				{
					continue;
				}

				if (IsCursorInsideBox(cursorPos, box))
				{
					if (topMostBox == nullptr || box->z < topMostBox->z)
					{
						topMostBox = box;
					}
				}
			}

			if (topMostBox != nullptr)
			{
				topMostBox->hover = true;
			}

			if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
			{
				ofMousePressed = true;

				if (topMostBox != nullptr)
				{
					ofClickedBox = topMostBox;
					ofClickedBox->pressed = true;
				}

				if (ofClickedBox != nullptr && ofClickedBox->draggable)
				{
					ofClickedBox->x -= ofDeltaMouseX;
					ofClickedBox->y -= ofDeltaMouseY;
				}
			}
			else
			{
				ofMousePressed = false;

				if (ofClickedBox != nullptr && IsCursorInsideBox(cursorPos, ofClickedBox))
				{
					if (ofClickedBox->parentBox != nullptr)
					{
						PlaceOnTop(ofClickedBox->parentBox);

						for (size_t i = 0; i < ofBoxes.size(); i++)
						{
							if (ofClickedBox->parentBox == ofBoxes[i]->parentBox)
							{
								PlaceOnTop(ofBoxes[i]);
							}
						}
					}

					PlaceOnTop(ofClickedBox);

					for (size_t i = 0; i < ofBoxes.size(); i++)
					{
						if (ofBoxes[i]->parentBox == ofClickedBox)
						{
							PlaceOnTop(ofBoxes[i]);
						}
					}

					ofClickedBox->pressed = false;
					ofClickedBox->clicked = true;
				}
			}
		}

		for (auto box : ofBoxes)
		{
			box->hasBeenRendered = false;
		}
	}
};