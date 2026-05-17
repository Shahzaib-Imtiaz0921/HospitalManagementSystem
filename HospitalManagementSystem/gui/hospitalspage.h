#pragma once
#include <QWidget>
#include <QTableWidget>
class SystemManager;

class HospitalsPage : public QWidget {
    Q_OBJECT
public:
    explicit HospitalsPage(SystemManager* sm, QWidget* parent = nullptr);
public slots:
    void refresh();
private:
    SystemManager* sysManager;
    QTableWidget*  table;
};
