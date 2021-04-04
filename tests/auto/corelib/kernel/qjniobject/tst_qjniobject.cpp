/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <jni.h>

#include <QString>
#include <QtCore/QJniEnvironment>
#include <QtCore/QJniObject>
#include <QtTest>

static const char testClassName[] = "org/qtproject/qt/android/testdatapackage/QtJniObjectTestClass";

static const jbyte A_BYTE_VALUE = 127;
static const jshort A_SHORT_VALUE = 32767;
static const jint A_INT_VALUE = 060701;
static const jlong A_LONG_VALUE = 060701;
static const jfloat A_FLOAT_VALUE = 1.0;
static const jdouble A_DOUBLE_VALUE = 1.0;
static const jchar A_CHAR_VALUE = 'Q';

static QString A_STRING_OBJECT()
{
    return QStringLiteral("TEST_DATA_STRING");
}

class tst_QJniObject : public QObject
{
    Q_OBJECT

public:
    tst_QJniObject();

private slots:
    void initTestCase();

    void ctor();
    void callMethodTest();
    void callObjectMethodTest();
    void stringConvertionTest();
    void compareOperatorTests();
    void callStaticObjectMethodClassName();
    void callStaticObjectMethod();
    void callStaticBooleanMethodClassName();
    void callStaticBooleanMethod();
    void callStaticCharMethodClassName();
    void callStaticCharMethod();
    void callStaticIntMethodClassName();
    void callStaticIntMethod();
    void callStaticByteMethodClassName();
    void callStaticByteMethod();
    void callStaticDoubleMethodClassName();
    void callStaticDoubleMethod();
    void callStaticFloatMethodClassName();
    void callStaticFloatMethod();
    void callStaticLongMethodClassName();
    void callStaticLongMethod();
    void callStaticShortMethodClassName();
    void callStaticShortMethod();
    void getStaticObjectFieldClassName();
    void getStaticObjectField();
    void getStaticIntFieldClassName();
    void getStaticIntField();
    void getStaticByteFieldClassName();
    void getStaticByteField();
    void getStaticLongFieldClassName();
    void getStaticLongField();
    void getStaticDoubleFieldClassName();
    void getStaticDoubleField();
    void getStaticFloatFieldClassName();
    void getStaticFloatField();
    void getStaticShortFieldClassName();
    void getStaticShortField();
    void getStaticCharFieldClassName();
    void getStaticCharField();
    void getBooleanField();
    void getIntField();
    void templateApiCheck();
    void isClassAvailable();
    void fromLocalRef();

    void cleanupTestCase();
};

tst_QJniObject::tst_QJniObject()
{
}

void tst_QJniObject::initTestCase()
{
}

void tst_QJniObject::cleanupTestCase()
{
}

void tst_QJniObject::ctor()
{
    {
        QJniObject object;
        QVERIFY(!object.isValid());
    }

    {
        QJniObject object("java/lang/String");
        QVERIFY(object.isValid());
    }

    {
        QJniObject string = QJniObject::fromString(QLatin1String("Hello, Java"));
        QJniObject object("java/lang/String", "(Ljava/lang/String;)V", string.object<jstring>());
        QVERIFY(object.isValid());
        QCOMPARE(string.toString(), object.toString());
    }

    {
        QJniEnvironment env;
        jclass javaStringClass = env->FindClass("java/lang/String");
        QJniObject string(javaStringClass);
        QVERIFY(string.isValid());
    }

    {
        QJniEnvironment env;
        const QString qString = QLatin1String("Hello, Java");
        jclass javaStringClass = env->FindClass("java/lang/String");
        QJniObject string = QJniObject::fromString(qString);
        QJniObject stringCpy(javaStringClass, "(Ljava/lang/String;)V", string.object<jstring>());
        QVERIFY(stringCpy.isValid());
        QCOMPARE(qString, stringCpy.toString());
    }
}

void tst_QJniObject::callMethodTest()
{
    {
        QJniObject jString1 = QJniObject::fromString(QLatin1String("Hello, Java"));
        QJniObject jString2 = QJniObject::fromString(QLatin1String("hELLO, jAVA"));
        QVERIFY(jString1 != jString2);

        const jboolean isEmpty = jString1.callMethod<jboolean>("isEmpty");
        QVERIFY(!isEmpty);

        const jint ret = jString1.callMethod<jint>("compareToIgnoreCase",
                                                   "(Ljava/lang/String;)I",
                                                   jString2.object<jstring>());
        QVERIFY(0 == ret);
    }

    {
        jlong jLong = 100;
        QJniObject longObject("java/lang/Long", "(J)V", jLong);
        jlong ret = longObject.callMethod<jlong>("longValue");
        QCOMPARE(ret, jLong);
    }
}

