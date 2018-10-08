#include "stdafx.h"
#include "SpriteFontLoader.h"
#include "BinaryReader.h"
#include "ContentManager.h"
#include "TextureData.h"

SpriteFont* SpriteFontLoader::LoadContent(const std::wstring& assetFile)
{
	auto binReader = new BinaryReader();
	binReader->Open(assetFile);

	if (!binReader->Exists())
	{
		Logger::LogFormat(LogLevel::Error, L"SpriteFontLoader::LoadContent > Failed to read the assetFile!\nPath: \'%s\'", assetFile.c_str());
		return nullptr;
	}

	const auto id0 = binReader->Read<char>();
	const auto id1 = binReader->Read<char>();
	const auto id2 = binReader->Read<char>();
	const auto version = binReader->Read<char>();

	if (id0 != 66 && id1 != 77 && id2 != 70)
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > Not a valid .fnt font!");
		return nullptr;
	}

	if (version != 3)
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > Only .fnt version 3 is supported!");
		return nullptr;
	}

	auto pSpriteFont = new SpriteFont();

	//**********
	// BLOCK 0 *
	//**********
	auto blockId = binReader->Read<char>();
	auto blockSize = binReader->Read<int>();
	pSpriteFont->m_FontSize = binReader->Read<short>();
	binReader->SetBufferPosition(23); //Set Buffer to end of Block
	pSpriteFont->m_FontName = binReader->ReadNullString();

	//**********
	// BLOCK 1 *
	//**********
	blockId = binReader->Read<char>();
	blockSize = binReader->Read<int>();
	binReader->MoveBufferPosition(4); //Move To Page Info
	pSpriteFont->m_TextureWidth = binReader->Read<unsigned short>();
	pSpriteFont->m_TextureHeight = binReader->Read<unsigned short>();
	const auto pageCount = binReader->Read<unsigned short>();

	if (pageCount>1)
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > SpriteFont (.fnt): Only one texture per font is allowed!");
	}

	binReader->MoveBufferPosition(5); //Move To Page Information

	//**********
	// BLOCK 2 *
	//**********
	blockId = binReader->Read<char>();
	blockSize = binReader->Read<int>();
	const auto pageName = binReader->ReadNullString();

	if (!assetFile.empty())
	{
		//GET PATH
		const auto slashPos = assetFile.rfind('/');
		std::wstring path;
		if (slashPos != std::wstring::npos)
		{
			path = assetFile.substr(0, slashPos + 1);
		}

		pSpriteFont->m_pTexture = ContentManager::Load<TextureData>(path + pageName);
	}

	//**********
	// BLOCK 3 *
	//**********
	blockId = binReader->Read<char>();
	blockSize = binReader->Read<int>();
	const auto charCount = blockSize / 20;

	for (int i = 0; i < charCount; ++i)
	{
		auto charId = static_cast<wchar_t>(binReader->Read<unsigned int>());

		if (!SpriteFont::IsCharValid(charId))
		{
			Logger::LogFormat(LogLevel::Warning, L"SpriteFontLoader::LoadContent > SpriteFont (.fnt): Skipped Character [ID: %i]. (Supported Range: %i - %i)\nFontFile: %s", charId, SpriteFont::MIN_CHAR_ID, SpriteFont::MAX_CHAR_ID, assetFile.c_str());

			binReader->MoveBufferPosition(16);
			continue;
		}

		auto& metric = pSpriteFont->GetMetric(charId);
		metric.IsValid = true;
		metric.Character = charId;
		const auto posX = binReader->Read<unsigned short>();
		const auto posY = binReader->Read<unsigned short>();
		metric.Width = binReader->Read<unsigned short>();
		metric.Height = binReader->Read<unsigned short>();
		metric.OffsetX = binReader->Read<short>();
		metric.OffsetY = binReader->Read<short>();
		metric.AdvanceX = binReader->Read<short>();
		metric.Page = binReader->Read<unsigned char>();

		const auto chnl = binReader->Read<unsigned char>();
		if (chnl == 1)metric.Channel = 2;
		else if (chnl == 2)metric.Channel = 1;
		else if (chnl == 8)metric.Channel = 3;
		else metric.Channel = 0;

		metric.TexCoord = DirectX::XMFLOAT2(posX / static_cast<float>(pSpriteFont->m_TextureWidth), posY / static_cast<float>(pSpriteFont->m_TextureHeight));
	}

	delete binReader;
	return pSpriteFont;
}

void SpriteFontLoader::Destroy(SpriteFont* objToDestroy)
{
	SafeDelete(objToDestroy);
}
