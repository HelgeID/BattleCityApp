//FINAL
#ifndef ANIMATION_H
#define ANIMATION_H

#include <SFML\Graphics.hpp>
/////////////////////////////////////////////////////////////////////
// class Animation
/////////////////////////////////////////////////////////////////////

class Animation
{
private:
	const sf::Texture* ptrTexture;
	std::vector<sf::IntRect> frames;

public:
	Animation();
	void AddFrame(sf::IntRect);
	void DelHeadFrame();
	void SetTexture(const sf::Texture&);
	const sf::Texture* GetTexture() const;
	std::size_t GetSize() const;
	const sf::IntRect& GetFrame(std::size_t) const;
};

inline Animation::Animation() : ptrTexture(NULL)
{
}

inline void Animation::AddFrame(sf::IntRect rect)
{
	frames.push_back(rect);
	return;
}

inline void Animation::DelHeadFrame()
{
	frames.erase(frames.begin());
	return;
}

inline void Animation::SetTexture(const sf::Texture& texture)
{
	ptrTexture = &texture;
	return;
}

inline const sf::Texture* Animation::GetTexture() const
{
	return ptrTexture;
}

inline std::size_t Animation::GetSize() const
{
	return frames.size();
}

inline const sf::IntRect& Animation::GetFrame(std::size_t num) const
{
	return frames[num];
}

/////////////////////////////////////////////////////////////////////
// class AnimatedObject
/////////////////////////////////////////////////////////////////////
class AnimatedObject : public sf::Drawable, public sf::Transformable
{
private:
	const Animation* animation;
	sf::Time frameTime, currentTime;
	std::size_t currentFrame;
	bool isPaused;
	bool isLooped;
	const sf::Texture* ptrTexture;
	sf::Vertex vertices[4];

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
	explicit AnimatedObject(
		sf::Time frameTime = sf::seconds(0.2f),
		bool paused = false,
		bool looped = true
	);

	void SetAnimation(const Animation&);
	void SetFrameTime(sf::Time);
	void PlayAnimation();
	void PlayAnimation(const Animation&);
	void PauseAnimation();
	void StopAnimation();
	void SetLooped(bool);
	void SetColor(const sf::Color&);
	const Animation* GetAnimation() const;
	sf::FloatRect GetLocalBounds() const;
	sf::FloatRect GetGlobalBounds() const;
	bool IsLooped() const;
	bool IsPlaying() const;
	sf::Time GetFrameTime() const;
	void SetFrame(std::size_t, bool);
	std::size_t GetCurrentFrame() const;
	void Update(sf::Time);
};

inline AnimatedObject::AnimatedObject(sf::Time frameTime, bool paused, bool looped) :
	animation(NULL), frameTime(frameTime), currentFrame(0), isPaused(paused), isLooped(looped), ptrTexture(NULL)
{
}

inline void AnimatedObject::SetAnimation(const Animation& animation)
{
	this->animation = &animation;
	ptrTexture = this->animation->GetTexture();
	currentFrame = 0;
	SetFrame(currentFrame, true);
	return;
}

inline void AnimatedObject::SetFrameTime(sf::Time time)
{
	frameTime = time;
	return;
}

inline void AnimatedObject::PlayAnimation()
{
	isPaused = false;
	return;
}

inline void AnimatedObject::PlayAnimation(const Animation& animation)
{
	if (GetAnimation() != &animation)
		SetAnimation(animation);
	PlayAnimation();
	return;
}

inline void AnimatedObject::PauseAnimation()
{
	isPaused = true;
	return;
}

inline void AnimatedObject::StopAnimation()
{
	isPaused = true;
	currentFrame = 0;
	SetFrame(currentFrame, true);
	return;
}

inline void AnimatedObject::SetLooped(bool looped)
{
	isLooped = looped;
	return;
}

inline void AnimatedObject::SetColor(const sf::Color& color)
{
	//Update the vertices' color
	vertices[0].color = color;
	vertices[1].color = color;
	vertices[2].color = color;
	vertices[3].color = color;
	return;
}

inline const Animation* AnimatedObject::GetAnimation() const
{
	return animation;
}

inline sf::FloatRect AnimatedObject::GetLocalBounds() const
{
	sf::IntRect rect = animation->GetFrame(currentFrame);
	float width = static_cast<float>(std::abs(rect.width));
	float height = static_cast<float>(std::abs(rect.height));
	return sf::FloatRect(0.f, 0.f, width, height);
}

inline sf::FloatRect AnimatedObject::GetGlobalBounds() const
{
	return getTransform().transformRect(GetLocalBounds());
}

inline bool AnimatedObject::IsLooped() const
{
	return isLooped;
}

inline bool AnimatedObject::IsPlaying() const
{
	return !isPaused;
}

inline sf::Time AnimatedObject::GetFrameTime() const
{
	return frameTime;
}

inline void AnimatedObject::SetFrame(std::size_t newFrame, bool resetTime)
{
	if (animation) {
		//calculate new vertex positions and texture coordiantes
		sf::IntRect rect = animation->GetFrame(newFrame);

		vertices[0].position = sf::Vector2f(0.f, 0.f);
		vertices[1].position = sf::Vector2f(0.f, static_cast<float>(rect.height));
		vertices[2].position = sf::Vector2f(static_cast<float>(rect.width), static_cast<float>(rect.height));
		vertices[3].position = sf::Vector2f(static_cast<float>(rect.width), 0.f);

		float left = static_cast<float>(rect.left) + 0.0001f;
		float right = left + static_cast<float>(rect.width);
		float top = static_cast<float>(rect.top);
		float bottom = top + static_cast<float>(rect.height);

		vertices[0].texCoords = sf::Vector2f(left, top);
		vertices[1].texCoords = sf::Vector2f(left, bottom);
		vertices[2].texCoords = sf::Vector2f(right, bottom);
		vertices[3].texCoords = sf::Vector2f(right, top);
	}

	if (resetTime)
		currentTime = sf::Time::Zero;

	return;
}

inline std::size_t AnimatedObject::GetCurrentFrame() const
{
	return currentFrame;
}

inline void AnimatedObject::Update(sf::Time deltaTime)
{
	//if not paused and we have a valid animation
	if (!isPaused && animation) {
		//add delta time
		currentTime += deltaTime;

		//if current time is bigger then the frame time advance one frame
		if (currentTime >= frameTime) {
			//reset time, but keep the remainder
			currentTime = sf::microseconds(currentTime.asMicroseconds() % frameTime.asMicroseconds());

			//get next Frame index
			if (currentFrame + 1 < animation->GetSize())
				currentFrame++;
			else {
				//animation has ended
				currentFrame = 0; //reset to start

				if (!isLooped)
					isPaused = true;

			}

			//set the current frame, not reseting the time
			SetFrame(currentFrame, false);
		}
	}
	return;
}

inline void AnimatedObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (animation && ptrTexture) {
		states.transform *= getTransform();
		states.texture = ptrTexture;
		target.draw(vertices, 4, sf::Quads, states);
	}
	return;
}

#endif
