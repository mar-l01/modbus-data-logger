#include "domain/application/includes/ModbusDataLoggerFacadeFactory.hpp"
#include "domain/application/includes/ModbusDataLoggerFrameworks.hpp"
#include "ui/views/includes/InitialView.hpp"

#include <QGuiApplication>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickView>

int main(int argc, char* argv[])
{
    // TODO (mar-l01, 01.01.2021): Workaround to set the Modbus configuration file
    if (argc <= 1) {
        return 1;
    }
    const std::string mbConfigFile(argv[1]);

    // create application main window
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setApplicationName("Modbus Data-Logger");

    QGuiApplication app(argc, argv);
    QQuickView view;

    // create facade
    const Application::FrameworkDependencies frameworkDependencies(
      Framework::LoggingFramework::SPDLOG, Framework::FileReaderFramework::NLOHMANN_JSON,
      Gateway::ModbusComponentsFramework::LIBMODBUS, Gateway::ModbusComponentsFramework::LIBMODBUS);
    const auto mbDataLoggerFacade =
      Application::ModbusDataLoggerFacadeFactory::createModbusDataLoggerFacade(mbConfigFile, frameworkDependencies);

    const auto rootContext = view.rootContext();

    // instantiate view models
    Views::InitialView initialView(mbDataLoggerFacade);
    rootContext->setContextProperty("initialView", &initialView);

    // show main window
    view.connect(view.engine(), &QQmlEngine::quit, &app, &QCoreApplication::quit);
    view.setSource(QUrl("qrc:/qml/MainWindow"));

    if (view.status() == QQuickView::Error) {
        return -1;
    }

    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.show();

    return app.exec();
}
