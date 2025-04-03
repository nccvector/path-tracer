#ifndef TRACERMATH_H
#define TRACERMATH_H
#include <array>
#include <cstdint>
#include <vector>

#ifdef DOUBLEPRECISION
typedef double Scalar;
#else
typedef float Scalar;
#endif

template <typename T, uint8_t N>
struct Vector {
  std::array<T, N> data;
  Vector() { data.fill(0); }

  Vector(const std::array<T, N>& data) : data{data} {}

  Vector(T value) { data.fill(value); }

  Vector(const Vector<T, N-1>& t, T value) {
    std::copy(t.data.begin(), t.data.end(), data.begin());
    data[N-1] = value;
  }

  Vector(T x, T y)
    requires(N == 2)
      : data{x, y} {}

  Vector(T x, T y, T z)
    requires(N == 3)
      : data{x, y, z} {}

  Vector(T x, T y, T z, T w)
    requires(N == 4)
      : data{x, y, z, w} {}

  T& x() { return data[0]; }
  const T& x() const { return data[0]; }

  T& y() { return data[1]; }
  const T& y() const { return data[1]; }

  T& z()
    requires(N > 2)
  {
    return data[2];
  }
  const T& z() const
    requires(N > 2)
  {
    return data[2];
  }

  T& w()
    requires(N > 3)
  {
    return data[3];
  }
  const T& w() const
    requires(N > 3)
  {
    return data[3];
  }

  // ================================================================================
  // swizzle
  // ================================================================================
  // Generic swizzle function using parameter pack.
  template <std::size_t... Indices>
  auto swizzle() const {
    static_assert((... && (Indices < N)), "Swizzle index out of range");
    return Vector(std::array<T, sizeof...(Indices)>{data[Indices]...});
  }

#define DEFINE_SWIZZLE(name, X, ...) \
  auto name() const                  \
    requires(N > X)                  \
  {                                  \
    return swizzle<__VA_ARGS__>();   \
  }

  // 2d
  DEFINE_SWIZZLE(xx, 1, 0, 0)
  DEFINE_SWIZZLE(xy, 1, 0, 1)
  DEFINE_SWIZZLE(xz, 2, 0, 2)
  DEFINE_SWIZZLE(xw, 3, 0, 3)

  DEFINE_SWIZZLE(yx, 1, 1, 0)
  DEFINE_SWIZZLE(yy, 1, 1, 1)
  DEFINE_SWIZZLE(yz, 2, 1, 2)
  DEFINE_SWIZZLE(yw, 3, 1, 3)

  DEFINE_SWIZZLE(zx, 2, 2, 0)
  DEFINE_SWIZZLE(zy, 2, 2, 1)
  DEFINE_SWIZZLE(zz, 2, 2, 2)
  DEFINE_SWIZZLE(zw, 3, 2, 3)

  DEFINE_SWIZZLE(wx, 3, 3, 0)
  DEFINE_SWIZZLE(wy, 3, 3, 1)
  DEFINE_SWIZZLE(wz, 3, 3, 2)
  DEFINE_SWIZZLE(ww, 3, 3, 3)

  // 3d
  DEFINE_SWIZZLE(xxx, 1, 0, 0, 0)
  DEFINE_SWIZZLE(xxy, 1, 0, 0, 1)
  DEFINE_SWIZZLE(xxz, 2, 0, 0, 2)
  DEFINE_SWIZZLE(xxw, 3, 0, 0, 3)
  DEFINE_SWIZZLE(xyx, 1, 0, 1, 0)
  DEFINE_SWIZZLE(xyy, 1, 0, 1, 1)
  DEFINE_SWIZZLE(xyz, 2, 0, 1, 2)
  DEFINE_SWIZZLE(xyw, 3, 0, 1, 3)
  DEFINE_SWIZZLE(xzx, 2, 0, 2, 0)
  DEFINE_SWIZZLE(xzy, 2, 0, 2, 1)
  DEFINE_SWIZZLE(xzz, 2, 0, 2, 2)
  DEFINE_SWIZZLE(xzw, 3, 0, 2, 3)
  DEFINE_SWIZZLE(xwx, 3, 0, 3, 0)
  DEFINE_SWIZZLE(xwy, 3, 0, 3, 1)
  DEFINE_SWIZZLE(xwz, 3, 0, 3, 2)
  DEFINE_SWIZZLE(xww, 3, 0, 3, 3)

