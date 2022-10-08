#pragma once
class IScene
{
protected:
	bool isEnd;
public:
	virtual ~IScene() { }
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual IScene* GetNextScene() = 0;
	inline bool GetisEnd() { return isEnd; }
};