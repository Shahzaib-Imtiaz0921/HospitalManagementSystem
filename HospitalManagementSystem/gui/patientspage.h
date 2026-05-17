#pragma once
#include <QWidget>
#include <QTableWidget>
#include <QPushButton>

class SystemManager;

class PatientsPage : public QWidget {
    Q_OBJECT
public:
    explicit PatientsPage(SystemManager* sm, QWidget* parent = nullptr);

public slots:
    void refresh();

private slots:
    void onExportCSV();

private:
    SystemManager*  sysManager;
    QTableWidget*   table;
    QPushButton*    exportBtn;
    QPushButton*    refreshBtn;
};
