#include "mainwindow.h"
#include "dashboardpage.h"
#include "hospitalspage.h"
#include "doctorspage.h"
#include "registerpatientpage.h"
#include "patientspage.h"
#include "aboutpage.h"

#include <QWidget>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , sysManager(new SystemManager())
{
    setupUI();
    applyStylesheet();
}

MainWindow::~MainWindow()
{
    delete sysManager;
}

SystemManager* MainWindow::getSystemManager()
{
    return sysManager;
}

void MainWindow::setupUI()
{
    // Central widget with horizontal layout
    QWidget* central = new QWidget(this);
    QHBoxLayout* layout = new QHBoxLayout(central);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // Navigation sidebar
    navList = new QListWidget(this);
    navList->setFixedWidth(200);
    navList->addItem("🏥  Dashboard");
    navList->addItem("🏨  Hospitals");
    navList->addItem("👨‍⚕️  Doctors");
    navList->addItem("📋  Register Patient");
    navList->addItem("👥  Patients");
    navList->addItem("ℹ️  About");

    // Page stack
    pageStack = new QStackedWidget(this);

    // Create page instances
    dashboardPage = new DashboardPage(sysManager, this);
    hospitalsPage = new HospitalsPage(sysManager, this);
    doctorsPage   = new DoctorsPage(sysManager, this);
    registerPage  = new RegisterPatientPage(sysManager, this);
    patientsPage  = new PatientsPage(sysManager, this);
    aboutPage     = new AboutPage(this);

    // Add pages to stack (order must match nav items)
    pageStack->addWidget(dashboardPage);
    pageStack->addWidget(hospitalsPage);
    pageStack->addWidget(doctorsPage);
    pageStack->addWidget(registerPage);
    pageStack->addWidget(patientsPage);
    pageStack->addWidget(aboutPage);

    layout->addWidget(navList);
    layout->addWidget(pageStack);

    setCentralWidget(central);

    // Connect navigation
    connect(navList, &QListWidget::currentRowChanged,
            this,    &MainWindow::onNavItemChanged);

    // Cross-page refresh: when a patient is registered, update Dashboard and Patients
    connect(registerPage, &RegisterPatientPage::patientRegistered,
            dashboardPage, &DashboardPage::refresh);
    connect(registerPage, &RegisterPatientPage::patientRegistered,
            patientsPage,  &PatientsPage::refresh);

    // Select Dashboard by default
    navList->setCurrentRow(0);

    setWindowTitle("Hospital Management System — Bahria University");
    setMinimumSize(1200, 700);
}

void MainWindow::onNavItemChanged(int index)
{
    pageStack->setCurrentIndex(index);

    if (index == 0) {
        dashboardPage->refresh();
    } else if (index == 4) {
        patientsPage->refresh();
    }
}

void MainWindow::applyStylesheet()
{
    setStyleSheet(R"(
QMainWindow {
    background-color: #f0f4f8;
}
QListWidget {
    background-color: #1a2744;
    color: #ffffff;
    border: none;
    font-size: 14px;
    padding: 8px 0px;
    outline: none;
}
QListWidget::item {
    padding: 14px 20px;
    border-left: 4px solid transparent;
}
QListWidget::item:selected {
    background-color: #2e86de;
    border-left: 4px solid #ffffff;
    color: #ffffff;
}
QListWidget::item:hover:!selected {
    background-color: #243460;
}
QStackedWidget {
    background-color: #f0f4f8;
}
QWidget {
    font-family: "Segoe UI", Arial, sans-serif;
}
QPushButton {
    background-color: #2e86de;
    color: white;
    border: none;
    padding: 8px 20px;
    border-radius: 4px;
    font-size: 13px;
    font-weight: bold;
}
QPushButton:hover {
    background-color: #1a6fc4;
}
QPushButton:pressed {
    background-color: #155a9e;
}
QTableWidget {
    background-color: white;
    gridline-color: #e0e0e0;
    border: 1px solid #d0d0d0;
    border-radius: 4px;
}
QTableWidget::item {
    padding: 6px;
}
QHeaderView::section {
    background-color: #1a2744;
    color: white;
    padding: 8px;
    border: none;
    font-weight: bold;
}
QLineEdit, QTextEdit, QSpinBox, QComboBox {
    border: 1px solid #c0c0c0;
    border-radius: 4px;
    padding: 6px;
    background-color: white;
    font-size: 13px;
}
QLineEdit:focus, QTextEdit:focus, QSpinBox:focus, QComboBox:focus {
    border: 1px solid #2e86de;
}
QLabel {
    color: #2c3e50;
}
)");
}
