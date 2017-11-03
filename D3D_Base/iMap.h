#pragma once
class iMap
{
public:
	iMap();
	~iMap();

	virtual bool GetHeight(IN float x, OUT float& y, IN float z ) = 0;
	virtual void Render() = 0;
};

