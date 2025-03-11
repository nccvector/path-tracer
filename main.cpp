#include <array>
#include <cmath>
#include <fmt/core.h>
#include <iostream>
#include <optional>
#include <vector>

#define PI 3.14159;

namespace math {
  template<std::size_t R, std::size_t C>
  struct Matrix {
    size_t                   rows, cols;
    std::array<float, R * C> data;

    float       &operator()(const std::size_t row, const std::size_t col) { return data[row * C + col]; }
    const float &operator()(const std::size_t row, const std::size_t col) const { return data[row * C + col]; }
  };

  template<std::size_t R, std::size_t C>
  Matrix<R, C> Identity() {
    Matrix<R, C> eye = {R, C, {}};
    for (std::size_t i = 0; i < R && i < C; i++) {
      eye.data[i * C + i] = 1.0f; // Set diagonal to 1
    }
    return eye;
  }

  template<std::size_t R1, std::size_t C1, std::size_t R2, std::size_t C2>
  Matrix<R1, C2> operator*(const Matrix<R1, C1> &A, const Matrix<R2, C2> &B) {
    static_assert(C1 == R2, "Matrix multiplication dimensions must match: C1 == R2");

    Matrix<R1, C2> result = {}; // Zero initialize

    for (std::size_t i = 0; i < R1; ++i) {
      for (std::size_t j = 0; j < C2; ++j) {
        for (std::size_t k = 0; k < C1; ++k) {
          result(i, j) += A(i, k) * B(k, j);
        }
      }
    }

    return result;
  }
} // namespace math

// Specialization of fmt::formatter for Matrix<R, C>
template<std::size_t R, std::size_t C>
struct fmt::formatter<math::Matrix<R, C>> {
  constexpr auto parse(fmt::format_parse_context &ctx) { return ctx.begin(); }

  template<typename FormatContext>
  auto format(const math::Matrix<R, C> &mat, FormatContext &ctx) const {
    auto out = ctx.out();
    out      = fmt::format_to(out, "Matrix<{}, {}> [\n", R, C);

    for (std::size_t i = 0; i < R; ++i) {
      out = fmt::format_to(out, "\t[ ");
      for (std::size_t j = 0; j < C; ++j) {
        out = fmt::format_to(out, "{:7.3f}, ", mat(i, j)); // Align values nicely
      }
      out = fmt::format_to(out, "\t],\n");
    }
    return fmt::format_to(out, "]");
  }
};

template<typename T>
T Degrees(T radian) {
  return radian * 180.0f / PI;
}
template<typename T>
T Radians(T degrees) {
  return degrees / 180.f * PI;
}


math::Matrix<3, 3>
ComputeCameraMatrix(const float verticalFOVDegrees, const float imageWidth, const float imageHeight) {
  // Convert vertical FOV from degrees to radians
  const float halfVerticalFOVRadians = Radians(verticalFOVDegrees) / 2.0f;

  // Compute image plane distance and horizontal FOV
  const float halfImageWidth  = imageWidth / 2.0f;
  const float halfImageHeight = imageHeight / 2.0f;
  const float baseDistance =
    halfImageHeight / std::tan(halfVerticalFOVRadians); // distance of image plane from camera origin
  const float halfHorizontalFOVRadians = std::atan2(halfImageWidth, baseDistance);

  // Compute camera matrix parameters
  const float fy = (imageHeight - 1.0f) / (2.0f * std::tan(halfVerticalFOVRadians));
  const float fx = (imageWidth - 1.0f) / (2.0f * std::tan(halfHorizontalFOVRadians));

  // Compute the image center
  const float cx = (imageWidth - 1.0f) / 2.0f;
  const float cy = (imageHeight - 1.0f) / 2.0f;

  // Intrinsic camera matrix
  math::Matrix<3, 3> cameraMatrix = math::Identity<3, 3>();
  cameraMatrix(0, 0)              = fx;
  cameraMatrix(1, 1)              = fy;
  cameraMatrix(0, 2)              = cx;
  cameraMatrix(1, 2)              = cy;

  return cameraMatrix;
}


struct vec3 {
  float x, y, z;
};

struct uchar4 {
  unsigned char r, g, b, a;
};

constexpr int WIDTH  = 800;
constexpr int HEIGHT = 600;

struct Sphere {
  float radius;
  vec3  position;
};

struct Camera {
  float              verticalFov;
  int                imageWidth;
  int                imageHeight;
  math::Matrix<3, 3> cameraMatrix;
  math::Matrix<4, 4> transform;

  Camera(float verticalFov, int imageWidth, int imageHeight) :
      verticalFov(verticalFov), imageWidth(imageWidth), imageHeight(imageHeight) {
    cameraMatrix = ComputeCameraMatrix(verticalFov, imageWidth, imageHeight);
    transform    = math::Identity<4, 4>();
  }
};

template<>
struct fmt::formatter<Camera> {
  // Parse format specifier (not used here)
  constexpr auto parse(fmt::format_parse_context &ctx) { return ctx.begin(); }

  // Format the struct as a string
  template<typename FormatContext>
  auto format(const Camera &camera, FormatContext &ctx) const {
    fmt::format_to(ctx.out(), "Camera: {{\n");
    fmt::format_to(ctx.out(), "\tverticalFov = {}\n", camera.verticalFov);
    fmt::format_to(ctx.out(), "\timageWidth = {}\n", camera.imageWidth);
    fmt::format_to(ctx.out(), "\timageHeight = {}\n", camera.imageHeight);
    fmt::format_to(ctx.out(), "\tcameraMatrix = {}\n", camera.cameraMatrix);
    fmt::format_to(ctx.out(), "\ttransform = {}\n", camera.transform);
    fmt::format_to(ctx.out(), "}}");
    return ctx.out();
  }
};

std::optional<float> Intersect(const Sphere &sphere, const vec3 &ray_origin, const vec3 &ray_direction) {
  const vec3 oc = {
    ray_origin.x - sphere.position.x, ray_origin.y - sphere.position.y, ray_origin.z - sphere.position.z};

  const float a =
    ray_direction.x * ray_direction.x + ray_direction.y * ray_direction.y + ray_direction.z * ray_direction.z;
  const float b = 2.0f * (oc.x * ray_direction.x + oc.y * ray_direction.y + oc.z * ray_direction.z);
  const float c = oc.x * oc.x + oc.y * oc.y + oc.z * oc.z - sphere.radius * sphere.radius;

  const float discriminant = b * b - 4 * a * c;

  if (discriminant < 0)
    return std::nullopt;

  const float t = (-b - std::sqrt(discriminant)) / (2.0f * a);
  return t > 0 ? t : (-b + std::sqrt(discriminant)) / (2.0f * a);
}

int main() {
  std::vector<uchar4> image;
  image.resize(WIDTH * HEIGHT);

  Camera camera = Camera(45.0f, WIDTH, HEIGHT);
  fmt::println("{}", camera.cameraMatrix);
  fmt::println("{}", camera);

  return 0;

  // std::vector spheres = {
  //   Sphere{0.5f, {0, 0, -1}},
  // };
  //
  // math::Matrix<4, 4> mat = math::Identity<4, 4>();
  // mat(0, 0)              = 2;
  // math::Display(mat);
  //
  // math::Matrix<4, 4> mat2 = math::Identity<4, 4>();
  // math::Display(mat2);
  //
  // math::Display(mat * mat2);
  //
  // std::cout << "Hello world\n";
  // return 0;
}