  DEFINE_SWIZZLE(yxx, 1, 1, 0, 0)
  DEFINE_SWIZZLE(yxy, 1, 1, 0, 1)
  DEFINE_SWIZZLE(yxz, 2, 1, 0, 2)
  DEFINE_SWIZZLE(yxw, 3, 1, 0, 3)
  DEFINE_SWIZZLE(yyx, 1, 1, 1, 0)
  DEFINE_SWIZZLE(yyy, 1, 1, 1, 1)
  DEFINE_SWIZZLE(yyz, 2, 1, 1, 2)
  DEFINE_SWIZZLE(yyw, 3, 1, 1, 3)
  DEFINE_SWIZZLE(yzx, 2, 1, 2, 0)
  DEFINE_SWIZZLE(yzy, 2, 1, 2, 1)
  DEFINE_SWIZZLE(yzz, 2, 1, 2, 2)
  DEFINE_SWIZZLE(yzw, 3, 1, 2, 3)
  DEFINE_SWIZZLE(ywx, 3, 1, 3, 0)
  DEFINE_SWIZZLE(ywy, 3, 1, 3, 1)
  DEFINE_SWIZZLE(ywz, 3, 1, 3, 2)
  DEFINE_SWIZZLE(yww, 3, 1, 3, 3)

  DEFINE_SWIZZLE(zxx, 2, 2, 0, 0)
  DEFINE_SWIZZLE(zxy, 2, 2, 0, 1)
  DEFINE_SWIZZLE(zxz, 2, 2, 0, 2)
  DEFINE_SWIZZLE(zxw, 3, 2, 0, 3)
  DEFINE_SWIZZLE(zyx, 2, 2, 1, 0)
  DEFINE_SWIZZLE(zyy, 2, 2, 1, 1)
  DEFINE_SWIZZLE(zyz, 2, 2, 1, 2)
  DEFINE_SWIZZLE(zyw, 3, 2, 1, 3)
  DEFINE_SWIZZLE(zzx, 2, 2, 2, 0)
  DEFINE_SWIZZLE(zzy, 2, 2, 2, 1)
  DEFINE_SWIZZLE(zzz, 2, 2, 2, 2)
  DEFINE_SWIZZLE(zzw, 3, 2, 2, 3)
  DEFINE_SWIZZLE(zwx, 3, 2, 3, 0)
  DEFINE_SWIZZLE(zwy, 3, 2, 3, 1)
  DEFINE_SWIZZLE(zwz, 3, 2, 3, 2)
  DEFINE_SWIZZLE(zww, 3, 2, 3, 3)

  DEFINE_SWIZZLE(wxx, 3, 3, 0, 0)
  DEFINE_SWIZZLE(wxy, 3, 3, 0, 1)
  DEFINE_SWIZZLE(wxz, 3, 3, 0, 2)
  DEFINE_SWIZZLE(wxw, 3, 3, 0, 3)
  DEFINE_SWIZZLE(wyx, 3, 3, 1, 0)
  DEFINE_SWIZZLE(wyy, 3, 3, 1, 1)
  DEFINE_SWIZZLE(wyz, 3, 3, 1, 2)
  DEFINE_SWIZZLE(wyw, 3, 3, 1, 3)
  DEFINE_SWIZZLE(wzx, 3, 3, 2, 0)
  DEFINE_SWIZZLE(wzy, 3, 3, 2, 1)
  DEFINE_SWIZZLE(wzz, 3, 3, 2, 2)
  DEFINE_SWIZZLE(wzw, 3, 3, 2, 3)
  DEFINE_SWIZZLE(wwx, 3, 3, 3, 0)
  DEFINE_SWIZZLE(wwy, 3, 3, 3, 1)
  DEFINE_SWIZZLE(wwz, 3, 3, 3, 2)
  DEFINE_SWIZZLE(www, 3, 3, 3, 3)

