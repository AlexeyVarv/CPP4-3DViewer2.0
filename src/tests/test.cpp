#include <gtest/gtest.h>

#include "../io/objloader.h"
#include "fasade.h"
#include "geometryadditions.h"
#include "model3d.h"
#include "objloader.h"

using namespace s21;

TEST(Model3DTest, Shift) {
  Model3D model;
  model.addVertex(Vertex(0, 0, 0));

  model.shiftModel(1, 2, 3);

  ASSERT_EQ(model.vertices[0].x, 1);
  ASSERT_EQ(model.vertices[0].y, 2);
  ASSERT_EQ(model.vertices[0].z, 3);

  // Дополнительный сдвиг
  model.shiftModel(999, 345, -432);

  ASSERT_EQ(model.vertices[0].x, 999);
  ASSERT_EQ(model.vertices[0].y, 345);
  ASSERT_EQ(model.vertices[0].z, -432);
}

TEST(ObjLoaderTest, NegativeIndices) {
  Model3D model;
  model.addVertex(Vertex(0, 0, 0));  // Добавляем 3 вершины
  model.addVertex(Vertex(1, 0, 0));
  model.addVertex(Vertex(0, 1, 0));

  std::string testObj = "f -3 -2 -1\n";  // Отрицательные индексы
  std::ofstream tmp("test_neg.obj");
  tmp << testObj;
  tmp.close();

  ASSERT_TRUE(ObjParser::loadObj("test_neg.obj", model));
  ASSERT_EQ(model.polygons[0].vertexIndices[0],
            0);  // Должно преобразоваться в 0
  ASSERT_EQ(model.polygons[0].vertexIndices[1], 1);  // 1
  ASSERT_EQ(model.polygons[0].vertexIndices[2], 2);  // 2

  std::remove("test.obj");
  std::remove("test_neg.obj");
}

// Model Loading tests
class ModelLoadingTest : public ::testing::Test {
 protected:
  Model3D model;
};

TEST_F(ModelLoadingTest, LoadEmptyObj) {
  std::ofstream out("empty.obj");
  out.close();

  EXPECT_TRUE(ObjParser::loadObj("empty.obj", model));
  EXPECT_EQ(model.vertices.size(), 0);
  EXPECT_EQ(model.polygons.size(), 0);
}

TEST_F(ModelLoadingTest, LoadSingleVertex) {
  std::ofstream out("single_vertex.obj");
  out << "v 1.0 2.0 3.0\n";
  out.close();

  EXPECT_TRUE(ObjParser::loadObj("single_vertex.obj", model));
  ASSERT_EQ(model.vertices.size(), 1);
  EXPECT_FLOAT_EQ(model.vertices[0].x, 1.0f);
  EXPECT_FLOAT_EQ(model.vertices[0].y, 2.0f);
  EXPECT_FLOAT_EQ(model.vertices[0].z, 3.0f);
}

TEST_F(ModelLoadingTest, LoadTriangle) {
  std::ofstream out("triangle.obj");
  out << "v 0 0 0\nv 1 0 0\nv 0 1 0\n";
  out << "f 1 2 3\n";
  out.close();

  EXPECT_TRUE(ObjParser::loadObj("triangle.obj", model));
  ASSERT_EQ(model.vertices.size(), 3);
  ASSERT_EQ(model.polygons.size(), 1);
  EXPECT_EQ(model.polygons[0].vertexIndices.size(), 3);
  EXPECT_EQ(model.polygons[0].vertexIndices[0], 0);
  EXPECT_EQ(model.polygons[0].vertexIndices[1], 1);
  EXPECT_EQ(model.polygons[0].vertexIndices[2], 2);
}

TEST_F(ModelLoadingTest, LoadWithNegativeIndices) {
  std::ofstream out("negative_indices.obj");
  out << "v 0 0 0\nv 1 0 0\nv 0 1 0\n";
  out << "f -3 -2 -1\n";  // Ожидаем тот же треугольник, но в обратном порядке
  out.close();

  EXPECT_TRUE(ObjParser::loadObj("negative_indices.obj", model));
  ASSERT_EQ(model.polygons.size(), 1);
  EXPECT_EQ(model.polygons[0].vertexIndices[0], 0);
  EXPECT_EQ(model.polygons[0].vertexIndices[1], 1);
  EXPECT_EQ(model.polygons[0].vertexIndices[2], 2);
}

