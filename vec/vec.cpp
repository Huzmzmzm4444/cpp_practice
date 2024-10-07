#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>
#include <stdexcept>
#include <string>

struct Vec2d {
    double x{ 0. };
    double y{ 0. };

    Vec2d() = default;
    Vec2d(double x_, double y_) : x{ x_ }, y{ y_ } {}

    // Операторы перегрузки
    Vec2d& operator+= (const Vec2d& rhs);
    Vec2d& operator-= (const Vec2d& rhs);
    Vec2d& operator*= (double scalar);
    friend Vec2d operator* (const Vec2d& v, double scalar);
    friend Vec2d operator* (double scalar, const Vec2d& v);
    friend Vec2d operator+ (const Vec2d& lhs, const Vec2d& rhs);
    friend Vec2d operator- (const Vec2d& lhs, const Vec2d& rhs);
    double length() const;
};

Vec2d& Vec2d::operator+= (const Vec2d& rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
}

Vec2d& Vec2d::operator-= (const Vec2d& rhs) {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
}

Vec2d& Vec2d::operator*= (double scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}

double Vec2d::length() const {
    return std::sqrt(x * x + y * y);
}

Vec2d operator+ (const Vec2d& lhs, const Vec2d& rhs) {
    return Vec2d(lhs) += rhs;
}

Vec2d operator- (const Vec2d& lhs, const Vec2d& rhs) {
    return Vec2d(lhs) -= rhs;
}

Vec2d operator* (const Vec2d& v, double scalar) {
    return Vec2d(v) *= scalar;
}

Vec2d operator* (double scalar, const Vec2d& v) {
    return Vec2d(v) *= scalar;
}

bool operator== (const Vec2d& p, const Vec2d& q) {
    return p.x == q.x && p.y == q.y;
}

bool operator!= (const Vec2d& p, const Vec2d& q) {
    return !(p == q);
}

struct TestV {
    std::string expr;
    Vec2d result;
    Vec2d answer;
};

struct TestD {
    std::string expr;
    double result;
    double answer;
};

#define EXPR(x) #x, x

int main() {
    Vec2d t{ 1, 1 };
    if (t.x != 1 || t.y != 1) // Исправлено условие
        throw std::runtime_error{ "Failed to construct a valid Vec2d object" };

    Vec2d r{ 1, 0 }, q{ 0, 1 };
    Vec2d s{ r }, p{ q };

    if (r != r || r == q || r == t)
        throw std::runtime_error{ "Failed to compare Vec2d objects" };

    std::vector<TestV> v_tests{
        {EXPR(r), {1, 0}},      {EXPR(q), {0, 1}},     {EXPR(r + r), {2, 0}},
        {EXPR(q + q), {0, 2}},  {EXPR(r + q), t},      {EXPR(r += r), {2, 0}},
        {EXPR(q += q), {0, 2}}, {EXPR(r), {2, 0}},     {EXPR(q), {0, 2}},
        {EXPR(r - s), {0, 0}},  {EXPR(q - p), {0, 0}}, {EXPR(r -= s), {0, 0}},
        {EXPR(q -= p), {0, 0}}, {EXPR(r), {0, 0}},     {EXPR(q), {0, 0}},
        {EXPR(r * 5), {0, 0}},  {EXPR(q * 5), {0, 0}}, {EXPR(5 * r), {0, 0}},
        {EXPR(5 * q), {0, 0}},   {EXPR(r = s), s},      {EXPR(q = p), p},
        {EXPR(r *= 5), {0, 0}},  {EXPR(q *= 5), {0, 0}}, {EXPR(r), {0, 0}},
        {EXPR(q), {0, 0}} };

    int total = 0, passed = 0;

    for (const auto& [expr, res, ans] : v_tests) {
        bool ok = (res == ans);
        std::cerr << std::setw(10) << std::left << expr << " = {" << res.x
            << ", " << res.y << "}"
            << " [" << (ok ? "OK" : "FAILED") << "]" << std::endl;
        ++total;
        if (ok)
            ++passed;
    }

    Vec2d s2{ 1, 1 }; // Пример использования для длины
    std::vector<TestD> d_tests{
        {EXPR(s.length()), sqrt(2.)},
        { "length(s2)", s.length(), sqrt(2.) }
    };

    constexpr double tol = std::numeric_limits<double>::epsilon();

    for (const auto& [expr, res, ans] : d_tests) {
        bool ok = (std::abs(res - ans) < tol);
        std::cerr << std::setw(25) << std::left << expr << " = " << std::setw(8)
            << res << " [" << (ok ? "OK" : "FAILED") << "]" << std::endl;
        ++total;
        if (ok)
            ++passed;
    }

    std::cerr << "-----------\n"
        << passed << " passed of " << total << " total" << std::endl;

    return 0;
}