  // 4d
  DEFINE_SWIZZLE(xxxx, 1, 0, 0, 0, 0)
  DEFINE_SWIZZLE(xxxy, 1, 0, 0, 0, 1)
  DEFINE_SWIZZLE(xxxz, 2, 0, 0, 0, 2)
  DEFINE_SWIZZLE(xxxw, 3, 0, 0, 0, 3)
  DEFINE_SWIZZLE(xxyx, 1, 0, 0, 1, 0)
  DEFINE_SWIZZLE(xxyy, 1, 0, 0, 1, 1)
  DEFINE_SWIZZLE(xxyz, 2, 0, 0, 1, 2)
  DEFINE_SWIZZLE(xxyw, 3, 0, 0, 1, 3)
  DEFINE_SWIZZLE(xxzx, 2, 0, 0, 2, 0)
  DEFINE_SWIZZLE(xxzy, 2, 0, 0, 2, 1)
  DEFINE_SWIZZLE(xxzz, 2, 0, 0, 2, 2)
  DEFINE_SWIZZLE(xxzw, 3, 0, 0, 2, 3)
  DEFINE_SWIZZLE(xxwx, 3, 0, 0, 3, 0)
  DEFINE_SWIZZLE(xxwy, 3, 0, 0, 3, 1)
  DEFINE_SWIZZLE(xxwz, 3, 0, 0, 3, 2)
  DEFINE_SWIZZLE(xxww, 3, 0, 0, 3, 3)
  DEFINE_SWIZZLE(xyxx, 1, 0, 1, 0, 0)
  DEFINE_SWIZZLE(xyxy, 1, 0, 1, 0, 1)
  DEFINE_SWIZZLE(xyxz, 2, 0, 1, 0, 2)
  DEFINE_SWIZZLE(xyxw, 3, 0, 1, 0, 3)
  DEFINE_SWIZZLE(xyyx, 1, 0, 1, 1, 0)
  DEFINE_SWIZZLE(xyyy, 1, 0, 1, 1, 1)
  DEFINE_SWIZZLE(xyyz, 2, 0, 1, 1, 2)
  DEFINE_SWIZZLE(xyyw, 3, 0, 1, 1, 3)
  DEFINE_SWIZZLE(xyzx, 2, 0, 1, 2, 0)
  DEFINE_SWIZZLE(xyzy, 2, 0, 1, 2, 1)
  DEFINE_SWIZZLE(xyzz, 2, 0, 1, 2, 2)
  DEFINE_SWIZZLE(xyzw, 3, 0, 1, 2, 3)
  DEFINE_SWIZZLE(xywx, 3, 0, 1, 3, 0)
  DEFINE_SWIZZLE(xywy, 3, 0, 1, 3, 1)
  DEFINE_SWIZZLE(xywz, 3, 0, 1, 3, 2)
  DEFINE_SWIZZLE(xyww, 3, 0, 1, 3, 3)
  DEFINE_SWIZZLE(xzxx, 2, 0, 2, 0, 0)
  DEFINE_SWIZZLE(xzxy, 2, 0, 2, 0, 1)
  DEFINE_SWIZZLE(xzxz, 2, 0, 2, 0, 2)
  DEFINE_SWIZZLE(xzxw, 3, 0, 2, 0, 3)
  DEFINE_SWIZZLE(xzyx, 2, 0, 2, 1, 0)
  DEFINE_SWIZZLE(xzyy, 2, 0, 2, 1, 1)
  DEFINE_SWIZZLE(xzyz, 2, 0, 2, 1, 2)
  DEFINE_SWIZZLE(xzyw, 3, 0, 2, 1, 3)
  DEFINE_SWIZZLE(xzzx, 2, 0, 2, 2, 0)
  DEFINE_SWIZZLE(xzzy, 2, 0, 2, 2, 1)
  DEFINE_SWIZZLE(xzzz, 2, 0, 2, 2, 2)
  DEFINE_SWIZZLE(xzzw, 3, 0, 2, 2, 3)
  DEFINE_SWIZZLE(xzwx, 3, 0, 2, 3, 0)
  DEFINE_SWIZZLE(xzwy, 3, 0, 2, 3, 1)
  DEFINE_SWIZZLE(xzwz, 3, 0, 2, 3, 2)
  DEFINE_SWIZZLE(xzww, 3, 0, 2, 3, 3)
  DEFINE_SWIZZLE(xwxx, 3, 0, 3, 0, 0)
  DEFINE_SWIZZLE(xwxy, 3, 0, 3, 0, 1)
  DEFINE_SWIZZLE(xwxz, 3, 0, 3, 0, 2)
  DEFINE_SWIZZLE(xwxw, 3, 0, 3, 0, 3)
  DEFINE_SWIZZLE(xwyx, 3, 0, 3, 1, 0)
  DEFINE_SWIZZLE(xwyy, 3, 0, 3, 1, 1)
  DEFINE_SWIZZLE(xwyz, 3, 0, 3, 1, 2)
  DEFINE_SWIZZLE(xwyw, 3, 0, 3, 1, 3)
  DEFINE_SWIZZLE(xwzx, 3, 0, 3, 2, 0)
  DEFINE_SWIZZLE(xwzy, 3, 0, 3, 2, 1)
  DEFINE_SWIZZLE(xwzz, 3, 0, 3, 2, 2)
  DEFINE_SWIZZLE(xwzw, 3, 0, 3, 2, 3)
  DEFINE_SWIZZLE(xwwx, 3, 0, 3, 3, 0)
  DEFINE_SWIZZLE(xwwy, 3, 0, 3, 3, 1)
  DEFINE_SWIZZLE(xwwz, 3, 0, 3, 3, 2)
  DEFINE_SWIZZLE(xwww, 3, 0, 3, 3, 3)

