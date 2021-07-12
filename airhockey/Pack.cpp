
# include <Siv3D.hpp>

# include "Pack.h"

Pack::Pack() {
	Body = Circle(400, 400, 10);
	PackVelocity = {20, 20};
}

Circle Pack::GetBody() {
	return Body;
}

Vec2 Pack::GetVelocity() {
	return PackVelocity;
}

void Pack::SetBody() {

}

void Pack::SetVelocityX(double d) {
	PackVelocity.x = d;
}

void Pack::SetVelocityY(double d) {
	PackVelocity.y = d;
}