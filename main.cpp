#include "mainwindow.h"
#include "Partie.h"

#include <QApplication>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#define byte win_byte_override
#include <windows.h>
#endif


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // C'est ICI que l'on charge le style global si besoin
    MainWindow w;
    w.show();

    return a.exec(); // La boucle Qt prend le relais, std::cin ne fonctionnera plus
}