  DEFINE_SWIZZLE(yxxx, 1, 1, 0, 0, 0)
  DEFINE_SWIZZLE(yxxy, 1, 1, 0, 0, 1)
  DEFINE_SWIZZLE(yxxz, 2, 1, 0, 0, 2)
  DEFINE_SWIZZLE(yxxw, 3, 1, 0, 0, 3)
  DEFINE_SWIZZLE(yxyx, 1, 1, 0, 1, 0)
  DEFINE_SWIZZLE(yxyy, 1, 1, 0, 1, 1)
  DEFINE_SWIZZLE(yxyz, 2, 1, 0, 1, 2)
  DEFINE_SWIZZLE(yxyw, 3, 1, 0, 1, 3)
  DEFINE_SWIZZLE(yxzx, 2, 1, 0, 2, 0)
  DEFINE_SWIZZLE(yxzy, 2, 1, 0, 2, 1)
  DEFINE_SWIZZLE(yxzz, 2, 1, 0, 2, 2)
  DEFINE_SWIZZLE(yxzw, 3, 1, 0, 2, 3)
  DEFINE_SWIZZLE(yxwx, 3, 1, 0, 3, 0)
  DEFINE_SWIZZLE(yxwy, 3, 1, 0, 3, 1)
  DEFINE_SWIZZLE(yxwz, 3, 1, 0, 3, 2)
  DEFINE_SWIZZLE(yxww, 3, 1, 0, 3, 3)
  DEFINE_SWIZZLE(yyxx, 1, 1, 1, 0, 0)
  DEFINE_SWIZZLE(yyxy, 1, 1, 1, 0, 1)
  DEFINE_SWIZZLE(yyxz, 2, 1, 1, 0, 2)
  DEFINE_SWIZZLE(yyxw, 3, 1, 1, 0, 3)
  DEFINE_SWIZZLE(yyyx, 1, 1, 1, 1, 0)
  DEFINE_SWIZZLE(yyyy, 1, 1, 1, 1, 1)
  DEFINE_SWIZZLE(yyyz, 2, 1, 1, 1, 2)
  DEFINE_SWIZZLE(yyyw, 3, 1, 1, 1, 3)
  DEFINE_SWIZZLE(yyzx, 2, 1, 1, 2, 0)
  DEFINE_SWIZZLE(yyzy, 2, 1, 1, 2, 1)
  DEFINE_SWIZZLE(yyzz, 2, 1, 1, 2, 2)
  DEFINE_SWIZZLE(yyzw, 3, 1, 1, 2, 3)
  DEFINE_SWIZZLE(yywx, 3, 1, 1, 3, 0)
  DEFINE_SWIZZLE(yywy, 3, 1, 1, 3, 1)
  DEFINE_SWIZZLE(yywz, 3, 1, 1, 3, 2)
  DEFINE_SWIZZLE(yyww, 3, 1, 1, 3, 3)
  DEFINE_SWIZZLE(yzxx, 2, 1, 2, 0, 0)
  DEFINE_SWIZZLE(yzxy, 2, 1, 2, 0, 1)
  DEFINE_SWIZZLE(yzxz, 2, 1, 2, 0, 2)
  DEFINE_SWIZZLE(yzxw, 3, 1, 2, 0, 3)
  DEFINE_SWIZZLE(yzyx, 2, 1, 2, 1, 0)
  DEFINE_SWIZZLE(yzyy, 2, 1, 2, 1, 1)
  DEFINE_SWIZZLE(yzyz, 2, 1, 2, 1, 2)
  DEFINE_SWIZZLE(yzyw, 3, 1, 2, 1, 3)
  DEFINE_SWIZZLE(yzzx, 2, 1, 2, 2, 0)
  DEFINE_SWIZZLE(yzzy, 2, 1, 2, 2, 1)
  DEFINE_SWIZZLE(yzzz, 2, 1, 2, 2, 2)
  DEFINE_SWIZZLE(yzzw, 3, 1, 2, 2, 3)
  DEFINE_SWIZZLE(yzwx, 3, 1, 2, 3, 0)
  DEFINE_SWIZZLE(yzwy, 3, 1, 2, 3, 1)
  DEFINE_SWIZZLE(yzwz, 3, 1, 2, 3, 2)
  DEFINE_SWIZZLE(yzww, 3, 1, 2, 3, 3)
  DEFINE_SWIZZLE(ywxx, 3, 1, 3, 0, 0)
  DEFINE_SWIZZLE(ywxy, 3, 1, 3, 0, 1)
  DEFINE_SWIZZLE(ywxz, 3, 1, 3, 0, 2)
  DEFINE_SWIZZLE(ywxw, 3, 1, 3, 0, 3)
  DEFINE_SWIZZLE(ywyx, 3, 1, 3, 1, 0)
  DEFINE_SWIZZLE(ywyy, 3, 1, 3, 1, 1)
  DEFINE_SWIZZLE(ywyz, 3, 1, 3, 1, 2)
  DEFINE_SWIZZLE(ywyw, 3, 1, 3, 1, 3)
  DEFINE_SWIZZLE(ywzx, 3, 1, 3, 2, 0)
  DEFINE_SWIZZLE(ywzy, 3, 1, 3, 2, 1)
  DEFINE_SWIZZLE(ywzz, 3, 1, 3, 2, 2)
  DEFINE_SWIZZLE(ywzw, 3, 1, 3, 2, 3)
  DEFINE_SWIZZLE(ywwx, 3, 1, 3, 3, 0)
  DEFINE_SWIZZLE(ywwy, 3, 1, 3, 3, 1)
  DEFINE_SWIZZLE(ywwz, 3, 1, 3, 3, 2)
  DEFINE_SWIZZLE(ywww, 3, 1, 3, 3, 3)

