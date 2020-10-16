#include "Textures.h"
#include "GameEngine.h"
#include "Debug.h"

Texture::Texture()
{
	name = "";
	tex = nullptr;
	rect = SDL_Rect();
	anchor = Anchor::Center;
}

Texture::~Texture()
{
	Clear();
}

void Texture::Clear()
{
	if (tex != nullptr)
		SDL_DestroyTexture(tex);

	tex = nullptr;

	rect.x = 0;
	rect.y = 0;
	rect.h = 0;
	rect.w = 0;
}

bool Texture::Load(std::string fileLoc, std::string newName)
{
	if (tex != nullptr)
	{
		debug.Log("Texture", "Load", "A texture is already loaded in this object");
		return false;
	}

	SDL_Surface* surface;

	// Use base SDL_Image loading function.
	surface = IMG_Load(fileLoc.c_str());

	if (surface == nullptr)
	{
		std::string str = SDL_GetError();
		debug.Log("Texture", "Load", "Failed to load image " + str);
		return false;
	}

	// Setup the default source rect
	rect.x = 0;
	rect.y = 0;
	rect.w = surface->w;
	rect.h = surface->h;

	// Set the color key for transparency as RGB(255, 0 , 255)
	SDL_SetColorKey(surface, true, SDL_MapRGB(surface->format, 255, 0, 255));

	// Create the new texture
	tex = SDL_CreateTextureFromSurface(game.GetRenderer().renderer, surface);

	// Clear out the old surface.
	SDL_FreeSurface(surface);

	if (tex == nullptr)
	{
		std::string str = SDL_GetError();
		debug.Log("Texture", "Load", "Failed to create texture from image " + str);
		return false;
	}

	name = newName;

	debug.Log("Texture", "Load", "Success loading texture at location : " + fileLoc);

	return true;
}

bool Texture::Draw(SDL_Renderer* renderer, int x, int y)
{
	return Draw(renderer, 0, x, y);
}

bool Texture::Draw(SDL_Renderer* renderer, float rotation, int x, int y)
{
	if (renderer != nullptr && HasTexture())
	{
		SDL_Rect tempRect = rect;

		// Prepare the render zones ahead of time.
		tempRect.x = x;
		tempRect.y = y;

		// Determine where we are anchoring the texture to from the given X/Y coordinates.
		switch (anchor)
		{
		case Anchor::TopRight:
			tempRect.x -= tempRect.w;
			break;

		case Anchor::Top:
			tempRect.x -= tempRect.w / 2;
			break;

		case Anchor::TopLeft:
			// Do nothing.
			break;

		case Anchor::Right:
			tempRect.x -= tempRect.w;
			tempRect.y -= tempRect.h / 2;
			break;

		case Anchor::Center:
			tempRect.x -= tempRect.w / 2;
			tempRect.y -= tempRect.h / 2;
			break;

		case Anchor::Left:
			tempRect.y -= tempRect.h / 2;
			break;

		case Anchor::BottomRight:
			tempRect.x -= tempRect.w;
			tempRect.y -= tempRect.h;
			break;

		case Anchor::Bottom:
			tempRect.x -= tempRect.w / 2;
			tempRect.y -= tempRect.h;
			break;

		case Anchor::BottomLeft:
			tempRect.y -= tempRect.h;
			break;
		}

		// TODO: rework this, surely we dont need to calculate this each time we draw? we should calculate this WHEN we choose to change scale.
		if (scale != 1.0)
		{
			tempRect.w *= scale;
			tempRect.h *= scale;
		}

		// Render the texture to the given renderer.
		if (SDL_RenderCopyEx(renderer, tex, NULL, &tempRect, rotation, nullptr, SDL_FLIP_NONE) >= 0)
		{
			if (drawGivenCoordinates)
			{
				SDL_SetRenderDrawColor(game.GetRenderer().renderer, 255, 0, 0, 0);
				SDL_RenderDrawPoint(game.GetRenderer().renderer, x, y);
				SDL_SetRenderDrawColor(game.GetRenderer().renderer, game.GetRenderer().renderColor.r, game.GetRenderer().renderColor.g, game.GetRenderer().renderColor.b, game.GetRenderer().renderColor.a);
			}

			return true;
		}
	}

	debug.Log("Texture", "Draw", "Failed to draw texture: " + (std::string)SDL_GetError());
	return false;
}

bool Texture::HasTexture()
{
	if (tex != nullptr)
		return true;
	else
		return false;
}

bool Texture::SetTexture(SDL_Texture* texture, std::string newName)
{
	if (texture == nullptr)
		return false;

	if (newName.empty())
		return false;

	name = newName;
	tex = texture;

	SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);

	return true;
}

void Texture::ResetImageDimensions()
{
	SDL_QueryTexture(tex, nullptr, nullptr, &rect.w, &rect.h);
}

void Texture::SetWidthHeight(int w, int h)
{
	rect.w = w;
	rect.h = h;
}

void Texture::SetScale(float newScale)
{
	if (newScale > 0.0)
		scale = newScale;
}

void Textures::Cleanup()
{
	for (int i = 0; i < textureList.size(); i++)
	{
		textureList.at(i)->Clear();
		delete textureList.at(i);
	}

	textureList.clear();

	debug.Log("Textures", "Cleanup", "Destroyed all textures");
}

Texture* Textures::GetTexture(std::string name)
{
	for (auto& tex : textureList)
		if (tex->Name() == name)
			return tex;

	return nullptr;
}

Texture* Textures::CreateTexture(std::string fileLoc, std::string name)
{
	Texture* tex = nullptr;

	// First check if a texture under this name already exists.
	tex = GetTexture(name);

	if (tex != nullptr)
	{
		debug.Log("Textures", "CreateTexture", "A texture with the name of " + name + " already exists. Returning existing texture instead.");
		return tex;
	}

	// If it doesnt exist lets create it instead.
	tex = new Texture();

	if (!tex->Load(fileLoc, name))
	{
		delete tex;
		return nullptr;
	}

	textureList.push_back(tex);

	return textureList.back();
}

bool Textures::AddTexture(SDL_Texture* texture, std::string name)
{
	if (texture == nullptr)
	{
		debug.Log("Textures", "AddTexture", "given texture was nullptr, can not add a blank texture");
		return false;
	}

	Texture* tex = new Texture();

	tex->SetTexture(texture, name);

	textureList.push_back(tex);

	return true;
}

void Textures::DeleteTexture(std::string name)
{
	int i = 0;

	for (auto& tex : textureList)
		if (tex->Name() == name)
		{
			delete textureList[i];
			textureList.erase(textureList.begin() + i);
			break;
		}
		else
			i++;
}