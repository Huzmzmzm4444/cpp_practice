struct Vec2d
{
	double x{ 0. };
	double y{ 0. };
	Vec2d() = default;
	Vec2d(double x_, double y_) : x{ x_ }, y{ y_ } {}
	Vec2d& operator+= (const Vec2d& rhs);
	Vec2d operator-= (const Vec2d& lhs);
	Vec2d operator*= (const Vec2d& lhs);
	double length(const Vec2d& v);
};
