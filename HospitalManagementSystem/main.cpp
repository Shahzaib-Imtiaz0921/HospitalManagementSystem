#include <QApplication>
#include "gui/mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("Hospital Management System");
    app.setOrganizationName("Bahria University");

    MainWindow window;
    window.show();

    return app.exec();
}