void tst_QJniObject::callObjectMethodTest()
{
    const QString qString = QLatin1String("Hello, Java");
    QJniObject jString = QJniObject::fromString(qString);
    const QString qStringRet = jString.callObjectMethod<jstring>("toUpperCase").toString();
    QCOMPARE(qString.toUpper(), qStringRet);

    QJniObject subString = jString.callObjectMethod("substring",
                                                    "(II)Ljava/lang/String;",
                                                    0, 4);
    QCOMPARE(subString.toString(), qString.mid(0, 4));
}

void tst_QJniObject::stringConvertionTest()
{
    const QString qString(QLatin1String("Hello, Java"));
    QJniObject jString = QJniObject::fromString(qString);
    QVERIFY(jString.isValid());
    QString qStringRet = jString.toString();
    QCOMPARE(qString, qStringRet);
}

void tst_QJniObject::compareOperatorTests()
{
    QString str("hello!");
    QJniObject stringObject = QJniObject::fromString(str);

    jobject obj = stringObject.object();
    jobject jobj = stringObject.object<jobject>();
    jstring jsobj = stringObject.object<jstring>();

    QVERIFY(obj == stringObject);
    QVERIFY(jobj == stringObject);
    QVERIFY(stringObject == jobj);
    QVERIFY(jsobj == stringObject);
    QVERIFY(stringObject == jsobj);

    QJniObject stringObject3 = stringObject.object<jstring>();
    QVERIFY(stringObject3 == stringObject);

    QJniObject stringObject2 = QJniObject::fromString(str);
    QVERIFY(stringObject != stringObject2);

    jstring jstrobj = 0;
    QJniObject invalidStringObject;
    QVERIFY(invalidStringObject == jstrobj);

    QVERIFY(jstrobj != stringObject);
    QVERIFY(stringObject != jstrobj);
    QVERIFY(!invalidStringObject.isValid());
}

void tst_QJniObject::callStaticObjectMethodClassName()
{
    QJniObject formatString = QJniObject::fromString(QLatin1String("test format"));
    QVERIFY(formatString.isValid());

    QVERIFY(QJniObject::isClassAvailable("java/lang/String"));
    QJniObject returnValue = QJniObject::callStaticObjectMethod("java/lang/String",
                                                                "format",
                                                                "(Ljava/lang/String;[Ljava/lang/Object;)Ljava/lang/String;",
                                                                formatString.object<jstring>(),
                                                                jobjectArray(0));
    QVERIFY(returnValue.isValid());

    QString returnedString = returnValue.toString();

    QCOMPARE(returnedString, QString::fromLatin1("test format"));
}

void tst_QJniObject::callStaticObjectMethod()
{
    QJniEnvironment env;
    jclass cls = env->FindClass("java/lang/String");
    QVERIFY(cls != 0);

    QJniObject formatString = QJniObject::fromString(QLatin1String("test format"));
    QVERIFY(formatString.isValid());

    QJniObject returnValue = QJniObject::callStaticObjectMethod(cls,
                                                                "format",
                                                                "(Ljava/lang/String;[Ljava/lang/Object;)Ljava/lang/String;",
                                                                formatString.object<jstring>(),
                                                                jobjectArray(0));
    QVERIFY(returnValue.isValid());

    QString returnedString = returnValue.toString();

    QCOMPARE(returnedString, QString::fromLatin1("test format"));
}

void tst_QJniObject::callStaticBooleanMethod()
{
    QJniEnvironment env;
    jclass cls = env->FindClass("java/lang/Boolean");
    QVERIFY(cls != 0);

    {
        QJniObject parameter = QJniObject::fromString("true");
        QVERIFY(parameter.isValid());

        jboolean b = QJniObject::callStaticMethod<jboolean>(cls,
                                                            "parseBoolean",
                                                            "(Ljava/lang/String;)Z",
                                                            parameter.object<jstring>());
        QVERIFY(b);
    }

    {
        QJniObject parameter = QJniObject::fromString("false");
        QVERIFY(parameter.isValid());

        jboolean b = QJniObject::callStaticMethod<jboolean>(cls,
                                                            "parseBoolean",
                                                            "(Ljava/lang/String;)Z",
                                                            parameter.object<jstring>());
        QVERIFY(!b);
    }
}

