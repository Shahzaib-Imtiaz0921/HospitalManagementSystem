#pragma once
#include <QWidget>
#include <QTableWidget>
#include <QComboBox>
#include <QList>
#include <QMap>
#include <QString>
class SystemManager;

class DoctorsPage : public QWidget {
    Q_OBJECT
public:
    explicit DoctorsPage(SystemManager* sm, QWidget* parent = nullptr);
public slots:
    void refresh();
private slots:
    void onFilterChanged(const QString& hospitalName);
private:
    SystemManager* sysManager;
    QTableWidget*  table;
    QComboBox*     filterCombo;
    void populateTable(const QList<QMap<QString,QString>>& data);
};
