/**
 * @file geometryadditions.h
 * @brief Объявления функций для преобразования и создания геометрических
 * примитивов.
 *
 * Содержит функции для преобразования полигонов в треугольники, линии и
 * создания геометрических рендереров для отображения моделей в Qt3D.
 */

#ifndef GEOMETRYADDITIONS_H
#define GEOMETRYADDITIONS_H

#include <QByteArray>
#include <Qt3DCore/QAttribute>
#include <Qt3DCore/QBuffer>
#include <Qt3DCore/QEntity>
#include <Qt3DCore/QGeometry>
#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DRender/QMaterial>
#include <Qt3DRender/QPointLight>
#include <QtCore/QCoreApplication>
#include <QtGui/QGuiApplication>

#include "../io/objloader.h"
#include "model3d.h"

using namespace s21;

/**
 * @brief Преобразует полигоны в треугольники.
 * @param polygons Вектор полигонов, каждый из которых — список индексов
 вершин.
 * @return Вектор индексов треугольников.
 */
std::vector<int> convertToTriangles(const std::vector<Polygon>& polygons);

/**
 * @brief Преобразует полигоны в линии (ребра).
 * @param polygons Вектор полигонов.
 * @return Вектор индексов рёбер.
 */
std::vector<int> convertToLines(const std::vector<Polygon>& polygons);

/**x
 * @brief Преобразует треугольники в линии.
 * @param triangleIndices Индексы треугольников (по 3 на треугольник).
 * @return Вектор индексов рёбер.
 */
std::vector<int> convertTrianglesToLines(
    const std::vector<int>& triangleIndices);

#endif