void tst_QJniObject::callStaticBooleanMethodClassName()
{
    {
        QJniObject parameter = QJniObject::fromString("true");
        QVERIFY(parameter.isValid());

        jboolean b = QJniObject::callStaticMethod<jboolean>("java/lang/Boolean",
                                                            "parseBoolean",
                                                            "(Ljava/lang/String;)Z",
                                                            parameter.object<jstring>());
        QVERIFY(b);
    }

    {
        QJniObject parameter = QJniObject::fromString("false");
        QVERIFY(parameter.isValid());

        jboolean b = QJniObject::callStaticMethod<jboolean>("java/lang/Boolean",
                                                            "parseBoolean",
                                                            "(Ljava/lang/String;)Z",
                                                            parameter.object<jstring>());
        QVERIFY(!b);
    }
}

void tst_QJniObject::callStaticByteMethodClassName()
{
    QString number = QString::number(123);
    QJniObject parameter = QJniObject::fromString(number);

    jbyte returnValue = QJniObject::callStaticMethod<jbyte>("java/lang/Byte",
                                                            "parseByte",
                                                            "(Ljava/lang/String;)B",
                                                            parameter.object<jstring>());
    QCOMPARE(returnValue, jbyte(number.toInt()));
}

void tst_QJniObject::callStaticByteMethod()
{
    QJniEnvironment env;
    jclass cls = env->FindClass("java/lang/Byte");
    QVERIFY(cls != 0);

    QString number = QString::number(123);
    QJniObject parameter = QJniObject::fromString(number);

    jbyte returnValue = QJniObject::callStaticMethod<jbyte>(cls,
                                                            "parseByte",
                                                            "(Ljava/lang/String;)B",
                                                            parameter.object<jstring>());
    QCOMPARE(returnValue, jbyte(number.toInt()));
}

void tst_QJniObject::callStaticIntMethodClassName()
{
    QString number = QString::number(123);
    QJniObject parameter = QJniObject::fromString(number);

    jint returnValue = QJniObject::callStaticMethod<jint>("java/lang/Integer",
                                                          "parseInt",
                                                          "(Ljava/lang/String;)I",
                                                          parameter.object<jstring>());
    QCOMPARE(returnValue, number.toInt());
}


void tst_QJniObject::callStaticIntMethod()
{
    QJniEnvironment env;
    jclass cls = env->FindClass("java/lang/Integer");
    QVERIFY(cls != 0);

    QString number = QString::number(123);
    QJniObject parameter = QJniObject::fromString(number);

    jint returnValue = QJniObject::callStaticMethod<jint>(cls,
                                                          "parseInt",
                                                          "(Ljava/lang/String;)I",
                                                          parameter.object<jstring>());
    QCOMPARE(returnValue, number.toInt());
}

void tst_QJniObject::callStaticCharMethodClassName()
{
    jchar returnValue = QJniObject::callStaticMethod<jchar>("java/lang/Character",
                                                            "toUpperCase",
                                                            "(C)C",
                                                            jchar('a'));
    QCOMPARE(returnValue, jchar('A'));
}


void tst_QJniObject::callStaticCharMethod()
{
    QJniEnvironment env;
    jclass cls = env->FindClass("java/lang/Character");
    QVERIFY(cls != 0);

    jchar returnValue = QJniObject::callStaticMethod<jchar>(cls,
                                                            "toUpperCase",
                                                            "(C)C",
                                                            jchar('a'));
    QCOMPARE(returnValue, jchar('A'));
}

void tst_QJniObject::callStaticDoubleMethodClassName    ()
{
    QString number = QString::number(123.45);
    QJniObject parameter = QJniObject::fromString(number);

    jdouble returnValue = QJniObject::callStaticMethod<jdouble>("java/lang/Double",
                                                                "parseDouble",
                                                                "(Ljava/lang/String;)D",
                                                                parameter.object<jstring>());
    QCOMPARE(returnValue, number.toDouble());
}


void tst_QJniObject::callStaticDoubleMethod()
{
    QJniEnvironment env;
    jclass cls = env->FindClass("java/lang/Double");
    QVERIFY(cls != 0);

    QString number = QString::number(123.45);
    QJniObject parameter = QJniObject::fromString(number);

    jdouble returnValue = QJniObject::callStaticMethod<jdouble>(cls,
                                                                "parseDouble",
                                                                "(Ljava/lang/String;)D",
                                                                parameter.object<jstring>());
    QCOMPARE(returnValue, number.toDouble());
}

void tst_QJniObject::callStaticFloatMethodClassName()
{
    QString number = QString::number(123.45);
    QJniObject parameter = QJniObject::fromString(number);

    jfloat returnValue = QJniObject::callStaticMethod<jfloat>("java/lang/Float",
                                                              "parseFloat",
                                                              "(Ljava/lang/String;)F",
                                                              parameter.object<jstring>());
    QCOMPARE(returnValue, number.toFloat());
}