  DEFINE_SWIZZLE(zxxx, 2, 2, 0, 0, 0)
  DEFINE_SWIZZLE(zxxy, 2, 2, 0, 0, 1)
  DEFINE_SWIZZLE(zxxz, 2, 2, 0, 0, 2)
  DEFINE_SWIZZLE(zxxw, 3, 2, 0, 0, 3)
  DEFINE_SWIZZLE(zxyx, 2, 2, 0, 1, 0)
  DEFINE_SWIZZLE(zxyy, 2, 2, 0, 1, 1)
  DEFINE_SWIZZLE(zxyz, 2, 2, 0, 1, 2)
  DEFINE_SWIZZLE(zxyw, 3, 2, 0, 1, 3)
  DEFINE_SWIZZLE(zxzx, 2, 2, 0, 2, 0)
  DEFINE_SWIZZLE(zxzy, 2, 2, 0, 2, 1)
  DEFINE_SWIZZLE(zxzz, 2, 2, 0, 2, 2)
  DEFINE_SWIZZLE(zxzw, 3, 2, 0, 2, 3)
  DEFINE_SWIZZLE(zxwx, 3, 2, 0, 3, 0)
  DEFINE_SWIZZLE(zxwy, 3, 2, 0, 3, 1)
  DEFINE_SWIZZLE(zxwz, 3, 2, 0, 3, 2)
  DEFINE_SWIZZLE(zxww, 3, 2, 0, 3, 3)
  DEFINE_SWIZZLE(zyxx, 2, 2, 1, 0, 0)
  DEFINE_SWIZZLE(zyxy, 2, 2, 1, 0, 1)
  DEFINE_SWIZZLE(zyxz, 2, 2, 1, 0, 2)
  DEFINE_SWIZZLE(zyxw, 3, 2, 1, 0, 3)
  DEFINE_SWIZZLE(zyyx, 2, 2, 1, 1, 0)
  DEFINE_SWIZZLE(zyyy, 2, 2, 1, 1, 1)
  DEFINE_SWIZZLE(zyyz, 2, 2, 1, 1, 2)
  DEFINE_SWIZZLE(zyyw, 3, 2, 1, 1, 3)
  DEFINE_SWIZZLE(zyzx, 2, 2, 1, 2, 0)
  DEFINE_SWIZZLE(zyzy, 2, 2, 1, 2, 1)
  DEFINE_SWIZZLE(zyzz, 2, 2, 1, 2, 2)
  DEFINE_SWIZZLE(zyzw, 3, 2, 1, 2, 3)
  DEFINE_SWIZZLE(zywx, 3, 2, 1, 3, 0)
  DEFINE_SWIZZLE(zywy, 3, 2, 1, 3, 1)
  DEFINE_SWIZZLE(zywz, 3, 2, 1, 3, 2)
  DEFINE_SWIZZLE(zyww, 3, 2, 1, 3, 3)
  DEFINE_SWIZZLE(zzxx, 2, 2, 2, 0, 0)
  DEFINE_SWIZZLE(zzxy, 2, 2, 2, 0, 1)
  DEFINE_SWIZZLE(zzxz, 2, 2, 2, 0, 2)
  DEFINE_SWIZZLE(zzxw, 3, 2, 2, 0, 3)
  DEFINE_SWIZZLE(zzyx, 2, 2, 2, 1, 0)
  DEFINE_SWIZZLE(zzyy, 2, 2, 2, 1, 1)
  DEFINE_SWIZZLE(zzyz, 2, 2, 2, 1, 2)
  DEFINE_SWIZZLE(zzyw, 3, 2, 2, 1, 3)
  DEFINE_SWIZZLE(zzzx, 2, 2, 2, 2, 0)
  DEFINE_SWIZZLE(zzzy, 2, 2, 2, 2, 1)
  DEFINE_SWIZZLE(zzzz, 2, 2, 2, 2, 2)
  DEFINE_SWIZZLE(zzzw, 3, 2, 2, 2, 3)
  DEFINE_SWIZZLE(zzwx, 3, 2, 2, 3, 0)
  DEFINE_SWIZZLE(zzwy, 3, 2, 2, 3, 1)
  DEFINE_SWIZZLE(zzwz, 3, 2, 2, 3, 2)
  DEFINE_SWIZZLE(zzww, 3, 2, 2, 3, 3)
  DEFINE_SWIZZLE(zwxx, 3, 2, 3, 0, 0)
  DEFINE_SWIZZLE(zwxy, 3, 2, 3, 0, 1)
  DEFINE_SWIZZLE(zwxz, 3, 2, 3, 0, 2)
  DEFINE_SWIZZLE(zwxw, 3, 2, 3, 0, 3)
  DEFINE_SWIZZLE(zwyx, 3, 2, 3, 1, 0)
  DEFINE_SWIZZLE(zwyy, 3, 2, 3, 1, 1)
  DEFINE_SWIZZLE(zwyz, 3, 2, 3, 1, 2)
  DEFINE_SWIZZLE(zwyw, 3, 2, 3, 1, 3)
  DEFINE_SWIZZLE(zwzx, 3, 2, 3, 2, 0)
  DEFINE_SWIZZLE(zwzy, 3, 2, 3, 2, 1)
  DEFINE_SWIZZLE(zwzz, 3, 2, 3, 2, 2)
  DEFINE_SWIZZLE(zwzw, 3, 2, 3, 2, 3)
  DEFINE_SWIZZLE(zwwx, 3, 2, 3, 3, 0)
  DEFINE_SWIZZLE(zwwy, 3, 2, 3, 3, 1)
  DEFINE_SWIZZLE(zwwz, 3, 2, 3, 3, 2)
  DEFINE_SWIZZLE(zwww, 3, 2, 3, 3, 3)

