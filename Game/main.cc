#include "engine.hh"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Q_INIT_RESOURCE(offlinedata);

    try {
        StudentSide::Engine engine;
        if (not engine.wasDialogAccepted()) {
            // Queueing quit() so it executes when the event loop starts
            QTimer::singleShot(0, qApp, &QCoreApplication::quit);
        }
        return a.exec();
    }
    catch (...) {
        return EXIT_FAILURE;
    }
}
