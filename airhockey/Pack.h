#pragma once
# include <Siv3D.hpp> // OpenSiv3D v0.4.3

class Pack
{
public:

	Pack();
	Circle GetBody();
	Vec2 GetVelocity();

	void SetBody();
	void SetVelocityX(double d);
	void SetVelocityY(double d);

private:
	Circle Body;
	Vec2 PackVelocity;
};

