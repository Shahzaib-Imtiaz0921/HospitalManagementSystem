#include "aboutpage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QTableWidget>
#include <QHeaderView>

static QFrame* makeInfoCard(const QString& title, const QString& value)
{
    QFrame* card = new QFrame;
    card->setStyleSheet(
        "QFrame {"
        "  background-color: #ffffff;"
        "  border-radius: 8px;"
        "  border: 1px solid #e0e0e0;"
        "  padding: 16px;"
        "}"
    );

    QVBoxLayout* layout = new QVBoxLayout(card);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(6);

    QLabel* titleLabel = new QLabel(title);
    titleLabel->setStyleSheet("font-size: 11px; color: #7f8c8d; border: none;");

    QLabel* valueLabel = new QLabel(value);
    valueLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #1a2744; border: none;");
    valueLabel->setWordWrap(true);

    layout->addWidget(titleLabel);
    layout->addWidget(valueLabel);

    return card;
}

static QTableWidget* makeStyledTable(int rows, const QStringList& headers)
{
    QTableWidget* table = new QTableWidget(rows, headers.size());
    table->setHorizontalHeaderLabels(headers);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setStretchLastSection(true);
    table->horizontalHeader()->setStyleSheet(
        "QHeaderView::section {"
        "  background-color: #1a2744;"
        "  color: #ffffff;"
        "  font-weight: bold;"
        "  padding: 6px;"
        "  border: none;"
        "}"
    );
    table->setStyleSheet(
        "QTableWidget {"
        "  border: 1px solid #e0e0e0;"
        "  border-radius: 4px;"
        "  gridline-color: #e0e0e0;"
        "}"
        "QTableWidget::item {"
        "  padding: 6px;"
        "}"
    );
    return table;
}

AboutPage::AboutPage(QWidget* parent) : QWidget(parent)
{
    setStyleSheet("background-color: #f0f4f8;");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(16);

    // ── Project header ────────────────────────────────────────────────────────
    QLabel* titleLabel = new QLabel("🏥 Hospital Management System");
    titleLabel->setStyleSheet("font-size: 26px; font-weight: bold; color: #1a2744;");
    titleLabel->setAlignment(Qt::AlignCenter);

    QLabel* subtitleLabel = new QLabel("Object Oriented Programming — C++ Lab Project");
    subtitleLabel->setStyleSheet("font-size: 14px; color: #7f8c8d;");
    subtitleLabel->setAlignment(Qt::AlignCenter);

    QFrame* separator = new QFrame;
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    separator->setStyleSheet("color: #e0e0e0;");

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(subtitleLabel);
    mainLayout->addWidget(separator);

    // ── Info cards row ────────────────────────────────────────────────────────
    QHBoxLayout* cardsLayout = new QHBoxLayout;
    cardsLayout->setSpacing(12);

    cardsLayout->addWidget(makeInfoCard("📚 Course",     "Object Oriented Programming"));
    cardsLayout->addWidget(makeInfoCard("👨‍🏫 Instructor", "Engr. Muhammad Faisal Zia"));
    cardsLayout->addWidget(makeInfoCard("🏛️ University", "Bahria University"));
    cardsLayout->addWidget(makeInfoCard("📍 Campus",     "H11 Campus, Islamabad"));

    mainLayout->addLayout(cardsLayout);

    // ── OOP Concepts section ──────────────────────────────────────────────────
    QLabel* oopHeader = new QLabel("✅ OOP Concepts Demonstrated");
    oopHeader->setStyleSheet("font-size: 16px; font-weight: bold; color: #1a2744;");

    QTableWidget* oopTable = makeStyledTable(6, {"Concept", "Implementation"});

    struct OopRow { QString concept; QString impl; };
    const OopRow oopRows[] = {
        { "Encapsulation",
          "Private data members with public getters/setters in all classes" },
        { "Inheritance",
          "Doctor and Patient inherit from abstract Person class" },
        { "Polymorphism",
          "displayInfo() is pure virtual in Person, overridden in Doctor and Patient" },
        { "Abstraction",
          "Person is an abstract base class with pure virtual displayInfo()" },
        { "Constructors/Destructors",
          "All classes have explicit constructors; DatabaseManager has destructor" },
        { "Classes & Objects",
          "Person, Doctor, Patient, Hospital, ConsultationSession, "
          "ProximityEngine, DatabaseManager, SystemManager" }
    };

    for (int i = 0; i < 6; ++i) {
        oopTable->setItem(i, 0, new QTableWidgetItem(oopRows[i].concept));
        oopTable->setItem(i, 1, new QTableWidgetItem(oopRows[i].impl));
    }
    oopTable->resizeColumnsToContents();
    oopTable->horizontalHeader()->setStretchLastSection(true);

    mainLayout->addWidget(oopHeader);
    mainLayout->addWidget(oopTable);

    // ── Group Members section ─────────────────────────────────────────────────
    QLabel* membersHeader = new QLabel("👥 Group Members");
    membersHeader->setStyleSheet("font-size: 16px; font-weight: bold; color: #1a2744;");

    QTableWidget* membersTable = makeStyledTable(3, {"Name", "Roll No", "Email"});
    membersTable->setFixedHeight(130);

    struct Member { QString name; QString roll; QString email; };
    const Member members[] = {
        { "Shahzaib", "059", "shahzaib.bscs@bahria.edu.pk" },
        { "Rabail",   "069", "rabail.bscs@bahria.edu.pk"   },
        { "Hijab",    "027", "hijab.bscs@bahria.edu.pk"    }
    };
    for (int i = 0; i < 3; ++i) {
        membersTable->setItem(i, 0, new QTableWidgetItem(members[i].name));
        membersTable->setItem(i, 1, new QTableWidgetItem(members[i].roll));
        membersTable->setItem(i, 2, new QTableWidgetItem(members[i].email));
    }
    membersTable->resizeColumnsToContents();
    membersTable->horizontalHeader()->setStretchLastSection(true);

    mainLayout->addWidget(membersHeader);
    mainLayout->addWidget(membersTable);

    // ── Bottom stretch ────────────────────────────────────────────────────────
    mainLayout->addStretch();
}