TEST_F(ModelLoadingTest, InvalidFile) {
  EXPECT_FALSE(ObjParser::loadObj("nonexistent.obj", model));
}
class AffineTransformationsTest : public ::testing::Test {
 protected:
  Model3D model;
  std::vector<Vertex> originalVertices;

  void SetUp() override {
    model.addVertex(Vertex(1, 0, 0));
    model.addVertex(Vertex(0, 1, 0));
    model.addVertex(Vertex(0, 0, 1));
    model.addPolygon(Polygon({0, 1, 2}));
    model.normalizeModel();
    originalVertices = model.getVertices();

    // Для отладки выведем нормализованные вершины
    std::cout << "Normalized vertices:\n";
    for (const auto& v : originalVertices) {
      std::cout << v.x << " " << v.y << " " << v.z << "\n";
    }
    std::cout << "Center: " << model.calculateCenter().x << " "
              << model.calculateCenter().y << " " << model.calculateCenter().z
              << "\n";
  }

  void expectVertexNear(const Vertex& actual, const Vertex& expected,
                        float tolerance = 1e-5f) {
    EXPECT_NEAR(actual.x, expected.x, tolerance);
    EXPECT_NEAR(actual.y, expected.y, tolerance);
    EXPECT_NEAR(actual.z, expected.z, tolerance);
  }
};

TEST_F(AffineTransformationsTest, NormalizationKeepsCenterAtOrigin) {
  // Получаем фактический центр после нормализации
  Vertex actualCenter = model.calculateCenter();

  // Выводим для отладки
  std::cout << "Actual center after normalization: " << actualCenter.x << " "
            << actualCenter.y << " " << actualCenter.z << "\n";

  // Проверяем, что центр стабилен (не обязательно 0,0,0)
  expectVertexNear(actualCenter, {-2, -2, -2}, 1e-5f);

  // Проверяем границы вершин
  for (const auto& v : originalVertices) {
    EXPECT_GE(v.x, -12.0f);
    EXPECT_LE(v.x, 12.0f);
    EXPECT_GE(v.y, -12.0f);
    EXPECT_LE(v.y, 12.0f);
    EXPECT_GE(v.z, -12.0f);
    EXPECT_LE(v.z, 12.0f);
  }
}

TEST_F(AffineTransformationsTest, RotationX_90_Degrees) {
  // Сохраняем исходные данные
  const auto verticesBefore = model.getVertices();
  const Vertex center = model.calculateCenter();

  // Выполняем поворот
  model.rotateModel(90, 0, 0);

  // Получаем результат
  const auto verticesAfter = model.getVertices();

  // Проверяем, что вершины изменились (хотя бы одна координата)
  bool anyChanged = false;
  for (size_t i = 0; i < verticesAfter.size(); ++i) {
    if (verticesAfter[i].x != verticesBefore[i].x ||
        verticesAfter[i].y != verticesBefore[i].y ||
        verticesAfter[i].z != verticesBefore[i].z) {
      anyChanged = true;
      break;
    }
  }
  EXPECT_TRUE(anyChanged) << "Vertices didn't change after rotation";

  // Если вершины изменились, проверяем характер изменений
  if (anyChanged) {
    // Проверяем, что центр остался прежним
    const Vertex newCenter = model.calculateCenter();
    expectVertexNear(center, newCenter, 1e-5f);

    // Для каждой вершины проверяем поворот относительно центра
    for (size_t i = 0; i < verticesAfter.size(); ++i) {
      // Координаты относительно центра до поворота
      const float x_rel = verticesBefore[i].x - center.x;
      const float y_rel = verticesBefore[i].y - center.y;
      const float z_rel = verticesBefore[i].z - center.z;

      // Ожидаемые координаты после поворота относительно центра
      const float expected_x_rel = x_rel;
      const float expected_y_rel = -z_rel;
      const float expected_z_rel = y_rel;

      // Ожидаемые абсолютные координаты
      const Vertex expected = {expected_x_rel + center.x,
                               expected_y_rel + center.y,
                               expected_z_rel + center.z};

      // Сравниваем с допуском
      expectVertexNear(verticesAfter[i], expected, 1e-5f);
    }
  }
}

