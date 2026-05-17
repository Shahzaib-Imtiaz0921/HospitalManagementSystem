#pragma once
#include <QDialog>
#include "../domain/consultationsession.h"
#include "../domain/patient.h"
#include "../domain/doctor.h"

class SystemManager;

class ConsultationResultDialog : public QDialog {
    Q_OBJECT
public:
    explicit ConsultationResultDialog(const ConsultationSession& session,
                                      const Patient& patient,
                                      const Doctor& doctor,
                                      SystemManager* mgr,
                                      QWidget* parent = nullptr);

private slots:
    void onDischarge();

private:
    ConsultationSession m_session;
    Patient             m_patient;
    Doctor              m_doctor;
    SystemManager*      sysManager;
};
