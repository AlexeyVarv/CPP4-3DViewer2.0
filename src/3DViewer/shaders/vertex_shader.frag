void MAIN() {
    // Нормализуем координаты относительно размера точки
    vec2 coord = gl_FragCoord.xy - vec2(0.5); // Центр точки
    float distance = length(coord); // Расстояние до центра

    
    // Рисуем круг
    //float radius = 1.0; // Радиус круга в нормализованных координатах
    float radius = pointSize * 2.0;
    if (distance > radius) {
        discard; // Отсекаем пиксели за пределами круга
    }

    FRAGCOLOR = vec4(baseColor.rgb, 1.0); // Устанавливаем цвет точки
}