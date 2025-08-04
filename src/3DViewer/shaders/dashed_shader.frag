void MAIN() {
    // Используем UV0.x как параметр для расчета паттерна
    float pattern = mod(UV0.x / (dashLength + gapLength), 1.0);
    if (pattern > dashLength / (dashLength + gapLength)) {
        discard; // Пропуск части линии
    }
    FRAGCOLOR = vec4(baseColor.rgb, 1.0); // Цвет линии
}