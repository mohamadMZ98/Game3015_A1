#pragma once
#include "Entity.hpp"
#include <string>

class Aircraft :
    public Entity
{
public:
	enum Type
	{
		Eagle,
		Raptor,
	};


public:
	Aircraft(Type type, Game* game);


private:
	virtual void		updateCurrent(const GameTimer& gt);
	virtual void		drawCurrent(ID3D12GraphicsCommandList* cmdList, FrameResource* mCurrFrameResource) const;
	virtual void		buildCurrent();


private:
	Type				mType;
	std::string			mSprite;
};
