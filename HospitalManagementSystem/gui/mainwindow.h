#pragma once
#include <QMainWindow>
#include <QListWidget>
#include <QStackedWidget>
#include "../manager/systemmanager.h"

// Forward declarations for page classes
class DashboardPage;
class HospitalsPage;
class DoctorsPage;
class RegisterPatientPage;
class PatientsPage;
class AboutPage;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

    SystemManager* getSystemManager();

private slots:
    void onNavItemChanged(int index);

private:
    void setupUI();
    void applyStylesheet();

    SystemManager*       sysManager;
    QListWidget*         navList;
    QStackedWidget*      pageStack;

    DashboardPage*       dashboardPage;
    HospitalsPage*       hospitalsPage;
    DoctorsPage*         doctorsPage;
    RegisterPatientPage* registerPage;
    PatientsPage*        patientsPage;
    AboutPage*           aboutPage;
};
