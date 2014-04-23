#ifndef GUI_Frame_h__
#define GUI_Frame_h__

#include <memory>

#include "Util/Rectangle.h"

namespace GUI
{
	
class Frame : public Rectangle
{
public:
	enum class Anchor
	{
		Left,
		Right,
		Top,
		Bottom
	};

	Frame() { }
	
	Frame(const Frame &parent)
		: Rectangle(static_cast<Rectangle>(parent))
		, Parent(std::shared_ptr<Frame>(&parent)) { }

	std::shared_ptr<Frame> Parent;
};

}

#endif // GUI_Frame_h__
