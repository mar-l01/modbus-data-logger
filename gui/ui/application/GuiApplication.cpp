#include "domain/application/includes/ModbusDataLoggerFacadeFactory.hpp"
#include "domain/application/includes/ModbusDataLoggerFrameworks.hpp"
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

    // create facade
    const auto configFile = "../resources/mbdl_config.json";
    const Application::FrameworkDependencies frameworkDependencies(
      Framework::LoggingFramework::SPDLOG, Framework::FileReaderFramework::NLOHMANN_JSON,
      Gateway::ModbusComponentsFramework::LIBMODBUS, Gateway::ModbusComponentsFramework::LIBMODBUS);
    const auto mbDataLoggerFacade =
      Application::ModbusDataLoggerFacadeFactory::createModbusDataLoggerFacade(configFile, frameworkDependencies);

    const auto rootContext = view.rootContext();

    // instantiate view models
    Views::InitialView initialView(mbDataLoggerFacade);
    rootContext->setContextProperty("initialView", &initialView);

    // show main window
    view.connect(view.engine(), &QQmlEngine::quit, &app, &QCoreApplication::quit);
    view.setSource(QUrl("qrc:/qml/MainWindow.qml"));

    if (view.status() == QQuickView::Error) {
        return -1;
    }

    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.show();

    return app.exec();
}
