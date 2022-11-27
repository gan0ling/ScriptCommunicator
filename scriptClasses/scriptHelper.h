#ifndef SCRIPTHELPER_H
#define SCRIPTHELPER_H

#include <QJSValueIterator>
#include <QJSEngine>
#include <QObject>

namespace ScriptHelper
{


    ///Converts a byte array to a variant list.
    static inline QList<QVariant> byteArrayToVariantList(QByteArray byteArray)
    {
        QList<QVariant> list;
        for(auto el : byteArray){list.push_back((unsigned char) el);}
        return list;
    }

    ///Converts a variant list to a byte array.
    static inline QByteArray variantListToByteArray(QList<QVariant> list)
    {
        QByteArray bytesArray;
        for(auto el : list){bytesArray.append(el.toByteArray());}
        return bytesArray;
    }

}
///Script map class.
class ScriptMap : public QMap<QString, QVariant>
{
public:

    ScriptMap(){}
    ScriptMap(QMap<QString, QVariant> map)
    {
        QMap<QString,QVariant>::const_iterator it(map.begin());
        for(; it != map.end(); ++it)
        {
            insert(it.key(), it.value());
        }
    }

    static QJSValue ScriptMapToScriptValue(QJSEngine* eng, const ScriptMap& map)
    {
        QJSValue a = eng->newObject();
        ScriptMap::const_iterator it(map.begin());
        for(; it != map.end(); ++it)
        {
            QString key = it.key();
            QVariant val = it.value();
            if(val.type() == QVariant::ByteArray){val = ScriptHelper::byteArrayToVariantList(val.toByteArray());}
            // QVariant *obj = new QVariant(val);
            // QJSValue script_obj = eng->newQObject((QObject*)obj);
            a.setProperty(key, eng->toScriptValue(val));
        }
        return a;
    }

    static void ScriptMapFromScriptValue( const QJSValue& value, ScriptMap& map)
    {
        QJSValueIterator itr(value);
        while(itr.hasNext())
        {
            itr.next();
            map[itr.name()] = qjsvalue_cast<ScriptMap::mapped_type>(itr.value());
            if(map[itr.name()].type() == QVariant::List){map[itr.name()] = ScriptHelper::variantListToByteArray(map[itr.name()].toList());}
        }
    }

    static void registerScriptMetaTypes(QJSEngine* engine)
    {
        // qScriptRegisterMetaType<ScriptMap>(engine, ScriptMapToScriptValue, ScriptMapFromScriptValue);
    }
}; // work around because typedefs do not register correctly.
Q_DECLARE_METATYPE(ScriptMap)


#endif // SCRIPTHELPER_H
