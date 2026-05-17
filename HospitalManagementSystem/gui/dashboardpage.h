#pragma once
#include <QWidget>
#include <QLabel>

class SystemManager;

class DashboardPage : public QWidget {
    Q_OBJECT
public:
    explicit DashboardPage(SystemManager* sm, QWidget* parent = nullptr);

public slots:
    void refresh();

private:
    SystemManager* sysManager;

    // Stat card value labels (updated on refresh)
    QLabel* hospitalsValueLabel;
    QLabel* doctorsValueLabel;
    QLabel* activePatientsValueLabel;
    QLabel* dischargedValueLabel;

    QWidget* createStatCard(const QString& title, const QString& value,
                             const QString& color, QLabel*& valueLabel);
};
