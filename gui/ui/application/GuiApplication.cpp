#include "ui/views/includes/InitialView.hpp"
#include <QGuiApplication>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickView>

int main(int argc, char* argv[])
{
    // create application main window
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setApplicationName("Modbus Data-Logger");

    QGuiApplication app(argc, argv);

    QQuickView view;
    view.connect(view.engine(), &QQmlEngine::quit, &app, &QCoreApplication::quit);
    view.setSource(QUrl("qrc:/qml/MainWindow.qml"));

    if (view.status() == QQuickView::Error) {
        return -1;
    }

    const auto rootContext = view.rootContext();

    // instantiate views
    Views::InitialView initialView;
    rootContext->setContextProperty("initialView", &initialView);

    // show application main window
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.show();

    return app.exec();
}
