#ifndef Util_Rectangle_h__
#define Util_Rectangle_h__

#include <algorithm>

//struct Rectangle
//{
//	Rectangle()
//		: X(0), Y(0), Width(0), Height(0) { }
//
//	Rectangle(int x, int y, int width = 0, int height = 0) 
//		: X(x), Y(y), Width(width), Height(height) { }
//
//	/*Rectangle(const Rectangle &rect)
//		: X(rect.X), Y(rect.Y), Width(rect.Width), Height(rect.Height) { }*/
//
//	int X;
//	int Y;
//	int Width;
//	int Height;
//
//	const int& GetLeft() const { return X; }
//	void SetLeft(int left) 
//	{
//		Width += X - left;
//		X = left;
//	}
//	int GetRight() const { return X + Width; }
//	void SetRight(int right) 
//	{
//		Width = right - X;
//	}
//	const int& GetTop() const { return Y; }
//	void SetTop(int top) 
//	{ 
//		Height += Y - top;
//		Y = top;
//	}
//	int GetBottom() const { return Y + Height; }
//	int SetBottom(int bottom)
//	{ 
//		Height = bottom - Y;
//	}
//
//	Rectangle& operator+=(const Rectangle &rhs)
//	{
//		SetLeft(std::min(GetLeft(), rhs.GetLeft()));
//		SetRight(std::max(GetRight(), rhs.GetRight()));
//		SetTop(std::min(GetTop(), rhs.GetTop()));
//		SetBottom(std::max(GetBottom(), rhs.GetBottom()));
//	}
//
//	static bool Intersects(const Rectangle &r1, const Rectangle &r2)
//	{
//		return !(r2.GetLeft() > r1.GetRight() || r2.GetRight() < r1.GetLeft() || r2.GetTop() > r1.GetBottom() || r2.GetBottom() < r1.GetTop());
//	}
//};
//
//inline bool operator==(const Rectangle &r1, const Rectangle &r2)
//{
//	return (r1.X == r2.X) && (r1.Y == r2.Y) && (r1.Width == r2.Width) && (r1.Height == r2.Height);
//}
//
//inline bool operator!=(const Rectangle &lhs, const Rectangle &rhs)
//{
//	return !(lhs == rhs);
//}
//
//inline bool operator<(const Rectangle &lhs, const Rectangle &rhs)
//{
//	return (lhs.Width < rhs.Width) && (lhs.Height < rhs.Height);
//}
//
//inline bool operator>(const Rectangle &lhs, const Rectangle &rhs)
//{
//	return rhs < lhs;
//}
//
//inline bool operator<=(const Rectangle &lhs, const Rectangle &rhs)
//{
//	return !(lhs > rhs);
//}
//
//inline bool operator>=(const Rectangle &lhs, const Rectangle &rhs)
//{
//	return !(lhs < rhs);
//}
//
//inline Rectangle operator+(Rectangle lhs, const Rectangle &rhs)
//{
//	lhs += rhs;
//	return lhs;
//}

#endif // Util_Rectangle_h__
