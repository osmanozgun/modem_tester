#include <QApplication>
#include <QTabWidget>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFont>
#include "Veri_Ekle.h"
#include "Kayit_Listele.h"
#include "Testler.h"
#include "Programlama.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    QFont appFont("Arial", 11);
    appFont.setStyleStrategy(QFont::PreferAntialias);
    app.setFont(appFont);
    
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("modem.db");
    if (!db.open()) {
        QMessageBox::critical(nullptr, "Hata", "Veritabanı açılamadı!");
        return -1;
    }
    
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS modem_testleri ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "seri_no TEXT UNIQUE, "
               "eeprom INTEGER, "
               "rf INTEGER, "
               "network INTEGER, "
               "result TEXT, "
               "aciklama TEXT)");
               
    query.exec("CREATE TABLE IF NOT EXISTS modem_loglari ("
               "seri_no TEXT PRIMARY KEY, "
               "log TEXT)");
    
    TabVeriEkle *ekleSekmesi = new TabVeriEkle();
    TabKayitListele *listeSekmesi = new TabKayitListele();
    TestlerWidget *testSekmesi = new TestlerWidget();  // Yeni TestlerWidget kullanılıyor
    TabModemProgramla *modemSekmesi = new TabModemProgramla();
    
    QTabWidget tabWidget;
    tabWidget.setWindowTitle("Modem Test Uygulaması");
    tabWidget.addTab(ekleSekmesi, "Veri Ekle");
    tabWidget.addTab(listeSekmesi, "Kayıtları Listele");
    tabWidget.addTab(testSekmesi, "Testleri Başlat ve Sanal Sayaç");  // Sekme adı güncellendi
    tabWidget.addTab(modemSekmesi, "Modem Programla");
    tabWidget.resize(1200, 600);  // Yeni split layout için genişlik artırıldı
    
    QObject::connect(&tabWidget, &QTabWidget::currentChanged, [&](int index) {
        if (index == tabWidget.indexOf(listeSekmesi)) {
            listeSekmesi->tabloyuDoldur();
        }
    });
    
    tabWidget.show();
    return app.exec();
}