void tst_QJniObject::callStaticFloatMethod()
{
    QJniEnvironment env;
    jclass cls = env->FindClass("java/lang/Float");
    QVERIFY(cls != 0);

    QString number = QString::number(123.45);
    QJniObject parameter = QJniObject::fromString(number);

    jfloat returnValue = QJniObject::callStaticMethod<jfloat>(cls,
                                                              "parseFloat",
                                                              "(Ljava/lang/String;)F",
                                                              parameter.object<jstring>());
    QCOMPARE(returnValue, number.toFloat());
}

void tst_QJniObject::callStaticShortMethodClassName()
{
    QString number = QString::number(123);
    QJniObject parameter = QJniObject::fromString(number);

    jshort returnValue = QJniObject::callStaticMethod<jshort>("java/lang/Short",
                                                              "parseShort",
                                                              "(Ljava/lang/String;)S",
                                                              parameter.object<jstring>());
    QCOMPARE(returnValue, number.toShort());
}


void tst_QJniObject::callStaticShortMethod()
{
    QJniEnvironment env;
    jclass cls = env->FindClass("java/lang/Short");
    QVERIFY(cls != 0);

    QString number = QString::number(123);
    QJniObject parameter = QJniObject::fromString(number);

    jshort returnValue = QJniObject::callStaticMethod<jshort>(cls,
                                                              "parseShort",
                                                              "(Ljava/lang/String;)S",
                                                              parameter.object<jstring>());
    QCOMPARE(returnValue, number.toShort());
}

void tst_QJniObject::callStaticLongMethodClassName()
{
    QString number = QString::number(123);
    QJniObject parameter = QJniObject::fromString(number);

    jlong returnValue = QJniObject::callStaticMethod<jlong>("java/lang/Long",
                                                            "parseLong",
                                                            "(Ljava/lang/String;)J",
                                                            parameter.object<jstring>());
    QCOMPARE(returnValue, jlong(number.toLong()));
}

void tst_QJniObject::callStaticLongMethod()
{
    QJniEnvironment env;
    jclass cls = env->FindClass("java/lang/Long");
    QVERIFY(cls != 0);

    QString number = QString::number(123);
    QJniObject parameter = QJniObject::fromString(number);

    jlong returnValue = QJniObject::callStaticMethod<jlong>(cls,
                                                            "parseLong",
                                                            "(Ljava/lang/String;)J",
                                                            parameter.object<jstring>());
    QCOMPARE(returnValue, jlong(number.toLong()));
}

void tst_QJniObject::getStaticObjectFieldClassName()
{
    {
        QJniObject boolObject = QJniObject::getStaticObjectField("java/lang/Boolean",
                                                                 "FALSE",
                                                                 "Ljava/lang/Boolean;");
        QVERIFY(boolObject.isValid());

        jboolean booleanValue = boolObject.callMethod<jboolean>("booleanValue");
        QVERIFY(!booleanValue);
    }

    {
        QJniObject boolObject = QJniObject::getStaticObjectField("java/lang/Boolean",
                                                                 "TRUE",
                                                                 "Ljava/lang/Boolean;");
        QVERIFY(boolObject.isValid());

        jboolean booleanValue = boolObject.callMethod<jboolean>("booleanValue");
        QVERIFY(booleanValue);
    }

    {
        QJniObject boolObject = QJniObject::getStaticObjectField("java/lang/Boolean",
                                                                 "FALSE",
                                                                 "Ljava/lang/Boolean;");
        QVERIFY(boolObject.isValid());
        jboolean booleanValue = boolObject.callMethod<jboolean>("booleanValue");
        QVERIFY(!booleanValue);
    }
}

void tst_QJniObject::getStaticObjectField()
{
    QJniEnvironment env;
    jclass cls = env->FindClass("java/lang/Boolean");
    QVERIFY(cls != 0);

    {
        QJniObject boolObject = QJniObject::getStaticObjectField(cls,
                                                                 "FALSE",
                                                                 "Ljava/lang/Boolean;");
        QVERIFY(boolObject.isValid());

        jboolean booleanValue = boolObject.callMethod<jboolean>("booleanValue");
        QVERIFY(!booleanValue);
    }

    {
        QJniObject boolObject = QJniObject::getStaticObjectField(cls,
                                                                 "TRUE",
                                                                 "Ljava/lang/Boolean;");
        QVERIFY(boolObject.isValid());

        jboolean booleanValue = boolObject.callMethod<jboolean>("booleanValue");
        QVERIFY(booleanValue);
    }

    {
        QJniObject boolObject = QJniObject::getStaticObjectField(cls,
                                                                 "FALSE",
                                                                 "Ljava/lang/Boolean;");
        QVERIFY(boolObject.isValid());

        jboolean booleanValue = boolObject.callMethod<jboolean>("booleanValue");
        QVERIFY(!booleanValue);
    }
}

