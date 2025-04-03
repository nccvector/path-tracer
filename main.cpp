#include <array>
#include <cmath>
#include <fmt/core.h>
#include <glm/glm.hpp>
#include <optional>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

#define PI 3.14159;

constexpr int WIDTH  = 8;
constexpr int HEIGHT = 4;


// Specialization of fmt::formatter for Matrix<R, C>
template<>
struct fmt::formatter<glm::mat3> {
  constexpr auto parse(fmt::format_parse_context &ctx) { return ctx.begin(); }

  template<typename FormatContext>
  auto format(const glm::mat3 &mat, FormatContext &ctx) const {
    auto out = ctx.out();
    return fmt::format_to(out, "{}", glm::to_string(mat));
  }
};

template<>
struct fmt::formatter<glm::mat4> {
  constexpr auto parse(fmt::format_parse_context &ctx) { return ctx.begin(); }

  template<typename FormatContext>
  auto format(const glm::mat4 &mat, FormatContext &ctx) const {
    auto out = ctx.out();
    return fmt::format_to(out, "{}", glm::to_string(mat));
  }
};

template<>
struct fmt::formatter<glm::vec3> {
  constexpr auto parse(fmt::format_parse_context &ctx) { return ctx.begin(); }

  template<typename FormatContext>
  auto format(const glm::mat3 &vec, FormatContext &ctx) const {
    auto out = ctx.out();
    return fmt::format_to(out, "{}", glm::to_string(vec));
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

glm::mat3 ComputeCameraMatrix(const float verticalFOVDegrees, const float imageWidth, const float imageHeight) {
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

  return {{fx, 0, cx}, {0, fy, cy}, {0, 0, 1}};
}


struct uchar4 {
  unsigned char r, g, b, a;
};


struct Sphere {
  float     radius;
  glm::vec3 position;
};


struct Ray {
  glm::vec3 origin;
  glm::vec3 direction;
};

template<>
struct fmt::formatter<Ray> {
  constexpr auto parse(fmt::format_parse_context &ctx) { return ctx.begin(); }

  template<typename FormatContext>
  auto format(const Ray &ray, FormatContext &ctx) const {
    auto out = ctx.out();
    return fmt::format_to(out, "Ray: {{origin: {}\tdirection: {}}}", glm::to_string(ray.origin), glm::to_string(ray.direction));
  }
};

struct Camera {
  float     verticalFov;
  int       imageWidth;
  int       imageHeight;
  glm::mat3 cameraMatrix;
  glm::mat3 cameraMatrixInverse;
  glm::mat4 transform;

  Camera(float verticalFov, int imageWidth, int imageHeight) :
      verticalFov(verticalFov), imageWidth(imageWidth), imageHeight(imageHeight) {
    cameraMatrix        = ComputeCameraMatrix(verticalFov, imageWidth, imageHeight);
    cameraMatrixInverse = glm::inverse(cameraMatrix);
    transform           = glm::mat4(1);
  }

  std::vector<Ray> GetRaysInLocalFrame() {
    std::vector<Ray> rays(imageWidth * imageHeight);
    for (int y = 0; y < imageHeight; y++) {
      for (int x = 0; x < imageWidth; x++) {
        rays[x + y * imageWidth] = {{0, 0, 0}, {glm::normalize(cameraMatrixInverse * glm::vec3{x, y, 1})}};
      }
    }
    return rays;
  }

  std::vector<Ray> GetTransformedRays() {
    glm::vec3        cameraPosition = transform[3]; // last column as vec3
    std::vector<Ray> rays(imageWidth * imageHeight);
    for (int y = 0; y < imageHeight; y++) {
      for (int x = 0; x < imageWidth; x++) {
        glm::vec3 rayDirection   = glm::normalize(cameraMatrixInverse * glm::vec3{x, y, 1});
        rays[x + y * imageWidth] = {cameraPosition, glm::mat3(transform) * rayDirection};
      }
    }
    return rays;
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
    fmt::format_to(ctx.out(), "\tcameraMatrixInverse = {}\n", camera.cameraMatrixInverse);
    fmt::format_to(ctx.out(), "\ttransform = {}\n", camera.transform);
    fmt::format_to(ctx.out(), "}}");
    return ctx.out();
  }
};

template<>
struct fmt::formatter<std::vector<Ray>> {
  constexpr auto parse(fmt::format_parse_context &ctx) { return ctx.begin(); }

  template<typename FormatContext>
  auto format(const std::vector<Ray> &rays, FormatContext &ctx) const {
    auto out = ctx.out();
    for (auto &ray: rays) {
      fmt::format_to(out, "{}\n", ray);
    }
    return out;
  }
};

std::optional<float> Intersect(const Sphere &sphere, const glm::vec3 &ray_origin, const glm::vec3 &ray_direction) {
  const glm::vec3 oc = {
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
  fmt::println("{}", camera.GetRaysInLocalFrame());

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
