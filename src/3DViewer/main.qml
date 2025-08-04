import QtQuick
import QtQuick3D
import QtQuick.Dialogs
import QtQuick.Controls
import QtQuick.Layouts

Window {
    visible: true
    width: 1280
    height: 960

    // Счетчик для уникальных имен файлов
    property int fileCounter: 1

    // Диалог выбора файла
    FileDialog {
        id: fileDialog
        title: "Select OBJ File"
        nameFilters: ["OBJ Files (*.obj)"]
    }

    View3D {
        id: view3d
        anchors.fill: parent

        // Переключатель проекции
        property bool perspectiveProjection: true

        environment: SceneEnvironment {
            id: sceneEnvironment
            clearColor: appSettings.backgroundColor
            backgroundMode: SceneEnvironment.Color
        }

        PerspectiveCamera {
            id: perspectiveCamera
            position: Qt.vector3d(0, 300, 2000)
            eulerRotation.x: -5
            visible: appSettings.isPerspective
        }

        OrthographicCamera {
            id: orthographicCamera
            position: Qt.vector3d(0, 300, 2000)
            eulerRotation.x: -5
            visible: !appSettings.isPerspective
        }

        DirectionalLight {
            eulerRotation.x: -30
            eulerRotation.y: -70
        }

        Model {
            id: model
            scale: Qt.vector3d(100, 100, 100)
            materials: PrincipledMaterial {
                lighting: PrincipledMaterial.NoLighting
                baseColor: "#42a5f5"
            }
        }

        Model {
            id: linesModel
            scale: Qt.vector3d(100, 100, 100)
            materials: principledMaterial
            PrincipledMaterial {
                id: principledMaterial
                baseColor: appSettings.lineColor
                lineWidth: appSettings.lineWidth
                lighting: PrincipledMaterial.NoLighting
            }
            CustomMaterial {
                id: dashedMaterial
                property real dashLength: 0.1
                property real gapLength: 0.1
                property color baseColor: appSettings.lineColor

                fragmentShader: "qrc:/shaders/dashed_shader.frag"
            }
        }

        Model {
            id: verticesModel
            scale: Qt.vector3d(100, 100, 100)
            materials: principledVertexMaterial
            CustomMaterial {
                id: vertexMaterial
                property color baseColor: appSettings.vertexColor
                property real pointSize: appSettings.pointSize  // Ширина круга

                shadingMode: CustomMaterial.Unshaded

                fragmentShader: "qrc:/shaders/vertex_shader.frag"
            }
            PrincipledMaterial {
                id: principledVertexMaterial
                baseColor: appSettings.vertexColor
                pointSize: appSettings.pointSize
                lighting: PrincipledMaterial.NoLighting
            }

            visible: appSettings.showVertices
        }
    }

    ColumnLayout {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        // Первая панель (информация о модели)
        Pane {
            Layout.fillWidth: true // Занимает всю доступную ширину
            contentHeight: infoLayout.implicitHeight

            RowLayout {
                id: infoLayout
                spacing: 10
                anchors.fill: parent

                // Кнопка для загрузки модели
                Button {
                    text: "Load Model"
                    onClicked: {
                        fileDialog.open(); // Открываем диалог выбора файла
                    }
                }

                // Информация о файле
                Rectangle {
                    color: "#FFFFFF"
                    radius: 5
                    width: 200 // Фиксированная ширина
                    height: 25 // Фиксированная высота
                    Text {
                        id: fileNameText
                        text: "File: none"
                        anchors.centerIn: parent
                        font.pixelSize: 12
                        elide: Text.ElideRight // Обрезаем текст, если он слишком длинный
                        wrapMode: Text.WrapAnywhere // Переносим текст на новую строку
                        maximumLineCount: 1 // Ограничиваем две строки
                    }
                }

                // Информация о вершинах
                Rectangle {
                    color: "#FFFFFF"
                    radius: 5
                    width: 200
                    height: 25
                    Text {
                        id: vertexInfo
                        text: "Vertices: " + facade.vertexCount()
                        font.pixelSize: 12
                        anchors.centerIn: parent
                    }
                }

                // Информация о полигонах
                Rectangle {
                    color: "#FFFFFF"
                    radius: 5
                    width: 200
                    height: 25
                    Text {
                        id: polygonInfo
                        text: "Polygons: " + facade.polygonCount()
                        font.pixelSize: 12
                        anchors.centerIn: parent
                    }
                }

                Button {
                    text: "Save as .jpeg"
                    onClicked: {
                        // Генерируем уникальное имя файла, добавляя номер
                        var uniqueFilename = "rendered_scene_" + fileCounter + ".jpeg";
                        facade.saveRenderImage(view3d, uniqueFilename);

                        // Увеличиваем счетчик для следующего сохранения
                        fileCounter++;
                    }
                }

                Button {
                    text: "Save as .bmp"
                    onClicked: {
                        // Генерируем уникальное имя файла для BMP
                        var uniqueFilename = "rendered_scene_" + fileCounter + ".bmp";
                        facade.saveRenderImage(view3d, uniqueFilename);

                        // Увеличиваем счетчик для следующего сохранения
                        fileCounter++;
                    }
                }

                Button {
                    text: "Save as .gif"
                    onClicked: {
                        facade.startGifRecording(view3d);
                    }
                }

                // Пустой элемент, который занимает все доступное пространство
                Item {
                    Layout.fillWidth: true // Занимает всё свободное место
                }

                Button {
                    id: settingsButton
                    text: "Settings"
                    onClicked: {
                        // Вычисляем позицию Popup
                        // let buttonPosition = settingsButton.mapToGlobal(Qt.point(0, 0));
                        let buttonPosition = settingsButton.mapToItem(null, 0, 0);
                        let popupX = buttonPosition.x - settingsPopup.width + settingsButton.width;
                        let popupY = buttonPosition.y + settingsButton.height + 15;

                        // Если Popup выходит за левый край экрана, показываем его справа от кнопки
                        if (popupX < 0) {
                            popupX = buttonPosition.x + settingsButton.width;
                        }

                        settingsPopup.x = popupX;
                        settingsPopup.y = popupY;
                        settingsPopup.open();
                    }
                }
            }

            Connections {
                target: fileDialog
                function onAccepted() {
                    var objFilePath;
                    if (fileDialog.selectedFile) {
                        objFilePath = fileDialog.selectedFile;
                        // Извлекаем только имя файла из полного пути
                        var fileName = objFilePath.toString().split('/').pop();
                        fileNameText.text = "File: " + fileName;
                        facade.loadModel(objFilePath);
                    } else {
                        console.log("No file selected.");
                        fileNameText.text = "File: none";
                    }
                    verticesModel.geometry = facade.createVerticesView();
                    linesModel.geometry = facade.createLinesView();
                }
            }

            Connections {
                target: facade
                // Обновление информации о вершинах
                function onVertexCountChanged() {
                    vertexInfo.text = "Vertices: " + facade.vertexCount();
                }
                // Обновление информации о полигонах
                function onPolygonCountChanged() {
                    polygonInfo.text = "Polygons: " + facade.polygonCount();
                }
            }
        }
    }

    Popup {
        id: settingsPopup
        width: 250
        height: 400
        modal: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
        focus: true

        // Свойства для хранения значений параметров
        property real lineValue: appSettings.lineWidth
        property real vertexValue: appSettings.pointSize
        property color lineColorValue: appSettings.lineColor
        property color vertexColorValue: appSettings.vertexColor
        property color backgroundColorValue: appSettings.backgroundColor
        property int projectionType: appSettings.isPerspective ? 0 : 1
        property bool showVerticesValue: appSettings.showVertices
        property int lineStyleType: 0

        ColumnLayout {
            anchors.fill: parent
            spacing: 10

            // Выбор типа проекции
            ComboBox {
                id: projectionCombo
                model: ["Perspective", "Orthographic"]
                currentIndex: settingsPopup.projectionType
                onCurrentIndexChanged: settingsPopup.projectionType = currentIndex
            }

            // Выбор типа линии
            ComboBox {
                id: lineStyleComboBox
                width: 200
                model: ["Solid Lines", "Dashed Lines"]
                currentIndex: settingsPopup.lineStyleType
                onCurrentIndexChanged: settingsPopup.lineStyleType = currentIndex
            }

            // Выбор цвета линий
            Button {
                text: "Line Color"
                onClicked: lineColorDialog.open()
            }

            ColorDialog {
                id: lineColorDialog
                title: "Choose Line Color"
                selectedColor: settingsPopup.lineColorValue
                onAccepted: settingsPopup.lineColorValue = selectedColor
            }

            // Настройка ширины линий
            RowLayout {
                Label {
                    text: "Line Width:"
                }
                Slider {
                    from: 1
                    to: 10
                    value: settingsPopup.lineValue
                    onValueChanged: settingsPopup.lineValue = value
                }
            }

            // Настройка вершин
            CheckBox {
                id: showVerticesCheckBox
                text: "Show Vertices"
                checked: settingsPopup.showVerticesValue
                onCheckedChanged: settingsPopup.showVerticesValue = checked
            }

            // Настройка цвета вершин
            Button {
                text: "Vertex Color"
                onClicked: vertexColorDialog.open()
            }

            ColorDialog {
                id: vertexColorDialog
                title: "Choose Vertex Color"
                selectedColor: settingsPopup.vertexColorValue
                onAccepted: settingsPopup.vertexColorValue = selectedColor
            }

            // Настройка размера вершин
            RowLayout {
                Label {
                    text: "Vertex Size:"
                }
                Slider {
                    from: 1
                    to: 10
                    value: settingsPopup.vertexValue
                    onValueChanged: settingsPopup.vertexValue = value
                }
            }

            // Настройка цвета фона
            Button {
                text: "Background Color"
                onClicked: backgroundColorDialog.open()
            }

            ColorDialog {
                id: backgroundColorDialog
                title: "Choose Background Color"
                selectedColor: settingsPopup.backgroundColorValue
                onAccepted: settingsPopup.backgroundColorValue = selectedColor
            }

            // Кнопка "Apply"
            Button {
                text: "Apply"
                onClicked: {
                    // Применяем тип проекции
                    appSettings.setIsPerspective(settingsPopup.projectionType === 0);
                    appSettings.setSolidLines(settingsPopup.lineStyleType === 0);
                    // Применяем тип линий
                    if (settingsPopup.lineStyleType === 0) {
                        linesModel.materials = [principledMaterial]; // Сплошные линии
                    } else if (settingsPopup.lineStyleType === 1) {
                        linesModel.materials = [dashedMaterial]; // Пунктирные линии
                    }

                    // Применяем цвета
                    appSettings.setLineColor(settingsPopup.lineColorValue);
                    appSettings.setVertexColor(settingsPopup.vertexColorValue);
                    appSettings.setBackgroundColor(settingsPopup.backgroundColorValue);

                    // Применяем размеры
                    appSettings.setLineWidth(settingsPopup.lineValue);
                    appSettings.setPointSize(settingsPopup.vertexValue);

                    // Применяем отображение вершин
                    appSettings.setShowVertices(settingsPopup.showVerticesValue);

                    // Обновляем материалы и геометрию
                    linesModel.materials[0].baseColor = settingsPopup.lineColorValue;
                    linesModel.materials[0].lineWidth = settingsPopup.lineValue;

                    verticesModel.materials[0].baseColor = settingsPopup.vertexColorValue;
                    verticesModel.materials[0].pointSize = settingsPopup.vertexValue;

                    linesModel.geometry = facade.createLinesView();
                    verticesModel.geometry = facade.createVerticesView();

                    // Устанавливаем цвет фона
                    sceneEnvironment.clearColor = settingsPopup.backgroundColorValue;

                    settingsPopup.close(); // Закрываем диалог
                }
            }
        }
    }

    // Панель с кнопками и полями ввода
    ColumnLayout {
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 20
        spacing: 10

        // Ползунок для перемещения по оси X
        RowLayout {
            Layout.alignment: Qt.AlignRight
            spacing: 10

            // Контейнер для поля ввода с рамкой
            Rectangle {
                id: xMoveInputContainer
                width: 70  // Общая ширина контейнера
                height: 30
                color: "transparent"  // Прозрачный фон
                border.color: "#AAAAAA"  // Цвет рамки
                border.width: 1  // Толщина рамки
                radius: 4  // Скругление углов

                // Поле ввода
                TextInput {
                    id: xMoveInput
                    anchors.centerIn: parent
                    width: 50  // Ширина текстового поля
                    text: xMoveSlider.value.toFixed(0)
                    validator: IntValidator {
                        bottom: -1000
                        top: 1000
                    }
                    horizontalAlignment: TextInput.AlignHCenter
                    font.pixelSize: 14

                    // Обновление слайдера при нажатии Enter
                    Keys.onReturnPressed: {
                        xMoveSlider.value = Number(text);
                    }
                }
            }

            // Кнопка Apply
            Button {
                text: "Shift X"
                width: 100  // Фиксированная ширина кнопки
                onClicked: {
                    xMoveSlider.value = Number(xMoveInput.text);
                    facade.shiftModel(xMoveSlider.value / 100, yMoveSlider.value / 100, zMoveSlider.value / 100);
                }
            }
        }

        Slider {
            id: xMoveSlider
            from: -1000
            to: 1000
            Layout.alignment: Qt.AlignRight
            onValueChanged: {
                xMoveInput.text = value.toFixed(0);  // Обновляем поле ввода
                facade.shiftModel(xMoveSlider.value / 100, yMoveSlider.value / 100, zMoveSlider.value / 100);
            }
        }

        // Ползунок для перемещения по оси Y
        RowLayout {
            Layout.alignment: Qt.AlignRight
            spacing: 10

            // Контейнер для поля ввода с рамкой
            Rectangle {
                id: yMoveInputContainer
                width: 70  // Общая ширина контейнера
                height: 30
                color: "transparent"  // Прозрачный фон
                border.color: "#AAAAAA"  // Цвет рамки
                border.width: 1  // Толщина рамки
                radius: 4  // Скругление углов

                // Поле ввода
                TextInput {
                    id: yMoveInput
                    anchors.centerIn: parent
                    width: 50  // Ширина текстового поля
                    text: yMoveSlider.value.toFixed(0)
                    validator: IntValidator {
                        bottom: -1000
                        top: 1000
                    }
                    horizontalAlignment: TextInput.AlignHCenter
                    font.pixelSize: 14

                    // Обновление слайдера при нажатии Enter
                    Keys.onReturnPressed: {
                        yMoveSlider.value = Number(text);
                    }
                }
            }

            // Кнопка Apply
            Button {
                text: "Shift Y"
                width: 100  // Фиксированная ширина кнопки
                onClicked: {
                    yMoveSlider.value = Number(yMoveInput.text);
                    facade.shiftModel(xMoveSlider.value / 100, yMoveSlider.value / 100, zMoveSlider.value / 100);
                }
            }
        }

        Slider {
            id: yMoveSlider
            from: -1000
            to: 1000
            Layout.alignment: Qt.AlignRight
            onValueChanged: {
                yMoveInput.text = value.toFixed(0);  // Обновляем поле ввода
                facade.shiftModel(xMoveSlider.value / 100, yMoveSlider.value / 100, zMoveSlider.value / 100);
            }
        }

        // Ползунок для перемещения по оси Z
        RowLayout {
            Layout.alignment: Qt.AlignRight
            spacing: 10

            // Контейнер для поля ввода с рамкой
            Rectangle {
                id: zMoveInputContainer
                width: 70  // Общая ширина контейнера
                height: 30
                color: "transparent"  // Прозрачный фон
                border.color: "#AAAAAA"  // Цвет рамки
                border.width: 1  // Толщина рамки
                radius: 4  // Скругление углов

                // Поле ввода
                TextInput {
                    id: zMoveInput
                    anchors.centerIn: parent
                    width: 50  // Ширина текстового поля
                    text: zMoveSlider.value.toFixed(0)
                    validator: IntValidator {
                        bottom: -1000
                        top: 1000
                    }
                    horizontalAlignment: TextInput.AlignHCenter
                    font.pixelSize: 14

                    // Обновление слайдера при нажатии Enter
                    Keys.onReturnPressed: {
                        zMoveSlider.value = Number(text);
                    }
                }
            }

            // Кнопка Apply
            Button {
                text: "Shift Z"
                width: 100  // Фиксированная ширина кнопки
                onClicked: {
                    zMoveSlider.value = Number(zMoveInput.text);
                    facade.shiftModel(xMoveSlider.value / 100, yMoveSlider.value / 100, zMoveSlider.value / 100);
                }
            }
        }

        Slider {
            id: zMoveSlider
            from: -1000
            to: 1000
            Layout.alignment: Qt.AlignRight
            onValueChanged: {
                zMoveInput.text = value.toFixed(0);  // Обновляем поле ввода
                facade.shiftModel(xMoveSlider.value / 100, yMoveSlider.value / 100, zMoveSlider.value / 100);
            }
            Layout.bottomMargin: 30  // Особый отступ после этого элемента
        }

        // Ползунок для вращения по оси X
        RowLayout {
            Layout.alignment: Qt.AlignRight
            spacing: 10

            // Контейнер для поля ввода с рамкой
            Rectangle {
                id: xRotInputContainer
                width: 70  // Общая ширина контейнера
                height: 30
                color: "transparent"  // Прозрачный фон
                border.color: "#AAAAAA"  // Цвет рамки
                border.width: 1  // Толщина рамки
                radius: 4  // Скругление углов

                // Поле ввода
                TextInput {
                    id: xRotInput
                    anchors.centerIn: parent
                    width: 50  // Ширина текстового поля
                    text: xRotSlider.value.toFixed(0)
                    validator: IntValidator {
                        bottom: 0
                        top: 360
                    }
                    horizontalAlignment: TextInput.AlignHCenter  // Выравнивание по центру
                    font.pixelSize: 14

                    // Обновление слайдера при нажатии Enter
                    Keys.onReturnPressed: {
                        xRotSlider.value = Number(text);
                    }
                }
            }

            // Кнопка вращения
            Button {
                text: "Rotate X"
                width: 100  // Фиксированная ширина кнопки
                onClicked: {
                    xRotSlider.value = Number(xRotInput.text);
                    facade.rotateModel(xRotSlider.value, yRotSlider.value, zRotSlider.value);
                }
            }
        }

        Slider {
            id: xRotSlider
            from: 0
            to: 360
            Layout.alignment: Qt.AlignRight
            onValueChanged: {
                xRotInput.text = value.toFixed(0);  // Обновляем поле ввода при изменении слайдера
                facade.rotateModel(value, yRotSlider.value, zRotSlider.value);
            }
        }

        // Ползунок для вращения по оси Y
        RowLayout {
            Layout.alignment: Qt.AlignRight
            spacing: 10

            // Контейнер для поля ввода с рамкой
            Rectangle {
                id: yRotInputContainer
                width: 70  // Общая ширина контейнера
                height: 30
                color: "transparent"  // Прозрачный фон
                border.color: "#AAAAAA"  // Цвет рамки
                border.width: 1  // Толщина рамки
                radius: 4  // Скругление углов

                // Поле ввода
                TextInput {
                    id: yRotInput
                    anchors.centerIn: parent
                    width: 50  // Ширина текстового поля
                    text: yRotSlider.value.toFixed(0)
                    validator: IntValidator {
                        bottom: 0
                        top: 360
                    }
                    horizontalAlignment: TextInput.AlignHCenter  // Выравнивание по центру
                    font.pixelSize: 14

                    // Обновление слайдера при нажатии Enter
                    Keys.onReturnPressed: {
                        yRotSlider.value = Number(text);
                    }
                }
            }

            // Кнопка вращения
            Button {
                text: "Rotate Y"
                width: 100  // Фиксированная ширина кнопки
                onClicked: {
                    yRotSlider.value = Number(yRotInput.text);
                    facade.rotateModel(xRotSlider.value, yRotSlider.value, zRotSlider.value);
                }
            }
        }

        Slider {
            id: yRotSlider
            from: 0
            to: 360
            Layout.alignment: Qt.AlignRight
            onValueChanged: {
                yRotInput.text = value.toFixed(0);  // Обновляем поле ввода при изменении слайдера
                facade.rotateModel(xRotSlider.value, value, zRotSlider.value);
            }
        }

        // Ползунок для вращения по оси Z
        RowLayout {
            Layout.alignment: Qt.AlignRight
            spacing: 10

            // Контейнер для поля ввода с рамкой
            Rectangle {
                id: zRotInputContainer
                width: 70  // Общая ширина контейнера
                height: 30
                color: "transparent"  // Прозрачный фон
                border.color: "#AAAAAA"  // Цвет рамки
                border.width: 1  // Толщина рамки
                radius: 4  // Скругление углов

                // Поле ввода
                TextInput {
                    id: zRotInput
                    anchors.centerIn: parent
                    width: 50  // Ширина текстового поля
                    text: zRotSlider.value.toFixed(0)
                    validator: IntValidator {
                        bottom: 0
                        top: 360
                    }
                    horizontalAlignment: TextInput.AlignHCenter  // Выравнивание по центру
                    font.pixelSize: 14

                    // Обновление слайдера при нажатии Enter
                    Keys.onReturnPressed: {
                        zRotSlider.value = Number(text);
                    }
                }
            }

            // Кнопка вращения
            Button {
                text: "Rotate Z"
                width: 100  // Фиксированная ширина кнопки
                onClicked: {
                    zRotSlider.value = Number(zRotInput.text);
                    facade.rotateModel(xRotSlider.value, yRotSlider.value, zRotSlider.value);
                }
            }
        }

        Slider {
            id: zRotSlider
            from: 0
            to: 360
            Layout.alignment: Qt.AlignRight
            onValueChanged: {
                zRotInput.text = value.toFixed(0);  // Обновляем поле ввода при изменении слайдера
                facade.rotateModel(xRotSlider.value, yRotSlider.value, value);
            }
            Layout.bottomMargin: 30  // Особый отступ после этого элемента
        }

        ColumnLayout {
            Layout.alignment: Qt.AlignRight
            spacing: 5  // Отступ между элементами

            // Текст "Scale" и значение
            RowLayout {
                Layout.alignment: Qt.AlignHCenter  // Центрируем по горизонтали
                spacing: 5

                Text {
                    text: "Scale:"
                    font.pixelSize: 14
                }

                Text {
                    id: scaleValueText
                    text: scaleSlider.value.toFixed(0)
                    font.pixelSize: 14
                    font.bold: true
                }
            }

            // Слайдер
            Slider {
                id: scaleSlider
                from: 1
                to: 300
                value: 100
                Timer {
                    id: scaleDebounceTimer
                    interval: 100
                    onTriggered: {
                        linesModel.scale = Qt.vector3d(scaleSlider.value, scaleSlider.value, scaleSlider.value);
                        verticesModel.scale = Qt.vector3d(scaleSlider.value, scaleSlider.value, scaleSlider.value);
                        linesModel.geometry = facade.createLinesView();
                        verticesModel.geometry = facade.createVerticesView();
                    }
                }

                onValueChanged: {
                    scaleValueText.text = value.toFixed(0);
                    scaleDebounceTimer.restart();
                }
            }
        }

        Connections {
            target: facade
            function onGeometryUpdated() {
                linesModel.geometry = facade.createLinesView();
                verticesModel.geometry = facade.createVerticesView();
            }
        }
    }
}