  DEFINE_SWIZZLE(wxxx, 3, 3, 0, 0, 0)
  DEFINE_SWIZZLE(wxxy, 3, 3, 0, 0, 1)
  DEFINE_SWIZZLE(wxxz, 3, 3, 0, 0, 2)
  DEFINE_SWIZZLE(wxxw, 3, 3, 0, 0, 3)
  DEFINE_SWIZZLE(wxyx, 3, 3, 0, 1, 0)
  DEFINE_SWIZZLE(wxyy, 3, 3, 0, 1, 1)
  DEFINE_SWIZZLE(wxyz, 3, 3, 0, 1, 2)
  DEFINE_SWIZZLE(wxyw, 3, 3, 0, 1, 3)
  DEFINE_SWIZZLE(wxzx, 3, 3, 0, 2, 0)
  DEFINE_SWIZZLE(wxzy, 3, 3, 0, 2, 1)
  DEFINE_SWIZZLE(wxzz, 3, 3, 0, 2, 2)
  DEFINE_SWIZZLE(wxzw, 3, 3, 0, 2, 3)
  DEFINE_SWIZZLE(wxwx, 3, 3, 0, 3, 0)
  DEFINE_SWIZZLE(wxwy, 3, 3, 0, 3, 1)
  DEFINE_SWIZZLE(wxwz, 3, 3, 0, 3, 2)
  DEFINE_SWIZZLE(wxww, 3, 3, 0, 3, 3)
  DEFINE_SWIZZLE(wyxx, 3, 3, 1, 0, 0)
  DEFINE_SWIZZLE(wyxy, 3, 3, 1, 0, 1)
  DEFINE_SWIZZLE(wyxz, 3, 3, 1, 0, 2)
  DEFINE_SWIZZLE(wyxw, 3, 3, 1, 0, 3)
  DEFINE_SWIZZLE(wyyx, 3, 3, 1, 1, 0)
  DEFINE_SWIZZLE(wyyy, 3, 3, 1, 1, 1)
  DEFINE_SWIZZLE(wyyz, 3, 3, 1, 1, 2)
  DEFINE_SWIZZLE(wyyw, 3, 3, 1, 1, 3)
  DEFINE_SWIZZLE(wyzx, 3, 3, 1, 2, 0)
  DEFINE_SWIZZLE(wyzy, 3, 3, 1, 2, 1)
  DEFINE_SWIZZLE(wyzz, 3, 3, 1, 2, 2)
  DEFINE_SWIZZLE(wyzw, 3, 3, 1, 2, 3)
  DEFINE_SWIZZLE(wywx, 3, 3, 1, 3, 0)
  DEFINE_SWIZZLE(wywy, 3, 3, 1, 3, 1)
  DEFINE_SWIZZLE(wywz, 3, 3, 1, 3, 2)
  DEFINE_SWIZZLE(wyww, 3, 3, 1, 3, 3)
  DEFINE_SWIZZLE(wzxx, 3, 3, 2, 0, 0)
  DEFINE_SWIZZLE(wzxy, 3, 3, 2, 0, 1)
  DEFINE_SWIZZLE(wzxz, 3, 3, 2, 0, 2)
  DEFINE_SWIZZLE(wzxw, 3, 3, 2, 0, 3)
  DEFINE_SWIZZLE(wzyx, 3, 3, 2, 1, 0)
  DEFINE_SWIZZLE(wzyy, 3, 3, 2, 1, 1)
  DEFINE_SWIZZLE(wzyz, 3, 3, 2, 1, 2)
  DEFINE_SWIZZLE(wzyw, 3, 3, 2, 1, 3)
  DEFINE_SWIZZLE(wzzx, 3, 3, 2, 2, 0)
  DEFINE_SWIZZLE(wzzy, 3, 3, 2, 2, 1)
  DEFINE_SWIZZLE(wzzz, 3, 3, 2, 2, 2)
  DEFINE_SWIZZLE(wzzw, 3, 3, 2, 2, 3)
  DEFINE_SWIZZLE(wzwx, 3, 3, 2, 3, 0)
  DEFINE_SWIZZLE(wzwy, 3, 3, 2, 3, 1)
  DEFINE_SWIZZLE(wzwz, 3, 3, 2, 3, 2)
  DEFINE_SWIZZLE(wzww, 3, 3, 2, 3, 3)
  DEFINE_SWIZZLE(wwxx, 3, 3, 3, 0, 0)
  DEFINE_SWIZZLE(wwxy, 3, 3, 3, 0, 1)
  DEFINE_SWIZZLE(wwxz, 3, 3, 3, 0, 2)
  DEFINE_SWIZZLE(wwxw, 3, 3, 3, 0, 3)
  DEFINE_SWIZZLE(wwyx, 3, 3, 3, 1, 0)
  DEFINE_SWIZZLE(wwyy, 3, 3, 3, 1, 1)
  DEFINE_SWIZZLE(wwyz, 3, 3, 3, 1, 2)
  DEFINE_SWIZZLE(wwyw, 3, 3, 3, 1, 3)
  DEFINE_SWIZZLE(wwzx, 3, 3, 3, 2, 0)
  DEFINE_SWIZZLE(wwzy, 3, 3, 3, 2, 1)
  DEFINE_SWIZZLE(wwzz, 3, 3, 3, 2, 2)
  DEFINE_SWIZZLE(wwzw, 3, 3, 3, 2, 3)
  DEFINE_SWIZZLE(wwwx, 3, 3, 3, 3, 0)
  DEFINE_SWIZZLE(wwwy, 3, 3, 3, 3, 1)
  DEFINE_SWIZZLE(wwwz, 3, 3, 3, 3, 2)
  DEFINE_SWIZZLE(wwww, 3, 3, 3, 3, 3)
};

typedef Vector<int, 2> vec2i;
typedef Vector<int, 3> vec3i;
typedef Vector<int, 4> vec4i;

typedef Vector<float, 2> vec2f;
typedef Vector<float, 3> vec3f;
typedef Vector<float, 4> vec4f;

typedef Vector<double, 2> vec2d;
typedef Vector<double, 3> vec3d;
typedef Vector<double, 4> vec4d;

typedef Vector<Scalar, 2> vec2;
typedef Vector<Scalar, 3> vec3;
typedef Vector<Scalar, 4> vec4;

template <uint8_t R, uint8_t C>
struct Matrix {
  std::vector<Scalar> data;
  uint8_t numRows = R;
  uint8_t numCols = C;

  Matrix() {
    data = std::vector<Scalar>(R * C, 0);
    for (int i = 0; i < R * C; i++) {
      for (int j = 0; j < R * C; j++) {
      }
    }
  }
};

#endif  // TRACERMATH_H
