# 3DViewer v2.0

## Требования

Для работы с проектом требуется установленная библиотека **QtQuick3D** (часть Qt6). Если вы устанавливаете Qt в нестандартную директорию, убедитесь, что вы правильно настроили переменные окружения.

### Установка Qt

1. Скачайте и установите Qt6 с официального сайта: [https://www.qt.io/download](https://www.qt.io/download).
2. Если вы используете нестандартную директорию для установки (например, `/usr/lib/qt6`), добавьте путь к исполняемым файлам в переменную `PATH`:

   ```bash
   export PATH=/usr/lib/qt6/bin:$PATH

## Сборка проекта
`Makefile`:
| #   | Target      | Description of the target                |
| --- | ----------- | ---------------------------------------- |
| 1   | `all`       | Builds the project                       |
| 2   | `install`   | Builds the project                       |
| 3   | `uninstall` | Uninstalls the project                   |
| 4   | `clean`     | Cleans the repository of generated files |
| 5   | `dvi`       | Builds project documentation             |
| 6   | `run`       | Launch the app                           |