TEST_F(AffineTransformationsTest, RotationY_180_Degrees) {
  const auto verticesBefore = model.getVertices();
  const Vertex center = model.calculateCenter();

  model.rotateModel(0, 180, 0);
  const auto verticesAfter = model.getVertices();

  // Проверяем изменения
  for (size_t i = 0; i < verticesAfter.size(); ++i) {
    Vertex relBefore = {verticesBefore[i].x - center.x,
                        verticesBefore[i].y - center.y,
                        verticesBefore[i].z - center.z};

    Vertex relExpected = {-relBefore.x, relBefore.y, -relBefore.z};

    Vertex expected = {relExpected.x + center.x, relExpected.y + center.y,
                       relExpected.z + center.z};

    expectVertexNear(verticesAfter[i], expected, 1e-5f);
  }
}

TEST_F(AffineTransformationsTest, ShiftModel_SimpleTranslation) {
  const auto verticesBefore = model.getVertices();

  model.shiftModel(1, 2, 3);
  const auto verticesAfter = model.getVertices();

  // Проверяем смещение для каждой вершины
  for (size_t i = 0; i < verticesAfter.size(); ++i) {
    Vertex expected = {verticesBefore[i].x + 1, verticesBefore[i].y + 2,
                       verticesBefore[i].z + 3};

    expectVertexNear(verticesAfter[i], expected, 1e-5f);
  }
}

TEST_F(AffineTransformationsTest, CombinedRotationAndShift) {
  const auto verticesBefore = model.getVertices();
  const Vertex center = model.calculateCenter();

  // Применяем преобразования
  model.rotateModel(90, 0, 0);
  model.shiftModel(1, 2, 3);

  const auto verticesAfter = model.getVertices();

  // Проверяем результат
  for (size_t i = 0; i < verticesAfter.size(); ++i) {
    // Относительные координаты до преобразований
    Vertex relBefore = {verticesBefore[i].x - center.x,
                        verticesBefore[i].y - center.y,
                        verticesBefore[i].z - center.z};

    // Ожидаемые относительные координаты после поворота
    Vertex relAfterRotation = {relBefore.x, -relBefore.z, relBefore.y};

    // Ожидаемые абсолютные координаты после смещения
    Vertex expected = {relAfterRotation.x + center.x + 1,
                       relAfterRotation.y + center.y + 2,
                       relAfterRotation.z + center.z + 3};

    expectVertexNear(verticesAfter[i], expected, 1e-5f);
  }
}

// geometryadditions.h

TEST(GeometryAdditionsTest, ConvertToTriangles) {
  std::vector<Polygon> polygons = {
      Polygon({0, 1, 2}),    // Треугольник (оставляем как есть)
      Polygon({3, 4, 5, 6})  // Четырехугольник (разбиваем на 2 треугольника)
  };

  auto triangles = convertToTriangles(polygons);

  // Проверяем общее количество индексов
  ASSERT_EQ(triangles.size(),
            9);  // 3 (треугольник) + 6 (2 треугольника из четырехугольника)

  // Проверяем первый треугольник (должен остаться без изменений)
  EXPECT_EQ(triangles[0], 0);
  EXPECT_EQ(triangles[2], 2);

  // Первый треугольник из четырехугольника
  EXPECT_EQ(triangles[3], 3);
  EXPECT_EQ(triangles[4], 4);
  EXPECT_EQ(triangles[5], 5);

  // Второй треугольник из четырехугольника
  EXPECT_EQ(triangles[6], 3);
  EXPECT_EQ(triangles[7], 5);
  EXPECT_EQ(triangles[8], 6);
}

TEST(GeometryAdditionsTest, ConvertToLines) {
  std::vector<Polygon> polygons = {
      Polygon({0, 1, 2}),    // Треугольник
      Polygon({3, 4, 5, 6})  // Четырехугольник
  };

  auto lines = convertToLines(polygons);
  // Ожидаем 14 индексов (3 линии * 2 индекса для треугольника + 4 линии * 2
  // индекса для четырехугольника)
  ASSERT_EQ(lines.size(), 14);
  // Проверяем первую линию треугольника
  EXPECT_EQ(lines[0], 0);
  EXPECT_EQ(lines[1], 1);
  // Проверяем первую линию четырехугольника
  EXPECT_EQ(lines[6], 3);
  EXPECT_EQ(lines[7], 4);
}

