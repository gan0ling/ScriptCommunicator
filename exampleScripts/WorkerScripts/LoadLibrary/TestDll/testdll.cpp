#include "testdll.h"



TestDll testDll;

TestDll::TestDll() : QObject()
{
}
void init(QJSEngine* engine)
{
    qRegisterMetaType<QJSEngine*>("TestDll*");

    engine->globalObject().setProperty("TestDll", engine->newQObject(&testDll));

    QJSValue sendDataArrayFunction = engine->evaluate("sendDataArray");

    testDll.setSendDataArrayFunction(sendDataArrayFunction);
    testDll.setScriptEngine(engine);

}