void tst_QJniObject::getStaticIntFieldClassName()
{
    jint i = QJniObject::getStaticField<jint>("java/lang/Double", "SIZE");
    QCOMPARE(i, 64);
}

void tst_QJniObject::getStaticIntField()
{
    QJniEnvironment env;
    jclass cls = env->FindClass("java/lang/Double");
    QVERIFY(cls != 0);

    jint i = QJniObject::getStaticField<jint>(cls, "SIZE");
    QCOMPARE(i, 64);
}

void tst_QJniObject::getStaticByteFieldClassName()
{
    jbyte i = QJniObject::getStaticField<jbyte>("java/lang/Byte", "MAX_VALUE");
    QCOMPARE(i, jbyte(127));
}

void tst_QJniObject::getStaticByteField()
{
    QJniEnvironment env;
    jclass cls = env->FindClass("java/lang/Byte");
    QVERIFY(cls != 0);

    jbyte i = QJniObject::getStaticField<jbyte>(cls, "MAX_VALUE");
    QCOMPARE(i, jbyte(127));
}

void tst_QJniObject::getStaticLongFieldClassName()
{
    jlong i = QJniObject::getStaticField<jlong>("java/lang/Long", "MAX_VALUE");
    QCOMPARE(i, jlong(9223372036854775807L));
}

void tst_QJniObject::getStaticLongField()
{
    QJniEnvironment env;
    jclass cls = env->FindClass("java/lang/Long");
    QVERIFY(cls != 0);

    jlong i = QJniObject::getStaticField<jlong>(cls, "MAX_VALUE");
    QCOMPARE(i, jlong(9223372036854775807L));
}

void tst_QJniObject::getStaticDoubleFieldClassName()
{
    jdouble i = QJniObject::getStaticField<jdouble>("java/lang/Double", "NaN");
    jlong *k = reinterpret_cast<jlong*>(&i);
    QCOMPARE(*k, jlong(0x7ff8000000000000L));
}

void tst_QJniObject::getStaticDoubleField()
{
    QJniEnvironment env;
    jclass cls = env->FindClass("java/lang/Double");
    QVERIFY(cls != 0);

    jdouble i = QJniObject::getStaticField<jdouble>(cls, "NaN");
    jlong *k = reinterpret_cast<jlong*>(&i);
    QCOMPARE(*k, jlong(0x7ff8000000000000L));
}

void tst_QJniObject::getStaticFloatFieldClassName()
{
    jfloat i = QJniObject::getStaticField<jfloat>("java/lang/Float", "NaN");
    unsigned *k = reinterpret_cast<unsigned*>(&i);
    QCOMPARE(*k, unsigned(0x7fc00000));
}

void tst_QJniObject::getStaticFloatField()
{
    QJniEnvironment env;
    jclass cls = env->FindClass("java/lang/Float");
    QVERIFY(cls != 0);

    jfloat i = QJniObject::getStaticField<jfloat>(cls, "NaN");
    unsigned *k = reinterpret_cast<unsigned*>(&i);
    QCOMPARE(*k, unsigned(0x7fc00000));
}

void tst_QJniObject::getStaticShortFieldClassName()
{
    jshort i = QJniObject::getStaticField<jshort>("java/lang/Short", "MAX_VALUE");
    QCOMPARE(i, jshort(32767));
}

void tst_QJniObject::getStaticShortField()
{
    QJniEnvironment env;
    jclass cls = env->FindClass("java/lang/Short");
    QVERIFY(cls != 0);

    jshort i = QJniObject::getStaticField<jshort>(cls, "MAX_VALUE");
    QCOMPARE(i, jshort(32767));
}

void tst_QJniObject::getStaticCharFieldClassName()
{
    jchar i = QJniObject::getStaticField<jchar>("java/lang/Character", "MAX_VALUE");
    QCOMPARE(i, jchar(0xffff));
}

void tst_QJniObject::getStaticCharField()
{
    QJniEnvironment env;
    jclass cls = env->FindClass("java/lang/Character");
    QVERIFY(cls != 0);

    jchar i = QJniObject::getStaticField<jchar>(cls, "MAX_VALUE");
    QCOMPARE(i, jchar(0xffff));
}