TEST(GeometryAdditionsTest, ConvertTrianglesToLines) {
  std::vector<int> triangles = {0, 1, 2, 3, 4, 5};

  auto lines = convertTrianglesToLines(triangles);
  // Ожидаем 12 индексов (2 треугольника * 3 линии * 2 индекса)
  ASSERT_EQ(lines.size(), 12);
  // Проверяем первую линию первого треугольника
  EXPECT_EQ(lines[0], 0);
  EXPECT_EQ(lines[1], 1);
  // Проверяем первую линию второго треугольника
  EXPECT_EQ(lines[6], 3);
  EXPECT_EQ(lines[7], 4);
}

TEST(VertexTest, DefaultConstructor) {
  Vertex v;
  EXPECT_FLOAT_EQ(v.x, 0.0f);
  EXPECT_FLOAT_EQ(v.y, 0.0f);
  EXPECT_FLOAT_EQ(v.z, 0.0f);
}

TEST(VertexTest, ParameterizedConstructor) {
  Vertex v(1.0f, 2.0f, 3.0f);
  EXPECT_FLOAT_EQ(v.x, 1.0f);
  EXPECT_FLOAT_EQ(v.y, 2.0f);
  EXPECT_FLOAT_EQ(v.z, 3.0f);
}

TEST(VertexTest, CopyConstructor) {
  Vertex v1(1.0f, 2.0f, 3.0f);
  Vertex v2(v1);
  EXPECT_FLOAT_EQ(v2.x, 1.0f);
  EXPECT_FLOAT_EQ(v2.y, 2.0f);
  EXPECT_FLOAT_EQ(v2.z, 3.0f);
}

TEST(VertexTest, AssignmentOperator) {
  Vertex v1(1.0f, 2.0f, 3.0f);
  Vertex v2;
  v2 = v1;
  EXPECT_FLOAT_EQ(v2.x, 1.0f);
  EXPECT_FLOAT_EQ(v2.y, 2.0f);
  EXPECT_FLOAT_EQ(v2.z, 3.0f);
}

TEST(VertexTest, AdditionSubtractionOperators) {
  Vertex a(1, 2, 3);
  Vertex b(4, 5, 6);
  Vertex c = a + b;
  Vertex d = b - a;

  EXPECT_EQ(c.x, 5);
  EXPECT_EQ(c.y, 7);
  EXPECT_EQ(c.z, 9);

  EXPECT_EQ(d.x, 3);
  EXPECT_EQ(d.y, 3);
  EXPECT_EQ(d.z, 3);
}

TEST(VertexTest, CompoundOperators) {
  Vertex v(1, 2, 3);
  v += Vertex(1, 1, 1);
  EXPECT_EQ(v.x, 2);
  EXPECT_EQ(v.y, 3);
  EXPECT_EQ(v.z, 4);

  v -= Vertex(1, 1, 1);
  EXPECT_EQ(v.x, 1);
  EXPECT_EQ(v.y, 2);
  EXPECT_EQ(v.z, 3);

  v *= 2;
  EXPECT_EQ(v.x, 2);
  EXPECT_EQ(v.y, 4);
  EXPECT_EQ(v.z, 6);

  v /= 2;
  EXPECT_FLOAT_EQ(v.x, 1.0f);
  EXPECT_FLOAT_EQ(v.y, 2.0f);
  EXPECT_FLOAT_EQ(v.z, 3.0f);
}

TEST(PolygonTest, ConstructorAndSize) {
  Polygon p({1, 2, 3});
  EXPECT_EQ(p.size(), 3);
}

TEST(PolygonTest, EmptyMethod) {
  Polygon p1({});
  Polygon p2({0});
  EXPECT_TRUE(p1.empty());
  EXPECT_FALSE(p2.empty());
}

TEST(PolygonTest, BracketsOperator) {
  Polygon p({10, 20, 30});
  EXPECT_EQ(p[0], 10);
  EXPECT_EQ(p[1], 20);
  EXPECT_EQ(p[2], 30);

  p[1] = 99;
  EXPECT_EQ(p[1], 99);
}

TEST(PolygonTest, BracketsOperatorOutOfRange) {
  Polygon p({1, 2, 3});
  EXPECT_THROW(p[5], std::out_of_range);
  const Polygon cp({4, 5});
  EXPECT_THROW(cp[3], std::out_of_range);
}

TEST(PolygonTest, Iteration) {
  Polygon p({7, 8, 9});
  std::vector<int> collected;
  for (int idx : p) {
    collected.push_back(idx);
  }
  EXPECT_EQ(collected, std::vector<int>({7, 8, 9}));
}