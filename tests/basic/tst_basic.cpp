#include <QtTest/QtTest>
#include <nickelspace.h>

class tst_Basic : public QObject
{
    W_OBJECT(tst_Basic)

private /*slots*/:
    void firstTest();
    W_SLOT(firstTest, W_Access::Private)

    void signalSlot_data();
    W_SLOT(signalSlot_data, W_Access::Private)
    void signalSlot();
    W_SLOT(signalSlot, W_Access::Private)

    void property_data();
    W_SLOT(property_data)

    void property();
    W_SLOT(property)
};

W_OBJECT_IMPL(tst_Basic)

void tst_Basic::firstTest()
{
    QCOMPARE(metaObject()->className(), "tst_Basic");
    QCOMPARE(metaObject()->superClass()->className(), "QObject");
}
/*
#undef Q_PROPERTY
#define WRITE , &ThisType::
#define READ , &ThisType::
#define NOTIFY , &ThisType::
#define MEMBER , &ThisType::

#define W_PROPERTY2(...)  W_PROPERTY(__VA_ARGS__)  // expands the WRITE and READ macro

*/

class BTestObj  : public QObject
{   W_OBJECT(BTestObj)
    QString value;
public:
    void setValue(const QString &s) { value = s; emit valueChanged(s); }
    W_SLOT(setValue)

    void resetValue() { value = QString(); }
    W_SLOT(resetValue)

    QString getValue() const { return value; }

public: /*signals*/
    W_SIGNAL_1(void valueChanged(const QString &s))
    W_SIGNAL_2(valueChanged, s)

    W_SIGNAL_1(void simpleSignal())
    W_SIGNAL_2(simpleSignal)

public:
    W_PROPERTY(QString, value1, &BTestObj::setValue, &BTestObj::getValue )
    QString member;
    W_PROPERTY(QString, member1, &BTestObj::member )
    W_PROPERTY(QString, all, &BTestObj::value, &BTestObj::setValue, &BTestObj::getValue)
};


W_OBJECT_IMPL(BTestObj)

void tst_Basic::signalSlot_data()
{
    QTest::addColumn<bool>("old");
    QTest::newRow("old") << true;
    QTest::newRow("new") << false;
}


void tst_Basic::signalSlot()
{
    BTestObj obj1, obj2;
    QFETCH(bool, old);
    if (old) {
        QVERIFY(connect(&obj1, SIGNAL(valueChanged(QString)), &obj2, SLOT(setValue(QString))));
        QVERIFY(connect(&obj1, SIGNAL(simpleSignal()), &obj2, SLOT(resetValue())));
    } else {
        QVERIFY(connect(&obj1, &BTestObj::valueChanged, &obj2, &BTestObj::setValue));
        QVERIFY(connect(&obj1, &BTestObj::simpleSignal, &obj2, &BTestObj::resetValue));
    }
    obj1.setValue("HOLLA");
    QCOMPARE(obj2.getValue(), QString("HOLLA"));
    emit obj1.simpleSignal();
    QCOMPARE(obj2.getValue(), QString());
}


void tst_Basic::property_data()
{
    QTest::addColumn<QByteArray>("name");
    QTest::newRow("value1") << "value1";
    QTest::newRow("member1") << "member1";
    QTest::newRow("all") << "all";
}


void tst_Basic::property()
{
    QFETCH(QByteArray, name);

    BTestObj obj;
    QString str = "TRALAL";
    QVERIFY(obj.setProperty(name, str));
    QCOMPARE(obj.property(name), QVariant(str));
}


QTEST_MAIN(tst_Basic)