void tst_QJniObject::getBooleanField()
{
    QJniObject obj("org/qtproject/qt/android/QtActivityDelegate");

    QVERIFY(obj.isValid());
    QVERIFY(!obj.getField<jboolean>("m_fullScreen"));
}

void tst_QJniObject::getIntField()
{
    QJniObject obj("org/qtproject/qt/android/QtActivityDelegate");

    QVERIFY(obj.isValid());
    jint res = obj.getField<jint>("m_currentRotation");
    QCOMPARE(res, -1);
}

void tst_QJniObject::templateApiCheck()
{
    QJniObject testClass(testClassName);
    QVERIFY(testClass.isValid());

    // void ---------------------------------------------------------------------------------------
    QJniObject::callStaticMethod<void>(testClassName, "staticVoidMethod");
    QJniObject::callStaticMethod<void>(testClassName,
                                       "staticVoidMethodWithArgs",
                                       "(IZC)V",
                                       1,
                                       true,
                                       'c');

    testClass.callMethod<void>("voidMethod");
    testClass.callMethod<void>("voidMethodWithArgs", "(IZC)V", 1, true, 'c');

    // jboolean -----------------------------------------------------------------------------------
    QVERIFY(QJniObject::callStaticMethod<jboolean>(testClassName, "staticBooleanMethod"));
    QVERIFY(QJniObject::callStaticMethod<jboolean>(testClassName,
                                                   "staticBooleanMethodWithArgs",
                                                   "(ZZZ)Z",
                                                   true,
                                                   true,
                                                   true));

    QVERIFY(testClass.callMethod<jboolean>("booleanMethod"));
    QVERIFY(testClass.callMethod<jboolean>("booleanMethodWithArgs",
                                           "(ZZZ)Z",
                                           true,
                                           true,
                                           true));

    // jbyte --------------------------------------------------------------------------------------
    QVERIFY(QJniObject::callStaticMethod<jbyte>(testClassName,
                                                "staticByteMethod") == A_BYTE_VALUE);
    QVERIFY(QJniObject::callStaticMethod<jbyte>(testClassName,
                                                "staticByteMethodWithArgs",
                                                "(BBB)B",
                                                1,
                                                1,
                                                1) == A_BYTE_VALUE);

    QVERIFY(testClass.callMethod<jbyte>("byteMethod") == A_BYTE_VALUE);
    QVERIFY(testClass.callMethod<jbyte>("byteMethodWithArgs", "(BBB)B", 1, 1, 1) == A_BYTE_VALUE);

    // jchar --------------------------------------------------------------------------------------
    QVERIFY(QJniObject::callStaticMethod<jchar>(testClassName,
                                                       "staticCharMethod") == A_CHAR_VALUE);
    QVERIFY(QJniObject::callStaticMethod<jchar>(testClassName,
                                                "staticCharMethodWithArgs",
                                                "(CCC)C",
                                                jchar(1),
                                                jchar(1),
                                                jchar(1)) == A_CHAR_VALUE);

    QVERIFY(testClass.callMethod<jchar>("charMethod") == A_CHAR_VALUE);
    QVERIFY(testClass.callMethod<jchar>("charMethodWithArgs",
                                        "(CCC)C",
                                        jchar(1),
                                        jchar(1),
                                        jchar(1)) == A_CHAR_VALUE);

    // jshort -------------------------------------------------------------------------------------
    QVERIFY(QJniObject::callStaticMethod<jshort>(testClassName,
                                                        "staticShortMethod") == A_SHORT_VALUE);
    QVERIFY(QJniObject::callStaticMethod<jshort>(testClassName,
                                                 "staticShortMethodWithArgs",
                                                 "(SSS)S",
                                                 jshort(1),
                                                 jshort(1),
                                                 jshort(1)) == A_SHORT_VALUE);

    QVERIFY(testClass.callMethod<jshort>("shortMethod") == A_SHORT_VALUE);
    QVERIFY(testClass.callMethod<jshort>("shortMethodWithArgs",
                                         "(SSS)S",
                                         jshort(1),
                                         jshort(1),
                                         jshort(1)) == A_SHORT_VALUE);

    // jint ---------------------------------------------------------------------------------------
    QVERIFY(QJniObject::callStaticMethod<jint>(testClassName,
                                                      "staticIntMethod") == A_INT_VALUE);
    QVERIFY(QJniObject::callStaticMethod<jint>(testClassName,
                                               "staticIntMethodWithArgs",
                                               "(III)I",
                                               jint(1),
                                               jint(1),
                                               jint(1)) == A_INT_VALUE);

    QVERIFY(testClass.callMethod<jint>("intMethod") == A_INT_VALUE);
    QVERIFY(testClass.callMethod<jint>("intMethodWithArgs",
                                       "(III)I",
                                       jint(1),
                                       jint(1),
                                       jint(1)) == A_INT_VALUE);

    // jlong --------------------------------------------------------------------------------------
    QVERIFY(QJniObject::callStaticMethod<jlong>(testClassName,
                                                       "staticLongMethod") == A_LONG_VALUE);
    QVERIFY(QJniObject::callStaticMethod<jlong>(testClassName,
                                                "staticLongMethodWithArgs",
                                                "(JJJ)J",
                                                jlong(1),
                                                jlong(1),
                                                jlong(1)) == A_LONG_VALUE);

    QVERIFY(testClass.callMethod<jlong>("longMethod") == A_LONG_VALUE);
    QVERIFY(testClass.callMethod<jlong>("longMethodWithArgs",
                                        "(JJJ)J",
                                        jlong(1),
                                        jlong(1),
                                        jlong(1)) == A_LONG_VALUE);

    // jfloat -------------------------------------------------------------------------------------
    QVERIFY(QJniObject::callStaticMethod<jfloat>(testClassName,
                                                        "staticFloatMethod") == A_FLOAT_VALUE);
    QVERIFY(QJniObject::callStaticMethod<jfloat>(testClassName,
                                                 "staticFloatMethodWithArgs",
                                                 "(FFF)F",
                                                 jfloat(1.1),
                                                 jfloat(1.1),
                                                 jfloat(1.1)) == A_FLOAT_VALUE);

    QVERIFY(testClass.callMethod<jfloat>("floatMethod") == A_FLOAT_VALUE);
    QVERIFY(testClass.callMethod<jfloat>("floatMethodWithArgs",
                                         "(FFF)F",
                                         jfloat(1.1),
                                         jfloat(1.1),
                                         jfloat(1.1)) == A_FLOAT_VALUE);

    // jdouble ------------------------------------------------------------------------------------
    QVERIFY(QJniObject::callStaticMethod<jdouble>(testClassName,
                                                         "staticDoubleMethod") == A_DOUBLE_VALUE);
    QVERIFY(QJniObject::callStaticMethod<jdouble>(testClassName,
                                                  "staticDoubleMethodWithArgs",
                                                  "(DDD)D",
                                                  jdouble(1.1),
                                                  jdouble(1.1),
                                                  jdouble(1.1)) == A_DOUBLE_VALUE);

    QVERIFY(testClass.callMethod<jdouble>("doubleMethod") == A_DOUBLE_VALUE);
    QVERIFY(testClass.callMethod<jdouble>("doubleMethodWithArgs",
                                          "(DDD)D",
                                          jdouble(1.1),
                                          jdouble(1.1),
                                          jdouble(1.1)) == A_DOUBLE_VALUE);

    // jobject ------------------------------------------------------------------------------------
    {
        QJniObject res = QJniObject::callStaticObjectMethod<jobject>(testClassName,
                                                                     "staticObjectMethod");
        QVERIFY(res.isValid());
    }

    {
        QJniObject res = testClass.callObjectMethod<jobject>("objectMethod");
        QVERIFY(res.isValid());
    }

    // jclass -------------------------------------------------------------------------------------
    {
        QJniObject res = QJniObject::callStaticObjectMethod<jclass>(testClassName,
                                                                    "staticClassMethod");
        QVERIFY(res.isValid());
        QJniEnvironment env;
        QVERIFY(env->IsInstanceOf(testClass.object(), res.object<jclass>()));
    }

    {
        QJniObject res = testClass.callObjectMethod<jclass>("classMethod");
        QVERIFY(res.isValid());
        QJniEnvironment env;
        QVERIFY(env->IsInstanceOf(testClass.object(), res.object<jclass>()));
    }
    // jstring ------------------------------------------------------------------------------------
    {
        QJniObject res = QJniObject::callStaticObjectMethod<jstring>(testClassName,
                                                                     "staticStringMethod");
        QVERIFY(res.isValid());
        QVERIFY(res.toString() == A_STRING_OBJECT());
    }

    {
        QJniObject res = testClass.callObjectMethod<jstring>("stringMethod");
        QVERIFY(res.isValid());
        QVERIFY(res.toString() == A_STRING_OBJECT());

    }
    // jthrowable ---------------------------------------------------------------------------------
    {
        // The Throwable object the same message (see: "getMessage()") as A_STRING_OBJECT
        QJniObject res = QJniObject::callStaticObjectMethod<jthrowable>(testClassName,
                                                                        "staticThrowableMethod");
        QVERIFY(res.isValid());
        QVERIFY(res.callObjectMethod<jstring>("getMessage").toString() == A_STRING_OBJECT());
    }

    {
        QJniObject res = testClass.callObjectMethod<jthrowable>("throwableMethod");
        QVERIFY(res.isValid());
        QVERIFY(res.callObjectMethod<jstring>("getMessage").toString() == A_STRING_OBJECT());
    }

    // jobjectArray -------------------------------------------------------------------------------
    {
        QJniObject res = QJniObject::callStaticObjectMethod<jobjectArray>(testClassName,
                                                                          "staticObjectArrayMethod");
        QVERIFY(res.isValid());
    }

    {
        QJniObject res = testClass.callObjectMethod<jobjectArray>("objectArrayMethod");
        QVERIFY(res.isValid());
    }

    // jbooleanArray ------------------------------------------------------------------------------
    {
        QJniObject res = QJniObject::callStaticObjectMethod<jbooleanArray>(testClassName,
                                                                           "staticBooleanArrayMethod");
        QVERIFY(res.isValid());
    }

    {
        QJniObject res = testClass.callObjectMethod<jbooleanArray>("booleanArrayMethod");
        QVERIFY(res.isValid());
    }

    // jbyteArray ---------------------------------------------------------------------------------
    {
        QJniObject res = QJniObject::callStaticObjectMethod<jbyteArray>(testClassName,
                                                                        "staticByteArrayMethod");
        QVERIFY(res.isValid());
    }

    {
        QJniObject res = testClass.callObjectMethod<jbyteArray>("byteArrayMethod");
        QVERIFY(res.isValid());
    }

    // jcharArray ---------------------------------------------------------------------------------
    {
        QJniObject res = QJniObject::callStaticObjectMethod<jcharArray>(testClassName,
                                                                        "staticCharArrayMethod");
        QVERIFY(res.isValid());
    }

    {
        QJniObject res = testClass.callObjectMethod<jcharArray>("charArrayMethod");
        QVERIFY(res.isValid());
    }

    // jshortArray --------------------------------------------------------------------------------
    {
        QJniObject res = QJniObject::callStaticObjectMethod<jshortArray>(testClassName,
                                                                         "staticShortArrayMethod");
        QVERIFY(res.isValid());
    }

    {
        QJniObject res = testClass.callObjectMethod<jshortArray>("shortArrayMethod");
        QVERIFY(res.isValid());
    }

    // jintArray ----------------------------------------------------------------------------------
    {
        QJniObject res = QJniObject::callStaticObjectMethod<jintArray>(testClassName,
                                                                       "staticIntArrayMethod");
        QVERIFY(res.isValid());
    }

    {
        QJniObject res = testClass.callObjectMethod<jintArray>("intArrayMethod");
        QVERIFY(res.isValid());
    }

    // jlongArray ---------------------------------------------------------------------------------
    {
        QJniObject res = QJniObject::callStaticObjectMethod<jlongArray>(testClassName,
                                                                        "staticLongArrayMethod");
        QVERIFY(res.isValid());
    }

    {
        QJniObject res = testClass.callObjectMethod<jlongArray>("longArrayMethod");
        QVERIFY(res.isValid());
    }

    // jfloatArray --------------------------------------------------------------------------------
    {
        QJniObject res = QJniObject::callStaticObjectMethod<jfloatArray>(testClassName,
                                                                         "staticFloatArrayMethod");
        QVERIFY(res.isValid());
    }

    {
        QJniObject res = testClass.callObjectMethod<jfloatArray>("floatArrayMethod");
        QVERIFY(res.isValid());
    }

    // jdoubleArray -------------------------------------------------------------------------------
    {
        QJniObject res = QJniObject::callStaticObjectMethod<jdoubleArray>(testClassName,
                                                                          "staticDoubleArrayMethod");
        QVERIFY(res.isValid());
    }

    {
        QJniObject res = testClass.callObjectMethod<jdoubleArray>("doubleArrayMethod");
        QVERIFY(res.isValid());
    }

}

void tst_QJniObject::isClassAvailable()
{
    QVERIFY(QJniObject::isClassAvailable("java/lang/String"));
    QVERIFY(!QJniObject::isClassAvailable("class/not/Available"));
    QVERIFY(QJniObject::isClassAvailable("org/qtproject/qt/android/QtActivityDelegate"));
}

void tst_QJniObject::fromLocalRef()
{
    const int limit = 512 + 1;
    QJniEnvironment env;
    for (int i = 0; i != limit; ++i)
        QJniObject o = QJniObject::fromLocalRef(env->FindClass("java/lang/String"));
}

QTEST_MAIN(tst_QJniObject)

#include "tst_qjniobject.moc"